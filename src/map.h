#ifndef _MAP_H_
#define _MAP_H_

extern char map[];
extern unsigned char cond_map[];

// input is temp_x column, temp_y row
unsigned char map_collision();
unsigned char map_fork();

#endif
