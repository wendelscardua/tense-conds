#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

typedef enum
  {
   TitleState,
   LevelState,
   GameOverState,
  } gamestate_t;

extern gamestate_t gamestate;
extern unsigned int rng_seed;
extern unsigned char fixed_seed;
#pragma zpsym("gamestate")
#pragma zpsym("fixed_seed")

void gamestate_init();
void gamestate_deinit();
void gamestate_update();
void gamestate_transition(gamestate_t);

#endif
