#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/donut.h"
#include "lib/unrle.h"
#include "palettes.h"
#include "chr-data.h"
#include "nametables.h"
#include "metasprites.h"
#include "metatiles.h"
#include "globals.h"
#include "attributes.h"
#include "subrand.h"
#include "gamestate.h"
#include "level-state.h"
#include "ggsound/ggsound-api.h"
#include "music/soundtrack.h"
#include "levels.h"
#include "conditions.h"

typedef enum
  {
   PlayerEntity
  } entity_t;

typedef enum
  {
   DirectionUp,
   DirectionDown,
   DirectionLeft,
   DirectionRight
  } direction_t;

typedef enum
  {
   ActionIdle,
   ActionMoving
  } action_t;

#pragma bss-name(push, "ZEROPAGE")
unsigned char player_row, player_column;
unsigned char player_x, player_y;
direction_t player_direction;
action_t player_action;
char * level_data;
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

char map[13 * 16];

#pragma bss-name(pop)

// input is temp_x column, temp_y row
unsigned char map_collision() {
  return map[temp_y * 16 + temp_x] == WallMetatile;
}

void level_state_init() {
  oam_size(0);

  vram_adr(PPU_PATTERN_TABLE_0);
  donut_stream_ptr = &gameplay_bg_chr;
  donut_decompress_to_ppu(PPU_PATTERN_TABLE_SIZE / 64);

  vram_adr(PPU_PATTERN_TABLE_1);
  donut_stream_ptr = &gameplay_spr_chr;
  donut_decompress_to_ppu(PPU_PATTERN_TABLE_SIZE / 64);

  vram_adr(NTADR_A(0,0));
  donut_stream_ptr = &level_screen;
  donut_decompress_to_ppu(1024 / 64);

  pal_bg(gameplay_bg_palette);
  pal_spr(gameplay_spr_palette);

  set_mt_pointer(metatiles);

  reset_attributes();

  set_unrle_buffer((char *) map);

  // TODO multiple levels ? how/why?

  level_data = levels[0];

  // read entities
  temp = *level_data++;
  for (i = 0; i < temp; i++) {
    switch(*level_data++) {
    case PlayerEntity:
      player_column = *level_data++;
      player_row = *level_data++;
      player_x = (player_column * 16);
      player_y = (player_row * 16);
      player_direction = DirectionDown;
      player_action = ActionIdle;
      break;
    default:
      j = *level_data++;
      j = *level_data++;
    }
  }

  set_unrle_buffer((unsigned char *) map);
  unrle_to_buffer((unsigned char *) level_data);
  set_data_pointer(map);

  init_conditions();

  k = 0;
  for(i = 0; i < 13; i++) {
    for(j = 0; j < 16; j++, k++) {
      switch(map[k]) {
      case ConditionerMetatile:
        // Add conditioner
        ADD_CONDITION(i, j, CondConditioner);
        break;
      }
    }
  }

  temp_int = 0x2000;
  temp = 0;
  for(temp_y = 0; temp_y < 12; temp_y+=2) {
    for(temp_x = 0; temp_x < 16; temp_x+=2) {
      temp_int = 0x2000 + 2 * temp_x + 0x40 * temp_y;
      buffer_4_mt(temp_int, (temp_y << 4) | temp_x);
      flush_vram_update2();
    }
  }

  // TODO: fade in?
  ppu_on_all();

  ggsound_play_song(song_no_music);
}

void level_state_deinit() {
  // TODO: fade out?
  ppu_off();
}

void player_input() {
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

void conditions_update() {
  for(i = 0; i < num_conditions; i++) {
    if (condition_frames[i]++ >= 60) {
      condition_frames[i] = 0;
      temp_x = condition_column[i];
      temp_y = condition_row[i];
      temp_int = NTADR_A(2 * temp_x, 2 * temp_y);

      temp = ++condition_seconds[i];
      if (temp == 1) {
        one_vram_buffer(0xf1, temp_int++);
        one_vram_buffer(0xe0, temp_int);
      } else if (temp < 6) {
        one_vram_buffer(0xf0 + temp, temp_int);
      } else if (temp < 10) {
        one_vram_buffer(0xe0 - 5 + temp, temp_int + 1);
      } else {
        one_vram_buffer(0xe5, temp_int + 1);
        condition_seconds[i] = 0;
        // TODO: trigger
      }
    }
  }
}

void level_state_update() {
  oam_clear();

  player_input();

  conditions_update();

  // TODO: stuff

  flush_attributes();

  // TODO: render sprites
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
