.ifndef _SOUNDTRACK_INC_
  _SOUNDTRACK_INC_ = 1

  .include "../ggsound/ggsound.inc"

  .import song_list, instrument_list, sfx_list
  .ifdef DPCM_FEATURE
    .import dpcm_list
  .endif
.endif
