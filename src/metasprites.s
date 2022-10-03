.feature force_range ; allows negative values

.segment "RODATA"

.define OAM_BEHIND $20
.define OAM_FLIP_H $40
.define OAM_FLIP_V $80

.export _metasprite_pointers

metasprite_playerIdleUp_data:


.byte   0,  0,$01,0
.byte   8,  0,$03,0
.byte $80


metasprite_playerIdleDown_data:


.byte   0,  0,$05,0
.byte   8,  0,$07,0
.byte $80


metasprite_playerIdleLeft_data:


.byte   0,  0,$09,0
.byte   8,  0,$0b,0
.byte $80


metasprite_playerIdleRight_data:


.byte   8,  0,$09,0|OAM_FLIP_H
.byte   0,  0,$0b,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkUp1_data:


.byte   0,  0,$0d,0
.byte   8,  0,$0f,0
.byte $80


metasprite_playerWalkUp2_data:


.byte   8,  0,$0d,0|OAM_FLIP_H
.byte   0,  0,$0f,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkDown1_data:


.byte   0,  0,$11,0
.byte   8,  0,$13,0
.byte $80


metasprite_playerWalkDown2_data:


.byte   8,  0,$11,0|OAM_FLIP_H
.byte   0,  0,$13,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkLeft1_data:


.byte   0,  0,$15,0
.byte   8,  0,$17,0
.byte $80


metasprite_playerWalkLeft2_data:


.byte   0,  0,$09,0
.byte   8,  0,$0b,0
.byte $80


metasprite_playerWalkRight1_data:


.byte   8,  0,$15,0|OAM_FLIP_H
.byte   0,  0,$17,0|OAM_FLIP_H
.byte $80


metasprite_playerWalkRight2_data:


.byte   8,  0,$09,0|OAM_FLIP_H
.byte   0,  0,$0b,0|OAM_FLIP_H
.byte $80


metasprite_enemyWalkUp1_data:


.byte   0,  0,$19,1
.byte   8,  0,$1b,1
.byte $80


metasprite_enemyWalkUp2_data:


.byte   8,  0,$19,1|OAM_FLIP_H
.byte   0,  0,$1b,1|OAM_FLIP_H
.byte $80


metasprite_enemyWalkDown1_data:


.byte   0,  0,$1d,1
.byte   8,  0,$1f,1
.byte $80


metasprite_enemyWalkDown2_data:


.byte   8,  0,$1d,1|OAM_FLIP_H
.byte   0,  0,$1f,1|OAM_FLIP_H
.byte $80


metasprite_enemyWalkLeft1_data:


.byte   0,  0,$21,1
.byte   8,  0,$23,1
.byte $80


metasprite_enemyWalkLeft2_data:


.byte   0,  0,$25,1
.byte   8,  0,$27,1
.byte $80


metasprite_enemyWalkRight1_data:


.byte   8,  0,$21,1|OAM_FLIP_H
.byte   0,  0,$23,1|OAM_FLIP_H
.byte $80


metasprite_enemyWalkRight2_data:


.byte   8,  0,$25,1|OAM_FLIP_H
.byte   0,  0,$27,1|OAM_FLIP_H
.byte $80


metasprite_flyerLeft1_data:


.byte   0,  0,$29,2
.byte   8,  0,$2b,2
.byte $80


metasprite_flyerLeft2_data:


.byte   0,  0,$2d,2
.byte   8,  0,$2f,2
.byte $80


metasprite_flyerRight1_data:


.byte   8,  0,$29,2|OAM_FLIP_H
.byte   0,  0,$2b,2|OAM_FLIP_H
.byte $80


metasprite_flyerRight2_data:


.byte   8,  0,$2d,2|OAM_FLIP_H
.byte   0,  0,$2f,2|OAM_FLIP_H
.byte $80


metasprite_swordUp_data:


.byte   7,-10,$35,3
.byte $80


metasprite_swordDown_data:


.byte - 1, 11,$35,3|OAM_FLIP_V
.byte $80


metasprite_swordLeft_data:


.byte -12,  6,$39,3
.byte - 4,  6,$3b,3
.byte $80


metasprite_swordRight_data:


.byte  22,  6,$39,3|OAM_FLIP_H
.byte  14,  6,$3b,3|OAM_FLIP_H
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
.word metasprite_playerWalkLeft2_data
.word metasprite_playerWalkRight1_data
.word metasprite_playerWalkRight2_data
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
.word metasprite_swordUp_data
.word metasprite_swordDown_data
.word metasprite_swordLeft_data
.word metasprite_swordRight_data
