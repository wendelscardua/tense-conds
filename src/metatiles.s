.segment "RODATA"

.export _metatiles

_metatiles:
.byte $00, $00, $00, $00, $00 ; 00: null
.byte $b0, $b1, $c0, $c1, $03 ; 01: floor
.byte $b2, $b3, $c2, $c3, $00 ; 02: wall
.byte $f0, $e0, $d0, $d1, $01 ; 10: conditioner
.byte $b0, $b1, $c0, $c1, $03 ; 01: forked-floor
