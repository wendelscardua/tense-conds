#include "lib/neslib.h"
#include "lib/donut.h"
#include "chr-data.h"
#include "game-over-state.h"
#include "nametables.h"
#include "palettes.h"
#include "ggsound/ggsound-api.h"
#include "music/soundtrack.h"

void game_over_state_init() {
  oam_size(0);

  vram_adr(PPU_PATTERN_TABLE_0);
  donut_stream_ptr = &gameplay_bg_chr;
  donut_decompress_to_ppu(PPU_PATTERN_TABLE_SIZE / 64);

  vram_adr(PPU_PATTERN_TABLE_1);
  donut_stream_ptr = &gameplay_spr_chr;
  donut_decompress_to_ppu(PPU_PATTERN_TABLE_SIZE / 64);

  vram_adr(NTADR_A(0,0));
  donut_stream_ptr = &game_over_screen;
  donut_decompress_to_ppu(1024 / 64);

  pal_bg(gameplay_bg_palette);
  pal_spr(gameplay_spr_palette);
  // TODO: fade in?
  ppu_on_all();

  ggsound_play_song(song_no_music);
}

void game_over_state_deinit() {
  // TODO: fade out?
  ppu_off();
}

void game_over_state_update() {
  oam_clear();
}
