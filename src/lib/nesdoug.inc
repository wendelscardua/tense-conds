.ifndef _NESDOUG_INC_
  _NESDOUG_INC_ = 1

  .import _set_vram_buffer, _multi_vram_buffer_horz, _multi_vram_buffer_vert, _one_vram_buffer
  .import _get_pad_new, _get_frame_count
  .import _check_collision, _pal_fade_to, _set_scroll_x, _set_scroll_y, _add_scroll_y, _sub_scroll_y
  .import  _get_ppu_addr, _get_at_addr, _set_data_pointer, _set_mt_pointer, _buffer_4_mt, _buffer_1_mt
  .import _color_emphasis, _xy_split, _gray_line, _seed_rng

  .importzp VRAM_INDEX
.endif
