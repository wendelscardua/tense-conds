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

extern char num_conditions;
extern char condition_row[MAX_CONDITIONS];
extern char condition_column[MAX_CONDITIONS];
extern char condition_seconds[MAX_CONDITIONS];
extern char condition_frames[MAX_CONDITIONS];
extern condition_t condition_type[MAX_CONDITIONS];

#pragma zpsym("num_conditions")

#define ADD_CONDITION(row, column, condition)                           \
  {                                                                     \
    condition_row[num_conditions] = row;                                \
    condition_column[num_conditions] = column;                          \
    condition_type[num_conditions] = condition;                         \
    condition_seconds[num_conditions] = 0;                              \
    condition_frames[num_conditions] = subrand8(16);                    \
    update_condition_pool(condition);                                   \
    num_conditions++;                                                   \
  }

void init_conditions();
void update_condition_pool(condition_t condition);
condition_t random_condition();

#endif
