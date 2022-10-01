#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/donut.h"
#include "palettes.h"
#include "chr-data.h"
#include "nametables.h"
#include "metasprites.h"
#include "globals.h"
#include "attributes.h"
#include "subrand.h"
#include "gamestate.h"
#include "level-state.h"
#include "ggsound/ggsound-api.h"
#include "music/soundtrack.h"

#pragma bss-name(push, "ZEROPAGE")

#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

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

  reset_attributes();

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
