.export _zombie_enemy_update
.export _enemy_player_collision

.importzp _i_enemy, _temp_enemy_x, _temp_enemy_y, _temp_x, _temp_y
.importzp _player_x, _player_y
.import _enemy_x, _enemy_y, _enemy_hp, _enemy_speed, _enemy_direction
.import _map

.import _rand8

.enum Metatile
  Null
  Floor
  Wall
  Conditioner
  ForkedFloor
.endenum

.segment "CODE"

.proc _enemy_player_collision
  lda _i_enemy
  asl
  tax

  ; algorithm:
  ; if (l1.x > r2.x || l2.x > r1.x)
  ;      return false;
  ; if (r1.y > l2.y || r2.y > l1.y)
  ;     return false;
  ; return true;
  ;
  ; l1 = enemy + 2, l2 = player + 2
  ; r1 = enemy + 16 - 2, r2 = player + 16 - 2

  ; l1.x > r2.x
  ; enemy.x + 2 > player.x + 16 - 2
  ; enemy.x > player.x + 12
  ; player.x + 12 < enemy.x

  lda _player_x
  clc
  adc #12
  cmp _enemy_x + 1, x
  bcc no_collision

  ; l2.x > r1.x
  ; player.x + 2 > enemy.x + 16 - 2
  ; enemy.x + 14 < player.x + 2
  ; enemy.x + 12 < player.x

  lda _enemy_x + 1, x
  clc
  adc #12
  cmp _player_x
  bcc no_collision

  ; idem for y

  lda _player_y
  clc
  adc #12
  cmp _enemy_y + 1, x
  bcc no_collision

  lda _enemy_y + 1, x
  clc
  adc #12
  cmp _player_y
  bcc no_collision

  ; collision
  lda #1
  ldx #0
  rts

no_collision:
  lda #0
  tax
  rts
.endproc

.proc _zombie_enemy_update
  lda _i_enemy
  asl
  tax

  ; temp_enemy_? = enemy_?[x]
  ; temp_x = (temp_enemy_x + 0x700) >> 8 >> 4
  ; temp_y = (temp_enemy_x + 0x700) >> 8 & 0xf0

  lda _enemy_x, x
  sta _temp_enemy_x
  lda _enemy_x + 1, x
  sta _temp_enemy_x + 1

  clc
  adc #07
  .repeat 4
    lsr
  .endrepeat
  sta _temp_x


  lda _enemy_y, x
  sta _temp_enemy_y
  lda _enemy_y + 1, x
  sta _temp_enemy_y + 1

  clc
  adc #$07
  and #$f0
  sta _temp_y


  lda _temp_y
  ora _temp_x
  tay

  ; map[temp_x, temp_y]

  lda _map, y
  cmp #Metatile::Wall

  bne no_wall

  ; hp = 0, bail out
  lda #0
  ldx _i_enemy
  sta _enemy_hp, x
  rts
no_wall:

  cmp #Metatile::ForkedFloor
  bne no_fork

  ; x near snap?
  lda _temp_enemy_x + 1
  and #$0f
  cmp #$03
  bcs no_fork
  cmp #$0e
  bcc no_fork

  ; y near snap?
  lda _temp_enemy_y + 1
  and #$0f
  cmp #$03
  bcs no_fork
  cmp #$0e
  bcc no_fork

  ; roll a dice
  jsr _rand8
  cmp #$10
  bcs no_fork

  and #$03
  ldx _i_enemy
  sta _enemy_direction, x

  ; snap coordinates
  txa
  asl
  tax

  lda #0
  sta _enemy_x, x

  lda _temp_x
  .repeat 4
    asl
  .endrepeat

  sta _enemy_x + 1, x

  lda #0
  sta _enemy_y + 1, x

  lda _temp_y
  sta _enemy_y, x

  rts

no_fork:

  ; try to move
  ldx _i_enemy
  lda _enemy_direction, x
  tax
  lda zombie_handlers_h, x
  pha
  lda zombie_handlers_l, x
  pha
  rts
.endproc

