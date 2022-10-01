#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "conditions.h"
#include "attributes.h"
#include "globals.h"

#pragma bss-name(push, "ZEROPAGE")
char num_conditions, num_conditions_in_pool;
char i_cond, j_cond;
condition_t temp_cond, temp_cond_2;
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

// tiles for empty timer
const char empty_timer[] = { 0xf0, 0xe0 };

// tiles for each condition
const char condition_icon[][] =
  {
   // CondConditioner
   { 0xd0, 0xd1 },
   // CondZombieSpawner
   { 0xd2, 0xd3 }
  };

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

// inputs: row temp_y, column temp_x, condition temp_cond
void add_condition() {
  if (num_conditions >= MAX_CONDITIONS) return;
  condition_row[num_conditions] = temp_y;
  condition_column[num_conditions] = temp_x;
  condition_type[num_conditions] = temp_cond;
  condition_seconds[num_conditions] = 0;
  condition_frames[num_conditions] = subrand8(16);
  update_condition_pool();
  num_conditions++;
  temp_int = NTADR_A(2 * temp_x, 2 * temp_y);
  multi_vram_buffer_horz(empty_timer, 2, temp_int);
  multi_vram_buffer_horz(condition_icon[temp_cond], 2, temp_int + 0x20);
  set_attribute(0x01);
}

void update_condition_pool() {
  if (unlocked[temp_cond]) return;
  unlocked[temp_cond] = 1;
  for(i_cond = 0; (temp_cond_2 = unlockables[temp_cond][i_cond]) != CondTotal; i_cond++) {
    for(j_cond = 0; j < num_conditions_in_pool; j++) {
      if (condition_pool[j_cond] == temp_cond_2) break;
    }
    if (j == num_conditions_in_pool) {
      condition_pool[num_conditions_in_pool] = temp_cond_2;
      num_conditions_in_pool++;
      cond_pool_weight += weights[temp_cond_2];
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
