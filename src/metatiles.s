.segment "RODATA"

.export _metatile_UL, _metatile_UR, _metatile_DL, _metatile_DR, _metatile_AT

;                   nul, floor, wall, ruler
_metatile_UL: .byte $00, $b0,   $b2, $f0
_metatile_UR: .byte $00, $b1,   $b3, $e0
_metatile_DL: .byte $00, $c0,   $c2, $d0
_metatile_DR: .byte $00, $c1,   $c3, $d1
_metatile_AT: .byte $00, $00,   $00, $01
