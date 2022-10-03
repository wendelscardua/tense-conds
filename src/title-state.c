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

void title_state_init() {
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
  donut_stream_ptr = &title_screen;
  donut_decompress_to_ppu(1024 / 64);

  if (fixed_seed) {
    one_vram_buffer(0x08, NTADR_A(17, 23));
    one_vram_buffer(0x09, NTADR_A(22, 23));

    temp = rng_seed & 0b1111;
    if (temp <= 9) {
      one_vram_buffer(temp + 0x10, NTADR_A(21, 23));
    } else {
      one_vram_buffer(temp - 10 + 0x21, NTADR_A(21, 23));
    }

    temp = (rng_seed >> 4) & 0b1111;
    if (temp <= 9) {
      one_vram_buffer(temp + 0x10, NTADR_A(20, 23));
    } else {
      one_vram_buffer(temp - 10 + 0x21, NTADR_A(20, 23));
    }

    temp = (rng_seed >> 8) & 0b1111;
    if (temp <= 9) {
      one_vram_buffer(temp + 0x10, NTADR_A(19, 23));
    } else {
      one_vram_buffer(temp - 10 + 0x21, NTADR_A(19, 23));
    }

    temp = (rng_seed >> 12) & 0b1111;
    if (temp <= 9) {
      one_vram_buffer(temp + 0x10, NTADR_A(18, 23));
    } else {
      one_vram_buffer(temp - 10 + 0x21, NTADR_A(18, 23));
    }

    flush_vram_update2();
  }

  // i will index select item
  i = 0;

  // TODO: fade in?
  ppu_on_all();

  ggsound_play_song(song_no_music);
}

void title_state_deinit() {
  // TODO: fade out?
  ppu_off();
}

void mess_with_seed() {
  if (!fixed_seed) {
    rng_seed = rand16();
  }
}

void title_state_update() {
  mess_with_seed();

  pad_poll(0);
  pad = pad_state(0);
  pad_new = get_pad_new(0);

  if (pad_new & (PAD_UP | PAD_LEFT | PAD_DOWN | PAD_RIGHT | PAD_B | PAD_SELECT)) {
    mess_with_seed();
    i = 1 - i;
  }

  if (pad_new & (PAD_A | PAD_START)) {
    switch(i) {
    case 0:
      set_rand(rng_seed);
      fixed_seed = 0;
      gamestate_transition(LevelState);
      break;
    }
    return;
  }

  // draw sprites
  oam_clear();

  switch(i) {
  case 0:
    oam_spr(0x29, 0xb7, 0x30, 0x02);
    break;
  case 1:
    oam_spr(0x31, 0xbf, 0x30, 0x02);
    break;
  }
}
