#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "conditions.h"
#include "attributes.h"
#include "globals.h"
#include "enemies.h"
#include "player.h"
#include "map.h"
#include "metatiles.h"
#include "ggsound/ggsound-api.h"
#include "music/soundtrack.h"

#pragma bss-name(push, "ZEROPAGE")
char num_conditions_in_pool;
char i_cond, j_cond;
condition_t temp_cond, temp_cond_2;
unsigned char cond_pool_weight;
condition_t condition_pool[CondTotal];
char unlocked[CondTotal];
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

char condition_row[MAX_CONDITIONS];
char condition_column[MAX_CONDITIONS];
char condition_seconds[MAX_CONDITIONS];
char condition_frames[MAX_CONDITIONS];
condition_t condition_type[MAX_CONDITIONS];
unsigned char condition_hp[MAX_CONDITIONS];

#pragma bss-name(pop)

// tiles for empty timer
const char empty_timer[] = { 0xf0, 0xe0 };

// tiles for each condition
const char condition_icon[][] =
  {
   // Conditioner
   { 0xd0, 0xd1 },
   // Zombie spawner
   { 0xd2, 0xd3 },
   // Bat spawner
   { 0xda, 0xdb },
   // Zombie haste
   { 0xd4, 0xd5 },
   // Bat haste
   { 0xde, 0xdf },
   // Sword
   { 0xd8, 0xd9 },
   // Ice
   { 0xd6, 0xd7 },
   // Teleport
   { 0xdc, 0xdd },
  };

// which conds are unlocked when adding the current cond
const condition_t unlockables[CondTotal][CondTotal] =
  {
   // Conditioner
   { CondZombieSpawner, CondBatSpawner, CondTotal },
   // Zombie spawner
   { CondZombieHaste, CondSword, CondIce, CondTotal },
   // Bat spawner
   { CondBatHaste, CondSword, CondIce, CondTotal },
   // Zombie haste
   { CondTotal },
   // Bat haste
   { CondTotal },
   // Sword
   { CondTeleport, CondTotal },
   // Ice
   { CondTotal },
   // Teleport
   { CondTotal },
  };

// weighted probabilities for each type of cond
const char weights[] =
  {
   0x00, // Conditioner
   0x24, // Zombie spawner
   0x20, // Bat spawner
   0x08, // Zombie haste
   0x08, // Bat haste
   0x10, // Sword
   0x18, // Ice
   0x08  // Teleport
  }; // Total: 0x80

// starting health for each cond
const unsigned char starting_health[] =
  {
   0xff, // Conditioner
   0x03, // Zombie spawner
   0x03, // Bat spawner
   0x02, // Zombie haste
   0x05, // Bat haste
   0x02, // Sword
   0x03, // Ice
   0x03, // Teleport
  };

void init_conditions() {
  num_conditions_in_pool = 0;
  cond_pool_weight = 0;
  for(i = 0; i < MAX_CONDITIONS; i++) {
    condition_hp[i] = 0;
  }
  for(i = 0; i < CondTotal; i++) {
    unlocked[i] = 0;
  }
}

// inputs: row temp_y, column temp_x, condition temp_cond
void add_condition() {
  for(i_cond = 0; i_cond < MAX_CONDITIONS; i_cond++) {
    if (condition_hp[i_cond] == 0) break;
  }
  if (i_cond >= MAX_CONDITIONS) return; // TODO: maybe overwrite another one?

  condition_row[i_cond] = temp_y;
  condition_column[i_cond] = temp_x;
  condition_type[i_cond] = temp_cond;
  condition_seconds[i_cond] = 0;
  condition_hp[i_cond] = starting_health[temp_cond];
  condition_frames[i_cond] = subrand8(16);

  cond_map[temp_y * 16 + temp_x] = i_cond;

  update_condition_pool();
  temp_int = NTADR_A(2 * temp_x, 2 * temp_y);
  multi_vram_buffer_horz(empty_timer, 2, temp_int);
  multi_vram_buffer_horz(condition_icon[temp_cond], 2, temp_int + 0x20);
  set_attribute(0x01);
}

