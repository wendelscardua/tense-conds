#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "directions.h"
#include "globals.h"
#include "player.h"
#include "map.h"
#include "metasprites.h"

#pragma bss-name(push, "ZEROPAGE")
unsigned char player_row, player_column;
unsigned char player_x, player_y;
direction_t player_direction;
action_t player_action;
#pragma bss-name(pop)

void player_update() {
  switch(player_action) {
  case ActionIdle:
    pad_poll(0);
    pad = pad_state(0);
    pad_new = get_pad_new(0);

    temp_x = player_column;
    temp_y = player_row;
    if (pad & PAD_UP) {
      player_direction = DirectionUp;
      temp_y--;
      if (!map_collision()) {
        player_row = temp_y;
        player_action = ActionMoving;
      }
    } else if (pad & PAD_DOWN) {
      player_direction = DirectionDown;
      temp_y++;
      if (!map_collision()) {
        player_row = temp_y;
        player_action = ActionMoving;
      }
    } else if (pad & PAD_LEFT) {
      player_direction = DirectionLeft;
      temp_x--;
      if (!map_collision()) {
        player_column = temp_x;
        player_action = ActionMoving;
      }
    } else if (pad & PAD_RIGHT) {
      player_direction = DirectionRight;
      temp_x++;
      if (!map_collision()) {
        player_column = temp_x;
        player_action = ActionMoving;
      }
    }

    break;
  case ActionMoving:
    temp_x = player_column * 16;
    temp_y = player_row * 16;
    if (player_x < temp_x) {
      player_x++;
    } else if (player_x > temp_x) {
      player_x--;
    } else if (player_y < temp_y) {
      player_y++;
    } else if (player_y > temp_y) {
      player_y--;
    } else {
      player_action = ActionIdle;
    }

    break;
  }
}

void render_player() {
  temp_x = player_x;
  temp_y = player_y - 1;
  switch(player_action) {
  case ActionIdle:
    oam_meta_spr(temp_x, temp_y, metasprite_pointers[PLAYER_IDLE + player_direction]);
    break;
  case ActionMoving:
    temp = PLAYER_WALK + 2 * player_direction;
    if ((temp_x ^ temp_y) & 0b100) {
      temp++;
    }
    oam_meta_spr(temp_x, temp_y, metasprite_pointers[temp]);
    break;
  }
}
