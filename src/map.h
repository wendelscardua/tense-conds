#ifndef _MAP_H_
#define _MAP_H_

#include "conditions.h"

extern char map[];
extern condition_t cond_map[];

// input is temp_x column, temp_y row
unsigned char map_collision();
unsigned char map_fork();

#endif
