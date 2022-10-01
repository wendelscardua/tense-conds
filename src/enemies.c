#include "directions.h"
#include "enemies.h"
#include "globals.h"
#include "subrand.h"

#pragma bss-name(push, "BSS")

unsigned int enemy_x[MAX_ENEMIES];
unsigned int enemy_y[MAX_ENEMIES];
direction_t enemy_direction[MAX_ENEMIES];
signed int enemy_sx[MAX_ENEMIES];
signed int enemy_sy[MAX_ENEMIES];
enemy_t enemy_type[MAX_ENEMIES];
unsigned char enemy_hp[MAX_ENEMIES];

#pragma bss-name(pop)

void init_enemies() {
  for(i = 0; i < MAX_ENEMIES; i++) {
    enemy_hp[i] = 0;
  }
}

// TODO report collisions
void update_enemies() {
  for(i = 0; i < MAX_ENEMIES; i++) {
    if (enemy_hp[i] == 0) continue;

  }
}

void render_enemies() {
  // TODO
}
