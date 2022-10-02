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
      temp_x = *level_data++;
      temp_y = *level_data++;
      init_player();
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

  for(i = 0; i < player_lives; i++) {
    one_vram_buffer(HEART_TILE, NTADR_A(LIVES_X + i, LIVES_Y));
  }
  flush_vram_update2();

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

void level_state_update() {
  oam_clear();

  player_update();

  if (gamestate == GameOverState) return;

  conditions_update();

  update_enemies();

  flush_attributes();

  render_enemies();

  render_player();
}
