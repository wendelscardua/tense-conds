#ifndef _METATILES_H_
#define _METATILES_H_

extern const char metatiles[];

typedef enum
  {
   NullMetatile,
   FloorMetatile,
   WallMetatile,
   ConditionerMetatile = 16,
  } metatile_t;

#endif
