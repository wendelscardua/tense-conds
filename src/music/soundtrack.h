#ifndef _MUSIC_SOUNDTRACK_H_
#define _MUSIC_SOUNDTRACK_H_

typedef enum
  {
   song_no_music
  } music_t;

typedef enum
  {
   sfx_on,
   sfx_off
  } sfx_t;

extern const void * song_list;
extern const void * instrument_list;
extern const void * sfx_list;

#endif
