.include "../ggsound/ggsound.inc"

.segment "MUSIC"

.export song_list
.export instrument_list
.export sfx_list

.ifdef DPCM_FEATURE
  .export dpcm_list
.endif

.feature force_range

.include "soundtrack.asm"

.ifdef DPCM_FEATURE
  .include "soundtrack_dpcm.asm"
.endif
