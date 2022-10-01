.autoimport on
.importzp sp
.include "ggsound.inc"

.segment "MUSIC"

; void ggsound_stop()
.export _ggsound_stop
_ggsound_stop = sound_stop

; void ggsound_update()
.export _ggsound_update
.proc _ggsound_update
  soundengine_update
  rts
.endproc

; void ggsound_play_song(unsigned char song)
.export _ggsound_play_song
.proc _ggsound_play_song
  sta sound_param_byte_0
  jsr play_song
  rts
.endproc

; void ggsound_play_sfx_1(unsigned char sfx);
.export _ggsound_play_sfx_1
.proc _ggsound_play_sfx_1
  sta sound_param_byte_0
  lda #soundeffect_one
  sta sound_param_byte_1
  jsr play_sfx
  rts
.endproc

; void ggsound_play_sfx_2(unsigned char sfx);
.export _ggsound_play_sfx_2
.proc _ggsound_play_sfx_2
  sta sound_param_byte_0
  lda #soundeffect_two
  sta sound_param_byte_1
  jsr play_sfx
  rts
.endproc

; void ggsound_pause();
.export _ggsound_pause
_ggsound_pause = pause_song

; void ggsound_resume();
.export _ggsound_resume
_ggsound_resume = resume_song
