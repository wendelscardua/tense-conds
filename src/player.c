#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "conditions.h"
#include "directions.h"
#include "gamestate.h"
#include "globals.h"
#include "player.h"
#include "map.h"
#include "metasprites.h"

#pragma bss-name(push, "ZEROPAGE")
unsigned char player_row, player_column;
unsigned char player_x, player_y;
direction_t player_direction;
action_t player_action;
unsigned char player_lives;
unsigned char player_iframes;
unsigned char player_score_tick;
unsigned int player_score;
unsigned char player_decimal_score[5];
unsigned char sword_durability;
#pragma bss-name(pop)

// temp_y is player row and temp_x is player column
void init_player() {
  player_column = temp_x;
  player_row = temp_y;
  player_x = (player_column * 16);
  player_y = (player_row * 16);
  player_direction = DirectionDown;
  player_action = ActionIdle;
  player_lives = PLAYER_STARTING_LIVES;
  player_iframes = 0;
  player_score = 0;
  player_score_tick = 60;
  for(i = 0; i < 5; i++) {
    player_decimal_score[i] = 0x10;
  }
  sword_durability = 0;
}

void player_update() {
  if (player_iframes > 0) player_iframes--;

  if (player_lives == 0) {
    if (player_iframes == 0) {
      gamestate_transition(GameOverState);
    }
    return;
  }

  player_score_tick--;
  if (player_score_tick == 0) {
    player_score_tick = 60;
    player_score++;
    i = 4;
    while(1) {
      player_decimal_score[i]++;
      if (player_decimal_score[i] == 0x1a) {
        player_decimal_score[i] = 0x10;
        i--;
      } else {
        break;
      }
    }
    multi_vram_buffer_horz(player_decimal_score + i, 5 - i, NTADR_A(SCORE_X + i, SCORE_Y));
  }

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
    if (pad_new & PAD_A) {
      temp_x = player_column;
      temp_y = player_row;
      switch(player_direction) {
      case DirectionUp:
        temp_y--;
        break;
      case DirectionDown:
        temp_y++;
        break;
      case DirectionLeft:
        temp_x--;
        break;
      case DirectionRight:
        temp_x++;
        break;
      }
      disable_condition();
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
  if (player_iframes > 0 && (get_frame_count() & 0b110) == 0) return;

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
  if (sword_durability > 0) {
    oam_meta_spr(temp_x, temp_y, metasprite_pointers[SWORD + player_direction]);
  }
}

void damage_player() {
  if (sword_durability > 0) {
    sword_lose();
    return;
  }
  player_iframes = PLAYER_IFRAMES;
  player_lives--;
  one_vram_buffer(HEARTLESS_TILE, NTADR_A(LIVES_X + player_lives, LIVES_Y));
}

void sword_get() {
  if (sword_durability < 10) {
    one_vram_buffer(SWORD_TILE, NTADR_A(SWORD_X + sword_durability, SWORD_Y));
    sword_durability++;
  }
}

void sword_lose() {
  if (sword_durability > 0) {
    sword_durability--;
    one_vram_buffer(SWORDLESS_TILE, NTADR_A(SWORD_X + sword_durability, SWORD_Y));
  }
}
