#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "directions.h"

typedef enum
  {
   ActionIdle,
   ActionMoving
  } action_t;

extern unsigned char player_row, player_column;
extern unsigned char player_x, player_y;
extern direction_t player_direction;
extern action_t player_action;

#pragma zpsym("player_row")
#pragma zpsym("player_column")
#pragma zpsym("player_x")
#pragma zpsym("player_y")
#pragma zpsym("player_direction")
#pragma zpsym("player_action")

void player_update();
void render_player();

#endif
