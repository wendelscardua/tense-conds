#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "directions.h"
#include "enemies.h"
#include "globals.h"
#include "map.h"
#include "player.h"
#include "subrand.h"
#include "metasprites.h"
#include "enemies-optimizations.h"
#include "sine-deltas.h"

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
unsigned char enemy_aux[MAX_ENEMIES];

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

  switch(enemy) {
  case ZombieEnemy:
    do {
      temp_x = subrand8(16);
      temp_y = subrand8(12);
    } while(map_collision());

    enemy_x[i_enemy] = (temp_x * 16) << 8;
    enemy_y[i_enemy] = (temp_y * 16) << 8;
    enemy_direction[i_enemy] = subrand8(4);
    enemy_hp[i_enemy] = 1;
    enemy_speed[i_enemy] = 0x0060;
    break;
  case BatEnemy:
    if (rand8() < 128) {
      enemy_direction[i_enemy] = DirectionRight;
      enemy_x[i_enemy] = 0x0000;
    } else {
      enemy_direction[i_enemy] = DirectionLeft;
      enemy_x[i_enemy] = 0xf000;
    }
    enemy_y[i_enemy] = (0x20 + subrand8(0x90 - 0x20)) << 8;
    enemy_hp[i_enemy] = 1;
    enemy_speed[i_enemy] = 0x0028;
    enemy_aux[i_enemy] = 0x00;
    break;
  }
}

void bat_enemy_update() {
  if (enemy_direction[i_enemy] == DirectionLeft) {
    if (enemy_x[i_enemy] < enemy_speed[i_enemy]) {
      enemy_hp[i_enemy] = 0;
      return;
    }
    enemy_x[i_enemy] -= enemy_speed[i_enemy];
  } else {
    if (enemy_x[i_enemy] >= 0xf000 - enemy_speed[i_enemy]) {
      enemy_hp[i_enemy] = 0;
      return;
    }
    enemy_x[i_enemy] += enemy_speed[i_enemy];
  }
  temp = enemy_aux[i_enemy];
  enemy_y[i_enemy] += sine_deltas[temp];

  if (++temp == NUM_DELTAS) enemy_aux[i_enemy] = 0;
  else enemy_aux[i_enemy] = temp;
}

// TODO report collisions
void update_enemies() {
  for(i_enemy = 0; i_enemy < MAX_ENEMIES; i_enemy++) {
    if (enemy_hp[i_enemy] == 0) continue;

    if (player_iframes == 0 && enemy_player_collision()) {
      enemy_hp[i_enemy] = 0;
      damage_player();
      continue;
    }

    // move enemy

    switch(enemy_type[i_enemy]) {
    case ZombieEnemy:
      zombie_enemy_update();
      break;
    case BatEnemy:
      bat_enemy_update();
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
      temp = FLYER_FLY;
      if (enemy_direction[shuffled_enemy] == DirectionRight) temp += 2;
      if ((temp_x ^ temp_y) & 0b100) {
        temp++;
      }
      oam_meta_spr(temp_x, temp_y, metasprite_pointers[temp]);
      break;
    default:
      break;
    }
  }
}

void enemy_haste(enemy_t enemy) {
  for(i_enemy = 0; i_enemy < MAX_ENEMIES; i_enemy++) {
    if (enemy_hp[i_enemy] && enemy_type[i_enemy] == enemy) {
      if (enemy_speed[i_enemy] < 0x0200) {
        enemy_speed[i_enemy] += enemy_speed[i_enemy] >> 1;
      }
    }
  }
}