.proc zombie_up_handler
  ; temp_y = (y - 1.00) & $f0
  lda _temp_enemy_y + 1
  sec
  sbc #1
  and #$f0

  ; check collision
  ora _temp_x
  tay

  lda _map, y
  cmp #Metatile::Wall

  bne move

  ; collision = snap, turn
  jsr _rand8
  and #$03
  ldx _i_enemy
  sta _enemy_direction, x
  txa
  asl
  tax

  ; enemy_y[i_enemy] = enemy_y[i_enemy] & 0xf000;
  lda #$00
  sta _enemy_y, x

  lda _enemy_y + 1, x
  and #$f0
  sta _enemy_y + 1, x

  rts
move:
  lda _i_enemy
  asl
  tax

  ; y -= speed
  lda _enemy_y, x
  sec
  sbc _enemy_speed, x
  sta _enemy_y, x
  lda _enemy_y + 1, x
  sbc _enemy_speed + 1, x
  sta _enemy_y + 1, x

  rts
.endproc

.proc zombie_down_handler
  ; temp_y = (y + 10.00) & $f0
  lda _temp_enemy_y + 1
  clc
  adc #$10
  and #$f0

  ; check collision
  ora _temp_x
  tay

  lda _map, y
  cmp #Metatile::Wall

  bne move

  ; collision = snap, turn
  jsr _rand8
  and #$03
  ldx _i_enemy
  sta _enemy_direction, x
  txa
  asl
  tax

  ; enemy_y[i_enemy] = (enemy_y[i_enemy] + 0x00ff) & 0xf000;
  lda _enemy_y, x
  clc
  adc #$ff
  lda #$00
  sta _enemy_y, x
  adc _enemy_y + 1, x
  and #$f0
  sta _enemy_y + 1, x

  rts
move:
  lda _i_enemy
  asl
  tax

  ; y += speed
  lda _enemy_y, x
  clc
  adc _enemy_speed, x
  sta _enemy_y, x
  lda _enemy_y + 1, x
  adc _enemy_speed + 1, x
  sta _enemy_y + 1, x

  rts
.endproc

.proc zombie_left_handler
  ; temp_x = (x - 1.00) & $f0
  lda _temp_enemy_x + 1
  sec
  sbc #1
  .repeat 4
    lsr
  .endrepeat

  ; check collision
  ora _temp_y
  tay

  lda _map, y
  cmp #Metatile::Wall

  bne move

  ; collision = snap, turn
  jsr _rand8
  and #$03
  ldx _i_enemy
  sta _enemy_direction, x
  txa
  asl
  tax

  ; enemy_x[i_enemy] = enemy_x[i_enemy] & 0xf000;
  lda #$00
  sta _enemy_x, x

  lda _enemy_x + 1, x
  and #$f0
  sta _enemy_x + 1, x

  rts
move:
  lda _i_enemy
  asl
  tax

  ; y -= speed
  lda _enemy_x, x
  sec
  sbc _enemy_speed, x
  sta _enemy_x, x
  lda _enemy_x + 1, x
  sbc _enemy_speed + 1, x
  sta _enemy_x + 1, x

  rts
.endproc

.proc zombie_right_handler
  ; temp_x = (x + 10.00) & $f0
  lda _temp_enemy_x + 1
  clc
  adc #$10
  .repeat 4
    lsr
  .endrepeat

  ; check collision
  ora _temp_y
  tay

  lda _map, y
  cmp #Metatile::Wall

  bne move

  ; collision = snap, turn
  jsr _rand8
  and #$03
  ldx _i_enemy
  sta _enemy_direction, x
  txa
  asl
  tax

  ; enemy_x[i_enemy] = (enemy_x[i_enemy] + 0x00ff) & 0xf000;
  lda _enemy_x, x
  clc
  adc #$ff
  lda #$00
  sta _enemy_x, x
  adc _enemy_x + 1, x
  and #$f0
  sta _enemy_x + 1, x

  rts
move:
  lda _i_enemy
  asl
  tax

  ; y += speed
  lda _enemy_x, x
  clc
  adc _enemy_speed, x
  sta _enemy_x, x
  lda _enemy_x + 1, x
  adc _enemy_speed + 1, x
  sta _enemy_x + 1, x

  rts
.endproc

.segment "RODATA"

zombie_handlers_l:
.byte <(zombie_up_handler-1), <(zombie_down_handler-1), <(zombie_left_handler-1), <(zombie_right_handler-1)

zombie_handlers_h:
.byte >(zombie_up_handler-1), >(zombie_down_handler-1), >(zombie_left_handler-1), >(zombie_right_handler-1)
