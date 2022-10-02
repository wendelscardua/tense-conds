#include "lib/nesdoug.h"
#include "gamestate.h"
#include "title-state.h"
#include "level-state.h"
#include "game-over-state.h"

#pragma bss-name(push, "ZEROPAGE")
gamestate_t gamestate;
unsigned int rng_seed;
unsigned char fixed_seed;
#pragma bss-name(pop)

#ifdef DEBUG
unsigned char frame, old_frame;
unsigned char lag_frames, max_lag;
#endif

void gamestate_update() {
#ifdef DEBUG
  old_frame = frame;
  frame = get_frame_count();
  if (frame > old_frame && frame - old_frame > 1) {
    lag_frames++;
    if (frame - old_frame > max_lag) {
      max_lag = frame - old_frame;
    }
  }
#endif
  switch(gamestate) {
  case TitleState:
    title_state_update();
    break;
  case LevelState:
    level_state_update();
    break;
  case GameOverState:
    game_over_state_update();
    break;
  }
}

void gamestate_transition(gamestate_t new_gamestate) {
  if (new_gamestate != gamestate) {
    gamestate_deinit();
  }
  gamestate = new_gamestate;
  gamestate_init();
}

void gamestate_init() {
  switch(gamestate) {
  case TitleState:
    title_state_init();
    break;
  case LevelState:
    level_state_init();
    break;
  case GameOverState:
    game_over_state_init();
    break;
  }
#ifdef DEBUG
  frame = old_frame = get_frame_count();
  lag_frames = 0;
  max_lag = 0;
#endif
}

void gamestate_deinit() {
  switch(gamestate) {
  case TitleState:
    title_state_deinit();
    break;
  case LevelState:
    level_state_deinit();
    break;
  case GameOverState:
    game_over_state_deinit();
    break;
  }
}
