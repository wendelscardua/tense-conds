; Startup code for cc65 and Shiru's NES library
; based on code by Groepaz/Hitmen <groepaz@gmx.net>, Ullrich von Bassewitz <uz@cc65.org>

;REMOVED initlib
;this called the CONDES function

.export _exit,__STARTUP__:absolute=1
.import push0,popa,popax,_main,zerobss,copydata

; Linker generated symbols
.import __STACK_START__ ,__STACKSIZE__ ;changed
.import __ROM0_START__ ,__ROM0_SIZE__
.import __STARTUP_LOAD__,__STARTUP_RUN__,__STARTUP_SIZE__
.import __CODE_LOAD__ ,__CODE_RUN__ ,__CODE_SIZE__
.import __RODATA_LOAD__ ,__RODATA_RUN__ ,__RODATA_SIZE__
.import NES_MAPPER, NES_PRG_BANKS, NES_CHR_BANKS, NES_MIRRORING

.include "lib/constants.inc"
.include "lib/neslib.inc"
.include "lib/nesdoug.inc"
.include "zeropage.inc"
.include "ggsound/ggsound.inc"
.include "lib/unrom.inc"

.segment "HEADER"

.byte $4e,$45,$53,$1a
.byte <NES_PRG_BANKS
.byte <NES_CHR_BANKS
.byte <NES_MIRRORING|(<NES_MAPPER<<4)
.byte <NES_MAPPER&$f0
.res 8,0

.segment "STARTUP"

start:
_exit:

sei
cld
ldx #$40
stx CTRL_PORT2
ldx #$ff
txs
inx
stx PPU_MASK
stx DMC_FREQ
stx PPU_CTRL ;no NMI

initPPU:
bit PPU_STATUS
@1:
bit PPU_STATUS
bpl @1
@2:
bit PPU_STATUS
bpl @2

clearPalette:
lda #$3f
sta PPU_ADDR
stx PPU_ADDR
lda #$0f
ldx #$20
@1:
sta PPU_DATA
dex
bne @1

clearVRAM:
txa
ldy #$20
sty PPU_ADDR
sta PPU_ADDR
ldy #$10
@1:
sta PPU_DATA
inx
bne @1
dey
bne @1

clearRAM:
txa
@1:
sta $000,x
sta $100,x
sta $200,x
sta $300,x
sta $400,x
sta $500,x
sta $600,x
sta $700,x
inx
bne @1

lda #4
jsr _pal_bright
jsr _pal_clear
jsr _oam_clear

jsr zerobss
jsr copydata

lda #<(__STACK_START__+__STACKSIZE__) ;changed
sta sp
lda #>(__STACK_START__+__STACKSIZE__)
sta sp+1 ; Set argument stack ptr

; jsr initlib
; removed. this called the CONDES function

lda #%10000000
sta <PPU_CTRL_VAR
sta PPU_CTRL ;enable NMI
lda #%00000110
sta <PPU_MASK_VAR

waitSync3:
lda <FRAME_CNT1
@1:
cmp <FRAME_CNT1
beq @1

detectNTSC:
ldx #52 ;blargg's code
ldy #24
@1:
dex
bne @1
dey
bne @1

lda PPU_STATUS
and #$80
sta <NTSC_MODE

jsr _ppu_off

lda #0
ldx #0
jsr _set_vram_update

lda #$fd
sta <RAND_SEED
sta <RAND_SEED+1

lda #0
sta PPU_SCROLL
sta PPU_SCROLL

; ensure first bank is selected
lda #0
jsr _bankswitch

.include "music/soundtrack.inc"

; ggsound init
lda #<.bank(sound_initialize)
jsr _bankswitch_no_save

lda #SOUND_REGION_NTSC
sta sound_param_byte_0
lda #<song_list
sta sound_param_word_0
lda #>song_list
sta sound_param_word_0+1
lda #<sfx_list
sta sound_param_word_1
lda #>sfx_list
sta sound_param_word_1+1
lda #<instrument_list
sta sound_param_word_2
lda #>instrument_list
sta sound_param_word_2+1
.ifdef DPCM_FEATURE
  lda #<dpcm_list
  sta sound_param_word_3
  lda #>dpcm_list
  sta sound_param_word_3+1
.endif

jsr sound_initialize

jsr _restore_bank

jmp _main ;no parameters

.segment "VECTORS"

.word nmi ;$fffa vblank nmi
.word start ;$fffc reset
.word irq ;$fffe irq / brk
