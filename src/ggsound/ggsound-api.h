#ifndef _GGSOUND_API_H_
#define _GGSOUND_API_H_

#include "../lib/unrom.h"

#pragma wrapped-call (push, farcall, bank)
#pragma code-name (push, "MUSIC")

// Kill all active streams and halt sound.
void ggsound_stop();

// Updates sound engine (should be used at the end of the nmi routine)
void ggsound_update();

// Plays a song
void ggsound_play_song(unsigned char song);

// Plays a sound effect (priority 1)
void ggsound_play_sfx_1(unsigned char sfx);

// Plays a sound effect (priority 2)
void ggsound_play_sfx_2(unsigned char sfx);

// Pauses a song
void ggsound_pause();

// Resumes a song
void ggsound_resume();

#pragma code-name (pop)
#pragma wrapped-call (pop)

#endif
