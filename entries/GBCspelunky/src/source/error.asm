INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "color.inc"

SECTION FRAGMENT "error", ROMX, BANK[bank_errorhandler]
errorhandler::
    
    ;Trigger a breakpoint in the emulator
    ld b, b
    
    ;Reset stack pointer
    ld sp, $D000
    
    ;Is LCD already disabled?
    ld hl, rLCDC
    bit 7, [hl]

    ;If yes, skip disabling the LCD
    jr z, :+


    ;Wait for Vblank
    ld hl, rLY
    ld a, 144
    .wait
    cp a, [hl]
    jr nz, .wait

    ;Disable LCD
    xor a
    ld [rLCDC], a
    :

    ;Reset background scrolling
    ld a, $F8
    ldh [rSCY], a
    xor a
    ldh [rSCX], a

    ;Set palettes to black and white
    ld hl, error_palette_bg
    call palette_copy_bg
    xor a
    ld hl, error_palette_obj
    call palette_copy_spr

    ;Clear VRAM
    ld b, 0
    ld de, $2000
    ld hl, $8000
    call memfill

    ;Clear VRAM again
    ld a, 1
    ldh [rVBK], a
    ld b, 0
    ld de, $2000
    ld hl, $8000
    call memfill
    xor a
    ldh [rVBK], a

    ;DMA setup
    call sprite_setup

    ;Clear OAM
    ld hl, w_oam_mirror
    ld b, a
    ld de, $0100
    call memfill
    call h_dma_routine
    
    ;Load graphics into VRAM
    ld hl, $8000
    ld bc, error_tiles
    ld de, error_tiles_e - error_tiles
    call memcopy

    ld hl, $8000 + ($00BE << 4)
    ld bc, error_sprites
    ld de, error_sprites_e -error_sprites
    call memcopy

    ;Load map into VRAM
    ld bc, error_map
    ld hl, $9800
    ld de, 0

    .loop
    ;Copy the data
    ld a, [bc]
    inc bc
    ld [hl+], a

    ;Horizontal counter
    inc d
    ld a, $10
    cp a, d
    jr nz, .loop
    ld d, 0

    ;Horizontal offset
    push bc
    ld bc, $10
    add hl, bc
    pop bc

    ;Vertical offset
    inc e
    ld a, $10
    cp a, e
    jr nz, .loop

    ;Screen scroll
    ld a, -28
    ld [rSCX], a

    ;Paletes
    ld a, %00110011
    ld [rBGP], a
    ld a, $C4
    ld [rOBP0], a

    ;Set sprite things
    ld hl, w_oam_mirror
    ld bc, error_spritedata
    ld de, $100
    call memcopy
    
    ;Update OAM
    call h_dma_routine

    ;Prepare
    ld hl, zero+144

    ;Enable interupts
    ld a, STATF_MODE00
    ld [rSTAT], a
    ld a, 2
    ld [rIE], a
    xor a
    ld [rIF], a

    ;re-enable LCD
    ld a, LCDCF_ON | LCDCF_BG8000 | LCDCF_OBJ16 | LCDCF_OBJON | LCDCF_BGON
    ld [rLCDC], a







    
error_wait:
    ldh a, [rIF]
    xor a, 2
    ldh [rIF], a
    halt

;Stat
int_stat:
    
    ;Write that shit
    ld a, b
    ldh [rSCX], a

    ;Decrement wave pointer
    dec de

    ;Grab final thing
    ld a, [de]
    sub a, $10
    ld b, a



    ;VBLANK CHECK
    ;Check scanline number
    ldh a, [rLY]
    cp a, $8F
    jp nz, error_wait

    ;This is the final scanline, just wait for VBlank
    ld a, 1
    ldh [rIE], a
    halt 





    
    ;VBLANK
    ;Cool and fun input test
    call input

    ;Go to the start of the animation if A is pressed
    bit PADB_A, c
    jr z, :+
    ld hl, grad+40
    :

    ;Go to the end of the animation if B is pressed
    bit PADB_B, c
    jr z, :+
    ld hl, sine+144
    :

    ;Save things on the stack
    push hl

    ;Decrease all 40 sprites Y-position
    ld b, 40
    ld hl, w_oam_mirror
    ld de, $0004
    .loop
    dec [hl]
    add hl, de
    dec b
    jr nz, .loop

    ;Run sprite DMA
    call h_dma_routine

    ;Retrieve sine pointer from stack
    pop hl

    ;Increase sine pointer
    inc hl
    
    ;Decrease sine pointer if too high
    ld a, high(sine)+2
    cp a, h
    jr nz, :+
    dec h
    :

    ;Load sine pointer back into DE
    ld d, h
    ld e, l

    ;Prepare next cycle
    ld a, [de]
    sub a, 31
    ld b, a

    ;Do this for now
    ldh [rSCX], a

    ;Reenable interupts
    xor a
    ldh [rIF], a
    ld a, 2
    ldh [rIE], a
    jp error_wait
;



SECTION FRAGMENT "error", ROMX, ALIGN[8], BANK[bank_errorhandler]

;Just a bunch of 0's
zero:
    REPT 512
    db 0
    ENDR

;Gradual sine curve
grad:
ANGLE = 0.0
MULTR = 0.0
    REPT 2048
ANGLE = ANGLE + 256.0
MULTR = MULTR + DIV(32.0, 2048.0)
    db MUL(MULTR, SIN(ANGLE)) >> 16
    ENDR

;Regular sine curve
sine:
ANGLE = 0.0
    REPT 512
    db MUL(32.0, SIN(ANGLE)) >> 16
ANGLE = ANGLE + 256.0
    ENDR

;Error screen background tileset
error_tiles:
    ;INCBIN "error/tilesEXT.bin"
    INCBIN "errorhandler/petiles.bin"
error_tiles_e:

;Erro screen sprite tiles
error_sprites:
    INCBIN "errorhandler/sprites.bin"
error_sprites_e:

;Error screen map data
error_map:
    ;INCBIN "error/mapEXT.bin"
    INCBIN "errorhandler/pemap.bin"
error_map_e:

;Error screen sprite data
error_spritedata:
    INCBIN "errorhandler/initsprites.bin"

;Error screen background palette
error_palette_bg:
    color_dmg_blk
    color_dmg_wht
    color_dmg_dkg
    color_dmg_blk

;Error screen sprite palette
error_palette_obj:
    color_dmg_wht
    color_dmg_ltg
    color_dmg_wht
    color_dmg_blk

;No free entity to allocate
error_entityoverflow:: 
db $00, $FF, "ENTITY OVERFLOW", $00

;Vblank interupt was called
error_vblank::
db $00, $FF, "VBLANK INTERRUPT ENTERED", $00