#include "lib/nesdoug.h"
#include "attributes.h"
#include "globals.h"

#pragma bss-name(push, "ZEROPAGE")
signed char attr_index;
unsigned char attr_shift;
unsigned char meta_x, meta_y;
#pragma bss-name(pop)

#pragma bss-name(push, "BSS")
unsigned char attribute_table[64];
unsigned char dirty_attribute_table[64];
#pragma bss-name(pop)

void reset_attributes() {
  for(i = 0; i < 64; i++) {
    attribute_table[i] = 0;
    dirty_attribute_table[i] = 0;
  }
}

// coordinates given by temp_x, temp_y
void set_attribute(unsigned char attribute) {
  meta_y = temp_y >> 1;
  meta_x = temp_x >> 1;
  attr_index = (meta_y / 2) * 32 / 4 + (meta_x / 2);
  attr_shift = 2 * ((meta_y % 2) * 2 + meta_x % 2);
  temp = attribute_table[attr_index];
  temp &= (0b11111111 ^ (0b11 << attr_shift));
  temp |= (attribute << attr_shift);
  attribute_table[attr_index] = temp;
  dirty_attribute_table[attr_index] = 1;
}

void flush_attributes() {
  temp = 0;
  for(attr_index = 63; attr_index >= 0; --attr_index) {
    if (dirty_attribute_table[attr_index]) {
      temp++;
      dirty_attribute_table[attr_index] = 0;
    } else if (temp) {
      multi_vram_buffer_horz(attribute_table + attr_index + 1, temp, 0x23c0 + attr_index + 1);
      temp = 0;
    }
  }
  if (temp) {
    multi_vram_buffer_horz(attribute_table + attr_index + 1, temp, 0x23c0 + attr_index + 1);
  }
}
