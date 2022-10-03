#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#define MAX_ENEMIES 16

typedef enum
  {
   ZombieEnemy,
   BatEnemy,
  } enemy_t;

void init_enemies();
void add_enemy(enemy_t enemy);
void enemy_haste(enemy_t enemy);
void update_enemies();
void render_enemies();

#endif
