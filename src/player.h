#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "directions.h"

#define PLAYER_STARTING_LIVES 5
#define HEART_TILE 0x71
#define HEARTLESS_TILE 0x00
#define LIVES_X 19
#define LIVES_Y 26

#define SCORE_X 9
#define SCORE_Y 26

#define PLAYER_IFRAMES 45

typedef enum
  {
   ActionIdle,
   ActionMoving,
  } action_t;

extern unsigned char player_row, player_column;
extern unsigned char player_x, player_y;
extern direction_t player_direction;
extern action_t player_action;
extern unsigned char player_lives;
extern unsigned char player_iframes;

#pragma zpsym("player_row")
#pragma zpsym("player_column")
#pragma zpsym("player_x")
#pragma zpsym("player_y")
#pragma zpsym("player_direction")
#pragma zpsym("player_action")
#pragma zpsym("player_lives")
#pragma zpsym("player_iframes")

void init_player();
void player_update();
void render_player();
void damage_player();

#endif
