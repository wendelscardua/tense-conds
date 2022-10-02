#ifndef _CONDITIONS_H_
#define _CONDITIONS_H_

#include "subrand.h"

typedef enum
  {
   CondConditioner,
   CondZombieSpawner,
   CondTotal
  } condition_t;

#define MAX_CONDITIONS 16

extern condition_t temp_cond;
extern char condition_row[MAX_CONDITIONS];
extern char condition_column[MAX_CONDITIONS];
extern char condition_seconds[MAX_CONDITIONS];
extern char condition_frames[MAX_CONDITIONS];
extern unsigned char condition_hp[MAX_CONDITIONS];
extern condition_t condition_type[MAX_CONDITIONS];

#pragma zpsym("temp_cond")

void init_conditions();
void add_condition();
void update_condition_pool();
condition_t random_condition();
void conditions_update();
void disable_condition();

#endif
