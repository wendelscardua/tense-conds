#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "level-state.h"
#include "gamestate.h"

void main (void) {

  fixed_seed = 0;

  ppu_off();

  bank_bg(0);
  bank_spr(1);

  set_vram_buffer();

  gamestate_transition(TitleState);

  while (1) {
    ppu_wait_nmi();
    gamestate_update();
  }
}