void update_condition_pool() {
  if (unlocked[temp_cond]) return;
  unlocked[temp_cond] = 1;
  for(i_cond = 0; (temp_cond_2 = unlockables[temp_cond][i_cond]) != CondTotal; i_cond++) {
    for(j_cond = 0; j_cond < num_conditions_in_pool; j_cond++) {
      if (condition_pool[j_cond] == temp_cond_2) break;
    }
    if (j_cond == num_conditions_in_pool) {
      condition_pool[num_conditions_in_pool] = temp_cond_2;
      num_conditions_in_pool++;
      cond_pool_weight += weights[temp_cond_2];
    }
  }
}

condition_t random_condition() {
  temp = subrand8(cond_pool_weight);
  for(i_cond = 0; i_cond < num_conditions_in_pool; i_cond++) {
    temp_cond = condition_pool[i_cond];
    if (temp < weights[temp_cond]) return temp_cond;
    temp -= weights[temp_cond];
  }
  return CondConditioner; // should never happen
}

void conditions_update() {
  for(i = 0; i < MAX_CONDITIONS; i++) {
    if (condition_hp[i] == 0) continue;
    if (condition_frames[i]++ >= 60) {
      condition_frames[i] = 0;
      temp_x = condition_column[i];
      temp_y = condition_row[i];
      temp_int = NTADR_A(2 * temp_x, 2 * temp_y);

      temp = ++condition_seconds[i];
      if (temp == 0) {
        // comming back from disabled state
        set_attribute(0x01);
      } else if (temp == 1) {
        one_vram_buffer(0xf1, temp_int++);
        one_vram_buffer(0xe0, temp_int);
      } else if (temp < 6) {
        one_vram_buffer(0xf0 + temp, temp_int);
      } else if (temp < 10) {
        one_vram_buffer(0xe0 - 5 + temp, temp_int + 1);
      } else if (temp == 10) {
        one_vram_buffer(0xe5, temp_int + 1);
        condition_seconds[i] = 0;
        ggsound_play_sfx_1(sfx_on);
        switch(condition_type[i]) {
        case CondConditioner:
          // TODO: optimize (random map index, then convert to coordinates if needed
          // TODO: delay spawn with nice animation
          do {
            temp_x = subrand8(16);
            temp_y = subrand8(12);
          } while(map_collision());
          temp_cond = random_condition();
          add_condition();
          break;
        case CondZombieSpawner:
          add_enemy(ZombieEnemy);
          break;
        case CondBatSpawner:
          add_enemy(BatEnemy);
          break;
        case CondZombieHaste:
          enemy_haste(ZombieEnemy);
          break;
        case CondBatHaste:
          enemy_haste(BatEnemy);
          break;
        case CondSword:
          sword_get();
          break;
        case CondIce:
          freeze_player();
          break;
        case CondTeleport:
          teleport_player();
          break;
        }
        if (condition_hp[i] != 0xff && (--condition_hp[i]) == 0) {
          // TODO: delay shutting down
          multi_vram_buffer_horz(metatiles + 5, 2, temp_int);
          multi_vram_buffer_horz(metatiles + 7, 2, temp_int + 0x20);
          temp_x = condition_column[i];
          temp_y = condition_row[i];
          set_attribute(0x03);
          cond_map[temp_y * 16 + temp_x] = 0xff;
        }
      }
    }
  }
}

// located at temp_y row, temp_x column
void disable_condition() {
  temp = temp_y * 16 + temp_x;
  i_cond = cond_map[temp];
  if (i_cond == 0xff) return;

  if (condition_seconds[i_cond] > 10) return;

  condition_seconds[i_cond] = 0xf5 - subrand8(20);

  temp_int = NTADR_A(2 * temp_x, 2 * temp_y);
  one_vram_buffer(0xf0, temp_int++);
  one_vram_buffer(0xe0, temp_int);
  set_attribute(0x02);
  ggsound_play_sfx_2(sfx_off);
}
