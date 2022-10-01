#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

typedef enum
  {
   TitleState,
   LevelState
  } gamestate_t;

extern gamestate_t gamestate;

void gamestate_init();
void gamestate_deinit();
void gamestate_update();
void gamestate_transition(gamestate_t);

#endif
