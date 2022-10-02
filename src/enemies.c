#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "directions.h"
#include "enemies.h"
#include "globals.h"
#include "map.h"
#include "subrand.h"
#include "metasprites.h"
#include "enemies-optimizations.h"

#pragma bss-name(push, "ZEROPAGE")
unsigned char i_enemy, shuffled_enemy;
unsigned int temp_enemy_x, temp_enemy_y;
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

unsigned int enemy_x[MAX_ENEMIES];
unsigned int enemy_y[MAX_ENEMIES];
direction_t enemy_direction[MAX_ENEMIES];
unsigned int enemy_speed[MAX_ENEMIES];
enemy_t enemy_type[MAX_ENEMIES];
unsigned char enemy_hp[MAX_ENEMIES];

#pragma bss-name(pop)

void init_enemies() {
  for(i_enemy = 0; i_enemy < MAX_ENEMIES; i_enemy++) {
    enemy_hp[i_enemy] = 0;
  }
}

void add_enemy(enemy_t enemy) {
  for(i_enemy = 0; i < MAX_ENEMIES; i_enemy++) {
    if (enemy_hp[i_enemy] == 0) break;
  }

  if (i_enemy >= MAX_ENEMIES) return; // TODO: maybe overwrite another enemy?

  enemy_type[i_enemy] = enemy;
  enemy_x[i_enemy] = (temp_x * 16) << 8;
  enemy_y[i_enemy] = (temp_y * 16) << 8;
  enemy_direction[i_enemy] = subrand8(4);
  enemy_hp[i_enemy] = 1;
  enemy_speed[i_enemy] = 0x0060;
}

unsigned char near_snap() {
  temp = (temp_enemy_x >> 8) & 0x0f;
  if (temp > 0x02 && temp < 0x0e) return 0;
  temp = (temp_enemy_y >> 8) & 0x0f;
  if (temp > 0x02 && temp < 0x0e) return 0;
  return 1;
}

// TODO report collisions
void update_enemies() {
  for(i_enemy = 0; i_enemy < MAX_ENEMIES; i_enemy++) {
    if (enemy_hp[i_enemy] == 0) continue;

    if (enemy_player_collision()) {
      enemy_hp[i_enemy] = 0;
      continue;
    }

    // move enemy

    switch(enemy_type[i_enemy]) {
    case ZombieEnemy:
      zombie_enemy_update();
      break;
    case BatEnemy:
      // TODO: fly around
      break;
    }


  }
}

void render_enemies() {
  shuffled_enemy = get_frame_count() & 0x0f;
  for(i_enemy = 0; i_enemy < MAX_ENEMIES; i_enemy++) {
    shuffled_enemy += 5; // coprime with MAX_ENEMIES
    if (shuffled_enemy >= MAX_ENEMIES) {
      shuffled_enemy -= MAX_ENEMIES;;
    }
    if (enemy_hp[shuffled_enemy] == 0) continue;
    temp_x = enemy_x[shuffled_enemy] >> 8;
    temp_y = (enemy_y[shuffled_enemy] >> 8) - 1;

    switch(enemy_type[shuffled_enemy]) {
    case ZombieEnemy:
      temp = ZOMBIE_WALK + 2 * enemy_direction[shuffled_enemy];
      if ((temp_x ^ temp_y) & 0b100) {
        temp++;
      }
      oam_meta_spr(temp_x, temp_y, metasprite_pointers[temp]);
      break;
    case BatEnemy:
      break;
    default:
      break;
    }
  }
}
