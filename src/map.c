#include "globals.h"
#include "metatiles.h"

#pragma bss-name(push, "BSS")

char map[16 * 13];

// input is temp_x column, temp_y row
unsigned char map_collision() {
  return map[temp_y * 16 + temp_x] == WallMetatile;
}

unsigned char map_fork() {
  return map[temp_y * 16 + temp_x] == ForkedFloorMetatile;
}
