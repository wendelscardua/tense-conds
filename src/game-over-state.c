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
#include "hex-writer.h"

#pragma bss-name(push, "ZEROPAGE")
unsigned long scorecode;
#pragma bss-name(pop)

void base64digit() {
  if (temp < 10) {
    temp = temp + 0x10;
  } else if (temp < 36) {
    temp = temp - 10 + 0x21;
  } else if (temp < 62) {
    temp = temp - 36 + 0x41;
  } else if (temp == 62) {
    temp = 0x1f; // ?
  } else if (temp == 63) {
    temp = 0x20; // @
  } else {
    temp = 0x01; // ! = bug
  }
}

void scorecode_stuff() {
  temp_int_1 = player_score;
  temp_int_2 = rng_seed;

  // first byte of each + checksum
  scorecode = 0;
  for(i = 0; i < 8; i++) {
    scorecode <<= 1;
    scorecode |= (temp_int_1 & 1);
    scorecode <<= 1;
    scorecode |= (temp_int_2 & 1);
    scorecode <<= 1;
    scorecode |= ((temp_int_1 ^ temp_int_2) & 1);
    temp_int_1 >>= 1;
    temp_int_2 >>= 1;
  }

  k = 11;

  temp = 0;
  while(scorecode >= 262144) {
    scorecode -= 262144;
    temp++;
  }

  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  temp = 0;
  while(scorecode >= 4096) {
    scorecode -= 4096;
    temp++;
  }

  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  temp = 0;
  while(scorecode >= 64) {
    scorecode -= 64;
    temp++;
  }

  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  temp = (unsigned char) (scorecode);
  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  // second byte of each + checksum

  scorecode = 0;
  for(i = 0; i < 8; i++) {
    scorecode <<= 1;
    scorecode |= (temp_int_1 & 1);
    scorecode <<= 1;
    scorecode |= (temp_int_2 & 1);
    scorecode <<= 1;
    scorecode |= ((temp_int_1 ^ temp_int_2) & 1);
    temp_int_1 >>= 1;
    temp_int_2 >>= 1;
  }

  temp = 0;
  while(scorecode >= 262144) {
    scorecode -= 262144;
    temp++;
  }

  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  temp = 0;
  while(scorecode >= 4096) {
    scorecode -= 4096;
    temp++;
  }

  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  temp = 0;
  while(scorecode >= 64) {
    scorecode -= 64;
    temp++;
  }

  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

  temp = (unsigned char) (scorecode);
  base64digit();
  one_vram_buffer(temp, NTADR_A(k++, 13));

}

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
  WRITE_INT_AS_HEX(rng_seed, 15, 15, temp);

  flush_vram_update2();

  multi_vram_buffer_horz(player_decimal_score, 5, NTADR_A(16, 10));

  scorecode_stuff();

  // TODO: fade in?
  ppu_on_all();

  ggsound_play_song(song_no_music);

  i = 0; // current selection
}

void game_over_state_deinit() {
  // TODO: fade out?
  ppu_off();
}

void game_over_state_update() {
  pad_poll(0);
  pad = pad_state(0);
  pad_new = get_pad_new(0);

  if (pad_new & (PAD_UP | PAD_LEFT | PAD_DOWN | PAD_RIGHT | PAD_B | PAD_SELECT)) {
    i = 1 - i;
  }

  if (pad_new & (PAD_A | PAD_START)) {
    switch(i) {
    case 0:
      set_rand(rng_seed);
      gamestate_transition(LevelState);
      break;
    case 1:
      gamestate_transition(TitleState);
      break;
    }
    return;
  }

  oam_clear();

  switch(i) {
  case 0:
    oam_spr(0x40, 0x97, 0x30, 0x02);
    break;
  case 1:
    oam_spr(0x40, 0xa7, 0x30, 0x02);
    break;
  }
}
