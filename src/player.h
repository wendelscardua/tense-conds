#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "directions.h"

#define PLAYER_STARTING_LIVES 5

#define HEART_TILE 0x71
#define HEARTLESS_TILE 0x00
#define SWORD_TILE 0x73
#define SWORDLESS_TILE 0x00

#define LIVES_X 23
#define LIVES_Y 26

#define SCORE_X 10
#define SCORE_Y 26

#define SWORD_X 4
#define SWORD_Y 27

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
extern unsigned int player_score;
extern unsigned char player_decimal_score[];

#pragma zpsym("player_row")
#pragma zpsym("player_column")
#pragma zpsym("player_x")
#pragma zpsym("player_y")
#pragma zpsym("player_direction")
#pragma zpsym("player_action")
#pragma zpsym("player_lives")
#pragma zpsym("player_iframes")
#pragma zpsym("player_score")
#pragma zpsym("player_decimal_score")

void init_player();
void player_update();
void render_player();
void damage_player();
void sword_get();
void sword_lose();
void freeze_player();

#endif
