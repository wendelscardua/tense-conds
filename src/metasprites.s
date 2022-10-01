.feature force_range ; allows negative values

.segment "RODATA"

.define OAM_BEHIND $20
.define OAM_FLIP_H $40
.define OAM_FLIP_V $80

.export _metasprite_pointers

_metasprite_Round_Dark_data:


.byte   0,  0,$80,0
.byte   7,  0,$80,0|OAM_FLIP_H
.byte $80


_metasprite_Square_Dark_data:


.byte   0,  0,$84,0
.byte   7,  0,$84,0|OAM_FLIP_H
.byte $80


_metasprite_Heart_Dark_data:


.byte   0,  0,$88,0
.byte   7,  0,$88,0|OAM_FLIP_H
.byte $80


_metasprite_Round_Light_data:


.byte   0,  0,$82,0
.byte   7,  0,$82,0|OAM_FLIP_H
.byte $80


_metasprite_Square_Light_data:


.byte   0,  0,$86,0
.byte   7,  0,$86,0|OAM_FLIP_H
.byte $80


_metasprite_Heart_Light_data:


.byte   0,  0,$8a,0
.byte   7,  0,$8a,0|OAM_FLIP_H
.byte $80


_metasprite_Round_Chips_data:


.byte   0,  0,$60,0
.byte   8,  0,$62,0
.byte $80


_metasprite_Round_Dipped_data:


.byte   0,  0,$64,0
.byte   8,  0,$66,0
.byte $80


_metasprite_Round_Striped_data:


.byte   0,  0,$68,0
.byte   8,  0,$6a,0
.byte $80


_metasprite_Square_Chips_data:


.byte   0,  0,$6c,0
.byte   8,  0,$6e,0
.byte $80


_metasprite_Square_Dipped_data:


.byte   0,  0,$70,0
.byte   8,  0,$72,0
.byte $80


_metasprite_Square_Striped_data:


.byte   0,  0,$74,0
.byte   8,  0,$76,0
.byte $80


_metasprite_Heart_Chips_data:


.byte   0,  0,$78,0
.byte   8,  0,$7a,0
.byte $80


_metasprite_Heart_Dipped_data:


.byte   0,  0,$5c,0
.byte   8,  0,$5e,0
.byte $80


_metasprite_Heart_Striped_data:


.byte   0,  0,$7c,0
.byte   8,  0,$7e,0
.byte $80


_metasprite_pointers:

.word _metasprite_Round_Dark_data
.word _metasprite_Square_Dark_data
.word _metasprite_Heart_Dark_data
.word _metasprite_Round_Light_data
.word _metasprite_Square_Light_data
.word _metasprite_Heart_Light_data
.word _metasprite_Round_Chips_data
.word _metasprite_Round_Dipped_data
.word _metasprite_Round_Striped_data
.word _metasprite_Square_Chips_data
.word _metasprite_Square_Dipped_data
.word _metasprite_Square_Striped_data
.word _metasprite_Heart_Chips_data
.word _metasprite_Heart_Dipped_data
.word _metasprite_Heart_Striped_data
