#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#define MAX_ENEMIES 16

typedef enum
  {
   ZombieEnemy,
   BatEnemy,
  } enemy_t;

void init_enemies();
void update_enemies();
void render_enemies();

#endif
