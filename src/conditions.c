#include "conditions.h"
#include "globals.h"

#pragma bss-name(push, "ZEROPAGE")
char num_conditions, num_conditions_in_pool;
char i_cond, j_cond;
condition_t temp_cond;
unsigned char cond_pool_weight;
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")

char condition_row[MAX_CONDITIONS];
char condition_column[MAX_CONDITIONS];
char condition_seconds[MAX_CONDITIONS];
char condition_frames[MAX_CONDITIONS];
condition_t condition_type[MAX_CONDITIONS];

condition_t condition_pool[CondTotal];
char unlocked[CondTotal];

#pragma bss-name(pop)

// which conds are unlocked when adding the current cond
const condition_t unlockables[][] =
  {
   // CondConditioner
   { CondZombieSpawner, CondTotal },
   // CondZombieSpawner
   { CondTotal }
  };

// weighted probabilities for each type of cond
const char weights[] =
  {
   0, // CondConditioner
   10, // CondZombieSpawner
  };

void init_conditions() {
  num_conditions = 0;
  num_conditions_in_pool = 0;
}

void update_condition_pool(condition_t condition) {
  if (unlocked[condition]) return;
  unlocked[condition] = 1;
  for(i_cond = 0; (temp_cond = unlockables[condition][i_cond]) != CondTotal; i_cond++) {
    for(j_cond = 0; j < num_conditions_in_pool; j++) {
      if (condition_pool[j_cond] == temp_cond) break;
    }
    if (j == num_conditions_in_pool) {
      condition_pool[num_conditions_in_pool] = temp_cond;
      num_conditions_in_pool++;
      cond_pool_weight += weights[temp_cond];
    }
  }
}

condition_t random_condition() {
  temp = subrand8(cond_pool_weight);
  for(i_cond = 0; i_cond < num_conditions_in_pool; i_cond) {
    temp_cond = condition_pool[i_cond];
    if (temp < weights[temp_cond]) return temp_cond;
    temp -= weights[temp_cond];
  }
  return CondConditioner; // should never happen
}
