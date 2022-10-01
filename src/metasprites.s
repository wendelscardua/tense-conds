.feature force_range ; allows negative values

.segment "RODATA"

.define OAM_BEHIND $20
.define OAM_FLIP_H $40
.define OAM_FLIP_V $80

.export _metasprite_pointers

metasprite_playerIdleUp_data:


.byte   0,  0,$02,0
.byte   8,  0,$03,0
.byte   0,  8,$12,0
.byte   8,  8,$13,0
.byte $80


metasprite_playerIdleDown_data:


.byte   0,  0,$00,0
.byte   8,  0,$01,0
.byte   0,  8,$10,0
.byte   8,  8,$11,0
.byte $80


metasprite_playerIdleLeft_data:


.byte   0,  0,$04,0
.byte   8,  0,$05,0
.byte   0,  8,$14,0
.byte   8,  8,$15,0
.byte $80


metasprite_playerIdleRight_data:


.byte   8,  0,$04,0|OAM_FLIP_H
.byte   0,  0,$05,0|OAM_FLIP_H
.byte   8,  8,$14,0|OAM_FLIP_H
.byte   0,  8,$15,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkUp1_data:


.byte   0,  0,$08,0
.byte   8,  0,$09,0
.byte   0,  8,$18,0
.byte   8,  8,$19,0
.byte $80


metasprite_playerWalkUp2_data:


.byte   8,  0,$08,0|OAM_FLIP_H
.byte   0,  0,$09,0|OAM_FLIP_H
.byte   8,  8,$18,0|OAM_FLIP_H
.byte   0,  8,$19,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkDown1_data:


.byte   0,  0,$06,0
.byte   8,  0,$07,0
.byte   0,  8,$16,0
.byte   8,  8,$17,0
.byte $80


metasprite_playerWalkDown2_data:


.byte   8,  0,$06,0|OAM_FLIP_H
.byte   0,  0,$07,0|OAM_FLIP_H
.byte   8,  8,$16,0|OAM_FLIP_H
.byte   0,  8,$17,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkLeft1_data:


.byte   0,  0,$0a,0
.byte   8,  0,$0b,0
.byte   0,  8,$1a,0
.byte   8,  8,$1b,0
.byte $80


metasprite_playerWalkRight1_data:


.byte   8,  0,$0a,0|OAM_FLIP_H
.byte   0,  0,$0b,0|OAM_FLIP_H
.byte   8,  8,$1a,0|OAM_FLIP_H
.byte   0,  8,$1b,0|OAM_FLIP_H
.byte $80


metasprite_enemyWalkUp1_data:


.byte   0,  0,$28,1
.byte   8,  0,$29,1
.byte   0,  8,$38,1
.byte   8,  8,$39,1
.byte $80


metasprite_enemyWalkUp2_data:


.byte   8,  0,$28,1|OAM_FLIP_H
.byte   0,  0,$29,1|OAM_FLIP_H
.byte   8,  8,$38,1|OAM_FLIP_H
.byte   0,  8,$39,1|OAM_FLIP_H
.byte $80


metasprite_enemyWalkDown1_data:


.byte   0,  0,$26,1
.byte   8,  0,$27,1
.byte   0,  8,$36,1
.byte   8,  8,$37,1
.byte $80


metasprite_enemyWalkDown2_data:


.byte   8,  0,$26,1|OAM_FLIP_H
.byte   0,  0,$27,1|OAM_FLIP_H
.byte   8,  8,$36,1|OAM_FLIP_H
.byte   0,  8,$37,1|OAM_FLIP_H
.byte $80


metasprite_enemyWalkLeft1_data:


.byte   0,  0,$2a,1
.byte   8,  0,$2b,1
.byte   0,  8,$3a,1
.byte   8,  8,$3b,1
.byte $80


metasprite_enemyWalkLeft2_data:


.byte   0,  0,$24,1
.byte   8,  0,$25,1
.byte   0,  8,$34,1
.byte   8,  8,$35,1
.byte $80


metasprite_enemyWalkRight1_data:


.byte   8,  0,$2a,1|OAM_FLIP_H
.byte   0,  0,$2b,1|OAM_FLIP_H
.byte   8,  8,$3a,1|OAM_FLIP_H
.byte   0,  8,$3b,1|OAM_FLIP_H
.byte $80


metasprite_enemyWalkRight2_data:


.byte   8,  0,$24,1|OAM_FLIP_H
.byte   0,  0,$25,1|OAM_FLIP_H
.byte   8,  8,$34,1|OAM_FLIP_H
.byte   0,  8,$35,1|OAM_FLIP_H
.byte $80


metasprite_flyerLeft1_data:


.byte   0,  0,$0c,2
.byte   8,  0,$0d,2
.byte   0,  8,$1c,2
.byte   8,  8,$1d,2
.byte $80


metasprite_flyerLeft2_data:


.byte   0,  0,$2c,2
.byte   8,  0,$2d,2
.byte   0,  8,$3c,2
.byte   8,  8,$3d,2
.byte $80


metasprite_flyerRight1_data:


.byte   8,  0,$0c,2|OAM_FLIP_H
.byte   0,  0,$0d,2|OAM_FLIP_H
.byte   8,  8,$1c,2|OAM_FLIP_H
.byte   0,  8,$1d,2|OAM_FLIP_H
.byte $80


metasprite_flyerRight2_data:


.byte   8,  0,$2c,2|OAM_FLIP_H
.byte   0,  0,$2d,2|OAM_FLIP_H
.byte   8,  8,$3c,2|OAM_FLIP_H
.byte   0,  8,$3d,2|OAM_FLIP_H
.byte $80


_metasprite_pointers:

.word metasprite_playerIdleUp_data
.word metasprite_playerIdleDown_data
.word metasprite_playerIdleLeft_data
.word metasprite_playerIdleRight_data
.word metasprite_playerWalkUp1_data
.word metasprite_playerWalkUp2_data
.word metasprite_playerWalkDown1_data
.word metasprite_playerWalkDown2_data
.word metasprite_playerWalkLeft1_data
.word metasprite_playerIdleLeft_data
.word metasprite_playerWalkRight1_data
.word metasprite_playerIdleRight_data
.word metasprite_enemyWalkUp1_data
.word metasprite_enemyWalkUp2_data
.word metasprite_enemyWalkDown1_data
.word metasprite_enemyWalkDown2_data
.word metasprite_enemyWalkLeft1_data
.word metasprite_enemyWalkLeft2_data
.word metasprite_enemyWalkRight1_data
.word metasprite_enemyWalkRight2_data
.word metasprite_flyerLeft1_data
.word metasprite_flyerLeft2_data
.word metasprite_flyerRight1_data
.word metasprite_flyerRight2_data
