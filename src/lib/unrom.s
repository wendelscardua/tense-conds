.segment "ZEROPAGE"

current_bank: .res 1

.segment "STARTUP"

bank_identity:
.repeat 3, bank
  .byte bank
.endrepeat

; void bankswitch(unsigned char bank)
.export _bankswitch
.proc _bankswitch
  tay
  sty current_bank
  sta bank_identity, y
  rts
.endproc

; void bankswitch_no_save(unsigned char bank)
.export _bankswitch_no_save
.proc _bankswitch_no_save
  tay
  sta bank_identity, y
  rts
.endproc

; void restore_bank()
.export _restore_bank
.proc _restore_bank
  lda current_bank
  tay
  sta bank_identity, y
  rts
.endproc

.export _farcall
.proc _farcall

  .import callptr4
  .importzp tmp4
  .import pusha, popa

  ldy current_bank

  ; don't switch to the same bank
  cpy tmp4
  bne :+

  jmp callptr4
:

  pha ; save argument to NES stack

  ; save current bank to C stack
  tya
  jsr pusha

  ; select the new bank
  lda tmp4
  jsr _bankswitch

  ;; jump to wrapped call

  pla ; restore C argument

  jsr callptr4

  ; save retval to NES stack
  pha

  ;; pop and restore the previous prg bank

  jsr popa
  jsr _bankswitch

  ; restore A
  pla

  rts
.endproc
