#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/donut.h"
#include "chr-data.h"
#include "gamestate.h"
#include "title-state.h"
#include "metasprites.h"
#include "palettes.h"
#include "nametables.h"
#include "globals.h"
#include "ggsound/ggsound-api.h"
#include "music/soundtrack.h"
#include "hex-writer.h"

void refresh_seed() {
  WRITE_INT_AS_HEX(rng_seed, 6, 14, temp);
}

void seed_input_state_init() {
  oam_size(0);

  pal_bg(gameplay_bg_palette);
  pal_spr(gameplay_spr_palette);

  vram_adr(PPU_PATTERN_TABLE_0);
  donut_stream_ptr = &gameplay_bg_chr;
  donut_decompress_to_ppu(PPU_PATTERN_TABLE_SIZE / 64);

  vram_adr(PPU_PATTERN_TABLE_1);
  donut_stream_ptr = &gameplay_spr_chr;
  donut_decompress_to_ppu(PPU_PATTERN_TABLE_SIZE / 64);

  vram_adr(NTADR_A(0,0));
  donut_stream_ptr = &seed_input_screen;
  donut_decompress_to_ppu(1024 / 64);

  if (!fixed_seed) {
    rng_seed = rand16();
  }

  refresh_seed();
  flush_vram_update2();

  // i will index select item
  i = 0;

  // TODO: fade in?
  ppu_on_all();

  ggsound_play_song(song_no_music);
}

void seed_input_state_deinit() {
  // TODO: fade out?
  ppu_off();
}

void seed_input_state_update() {
  oam_clear();

  pad_poll(0);
  pad = pad_state(0);
  pad_new = get_pad_new(0);

  if (pad_new & PAD_LEFT) {
    if (i == 0) i = 5;
    else i--;
  } else if (pad_new & (PAD_RIGHT | PAD_SELECT)) {
    if (i == 5) i = 0;
    else i++;
  }

  if (i < 4) {
    oam_spr(0x30 + 0x08 * i, 0x67, 0x31, 0x02);
    oam_spr(0x30 + 0x08 * i, 0x77, 0x32, 0x02);
    // seed input
    if (pad_new & (PAD_UP | PAD_A)) {
      temp_int = (0xf << (4 * (3 - i)));
      rng_seed = (rng_seed & ~temp_int) | (((rng_seed & temp_int) + 0x1111) & temp_int);
      refresh_seed();
    }
    if (pad_new & PAD_DOWN) {
      temp_int = (0xf << (4 * (3 - i)));
      rng_seed = (rng_seed & ~temp_int) | (((rng_seed & temp_int) + 0xffff) & temp_int);
      refresh_seed();
    }
  } else if (i == 4) {
    oam_spr(0x58, 0x6f, 0x30, 0x03);
    // ok button
    if (pad_new & (PAD_A | PAD_START)) {
      fixed_seed = 1;
      gamestate_transition(TitleState);
    }
  } else {
    oam_spr(0x78, 0x6f, 0x30, 0x03);
    // cancel button
    if (pad_new & (PAD_A | PAD_START)) {
      fixed_seed = 0;
      gamestate_transition(TitleState);
    }
  }

  switch(i) {
  case 0:

    break;
  case 1:
    oam_spr(0x31, 0xbf, 0x30, 0x02);
    break;
  }
}
