#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/donut.h"
#include "chr-data.h"
#include "gamestate.h"
#include "game-over-state.h"
#include "globals.h"
#include "nametables.h"
#include "palettes.h"
#include "player.h"
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


  // display seed
  temp = rng_seed & 0b1111;
  if (temp <= 9) {
    one_vram_buffer(temp + 0x10, NTADR_A(18, 15));
  } else {
    one_vram_buffer(temp - 10 + 0x21, NTADR_A(18, 15));
  }

  temp = (rng_seed >> 4) & 0b1111;
  if (temp <= 9) {
    one_vram_buffer(temp + 0x10, NTADR_A(17, 15));
  } else {
    one_vram_buffer(temp - 10 + 0x21, NTADR_A(17, 15));
  }

  temp = (rng_seed >> 8) & 0b1111;
  if (temp <= 9) {
    one_vram_buffer(temp + 0x10, NTADR_A(16, 15));
  } else {
    one_vram_buffer(temp - 10 + 0x21, NTADR_A(16, 15));
  }

  temp = (rng_seed >> 12) & 0b1111;
  if (temp <= 9) {
    one_vram_buffer(temp + 0x10, NTADR_A(15, 15));
  } else {
    one_vram_buffer(temp - 10 + 0x21, NTADR_A(15, 15));
  }

  flush_vram_update2();

  multi_vram_buffer_horz(player_decimal_score, 5, NTADR_A(16, 10));

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
