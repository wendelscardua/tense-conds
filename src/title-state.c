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

void title_state_update() {
  rand8();

  pad_poll(0);
  pad = pad_state(0);
  pad_new = get_pad_new(0);

  if (pad_new & (PAD_UP | PAD_LEFT)) {
    rand8();
    if (i > 0) i--;
  }

  if (pad_new & (PAD_DOWN | PAD_RIGHT | PAD_SELECT | PAD_B)) {
    rand8();
    if (i < 2) i++;
  }

  if (pad_new & (PAD_A | PAD_START)) {
    switch(i) {
    case 0:
      gamestate_transition(LevelState);
      break;
    }
    return;
  }

  // draw sprites
  oam_clear();

  switch(i) {
  case 0:
    oam_spr(0x26, 0xa6, 0xe8, 0x02);
    break;
  case 1:
    oam_spr(0x5e, 0xa6, 0xe8, 0x01);
    break;
  case 2:
    oam_spr(0x8e, 0xa6, 0xe8, 0x03);
    break;
  }
}
