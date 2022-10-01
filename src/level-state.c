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

typedef enum
  {
   PlayerEntity
  } entity_t;

#pragma bss-name(push, "ZEROPAGE")
unsigned char player_row, player_column, player_x, player_y;
char * level_data;
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

char map[13 * 16];

#pragma bss-name(pop)

void level_state_init() {
  oam_size(1);

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
      player_x = player_column * 16;
      player_y = player_row * 16;
      break;
    default:
      j = *level_data++;
      j = *level_data++;
    }
  }

  set_unrle_buffer((unsigned char *) map);
  unrle_to_buffer((unsigned char *) level_data);
  set_data_pointer(map);

  k = 0;
  for(i = 0; i < 13; i++) {
    for(j = 0; j < 16; j++, k++) {
      switch(map[k]) {
      case ConditionerMetatile:
        // TODO add instance of conditioner
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


void level_state_update() {
  oam_clear();

  // TODO: stuff

  flush_attributes();

  // TODO: render sprites
}
