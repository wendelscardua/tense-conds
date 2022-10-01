.ifndef _NESLIB_INC_
  _NESLIB_INC_ = 1

  .import _pal_all,_pal_bg,_pal_spr,_pal_col,_pal_clear
  .import _pal_bright,_pal_spr_bright,_pal_bg_bright
  .import _ppu_off,_ppu_on_all,_ppu_on_bg,_ppu_on_spr,_ppu_mask,_ppu_system
  .import _oam_clear,_oam_size,_oam_spr,_oam_meta_spr,_oam_hide_rest
  .import _ppu_wait_frame,_ppu_wait_nmi
  .import _scroll,_split
  .import _bank_spr,_bank_bg
  .import _vram_read,_vram_write
  .import _pad_poll,_pad_trigger,_pad_state
  .import _rand8,_rand16,_set_rand
  .import _vram_adr,_vram_put,_vram_fill,_vram_inc,_vram_unrle
  .import _set_vram_update,_flush_vram_update
  .import _memcpy,_memfill,_delay

  .import _flush_vram_update2, _oam_set, _oam_get

  .import nmi, irq

  .importzp NTSC_MODE, FRAME_CNT1
  .importzp PPU_CTRL_VAR, PPU_MASK_VAR
  .importzp SCROLL_X, SCROLL_Y
  .importzp PAD_STATET
  .importzp RAND_SEED
  .importzp TEMP, PTR
.endif
