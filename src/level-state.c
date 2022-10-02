#include "lib/donut.h"
#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "attributes.h"
#include "chr-data.h"
#include "conditions.h"
#include "enemies.h"
#include "gamestate.h"
#include "ggsound/ggsound-api.h"
#include "globals.h"
#include "level-state.h"
#include "levels.h"
#include "map.h"
#include "metatiles.h"
#include "music/soundtrack.h"
#include "nametables.h"
#include "palettes.h"
#include "player.h"
#include "subrand.h"

typedef enum
  {
   PlayerEntity
  } entity_t;

#pragma bss-name(push, "ZEROPAGE")
char * level_data;
#pragma bss-name(pop)

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

  temp_int = 0x2000;
  temp = 0;
  for(temp_y = 0; temp_y < 12; temp_y+=2) {
    for(temp_x = 0; temp_x < 16; temp_x+=2) {
      temp_int = 0x2000 + 2 * temp_x + 0x40 * temp_y;
      buffer_4_mt(temp_int, (temp_y << 4) | temp_x);
      flush_vram_update2();
    }
  }

  init_conditions();
  init_enemies();

  k = 0;
  for(temp_y = 0; temp_y < 13; temp_y++) {
    for(temp_x = 0; temp_x < 16; temp_x++, k++) {
      switch(map[k]) {
      case ConditionerMetatile:
        // Add conditioner
        temp_cond = CondConditioner;
        add_condition();
        map[k] = WallMetatile;
        break;
      }
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

void conditions_update() {
  for(i = 0; i < MAX_CONDITIONS; i++) {
    if (condition_hp[i] == 0) continue;
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
          map[temp_y * 16 + temp_x] = WallMetatile; // conditions act as walls
          break;
        case CondZombieSpawner:
          do {
            temp_x = subrand8(16);
            temp_y = subrand8(12);
          } while(map_collision());
          add_enemy(ZombieEnemy);
          break;
        }
        if (condition_hp[i] != 0xff && (--condition_hp[i]) == 0) {
          // TODO: delay shutting down
          multi_vram_buffer_horz(metatiles + 5, 2, temp_int);
          multi_vram_buffer_horz(metatiles + 7, 2, temp_int + 0x20);
          temp_x = condition_column[i];
          temp_y = condition_row[i];
          set_attribute(0x00);
          map[temp_y * 16 + temp_x] = FloorMetatile;
        }
      }
    }
  }
}

void level_state_update() {
  oam_clear();

  player_update();

  conditions_update();
  update_enemies();

  flush_attributes();

  render_enemies();

  render_player();
}
