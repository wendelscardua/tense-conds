#include "directions.h"
#include "player.h"

#pragma bss-name(push, "ZEROPAGE")
unsigned char player_row, player_column;
unsigned char player_x, player_y;
direction_t player_direction;
action_t player_action;
#pragma bss-name(pop)
