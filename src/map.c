#include "globals.h"
#include "metatiles.h"

#pragma bss-name(push, "ZEROPAGE")
char temp_index;
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

char map[16 * 13];
unsigned char cond_map[16 * 13];

#pragma bss-name(pop)

// input is temp_x column, temp_y row
unsigned char map_collision() {
  temp_index = temp_y * 16 + temp_x;
  return map[temp_index] == WallMetatile ||
    cond_map[temp_index] != 0xff;
}

unsigned char map_fork() {
  return map[temp_y * 16 + temp_x] == ForkedFloorMetatile;
}
