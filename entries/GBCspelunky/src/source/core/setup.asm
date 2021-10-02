INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "player.inc"

SECTION "SETUP", ROM0

;Supposed to run first thing when the game starts.
setup_complete::
    
    ;Set h_is_color variable
    ld b, 0 ;Screw GBA enhanched mode, not doin' it
    cp a, $11
    jr nz, .nocolor
    ld a, [$0143] ;GBC enable flag
    bit 7, a
    jr z, .nocolor
    ld b, $FF
    .nocolor
    ld a, b
    ldh [h_is_color], a

    ;DMG detected, jump to error handler
    ld b, bank(errorhandler)
    ld hl, errorhandler
    cp a, 0
    call z, bank_call_0

    ;Set setup variable to 0
    ld a, 1
    ldh [h_setup], a

    ;Reset stack and play the intro
    ld sp, w_stack
    call intro


;Same as `setup_complete`, but skips checking the GBC enable flag.
setup_partial::
    
    ;Wait for Vblank
    di
    ld a, IEF_VBLANK
    ldh [rIE], a
    xor a
    ldh [rIF], a
    halt 

    ;Disable LCD
    ld hl, rLCDC
    res 7, [hl]

    ;Place stack pointer at the end of WRAM0
    ld sp, w_stack

    ;Check if RNG seed should be saved
    ldh a, [h_setup]
    cp a, 0
    push af
    jr z, .rngskip

        ;Save RNG values to stack
        ld hl, h_rng
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        ld c, a
        ld a, [hl+]
        ld d, a
        ld e, [hl]

        ;Stack shuffling
        pop af
        push bc
        push de
        push af
    .rngskip

    ;Setup ALL variables
    call_bank_m0 variables_init

    ;Put RNG seed back
    pop af
    jr z, .rngignore
        
        ;Retrieve RNG values from stack
        pop de
        pop bc
        ld hl, h_rng
        ld a, b
        ld [hl+], a
        ld a, c
        ld [hl+], a
        ld a, d
        ld [hl+], a
        ld [hl], e

    .rngignore

    ;Set CPU speed to double
    ld hl, rKEY1
    bit 7, [hl]
    jr nz, :+

        ;Double CPU speed
        ldh a, [rIE]
        ld d, a
        xor a
        ldh [rIE], a
        inc a
        ldh [rKEY1], a
        ld a, $30
        ldh [rP1], a
        stop 
        ld a, d
        ldh [rIE], a
    :

    ;Clear background
    xor a
    ldh [rVBK], a
    ld hl, $9800
    ld b, $00
    ld de, $0800
    call memfill

    ;Initialize hud
    call hud_init
    call hud_update

    ;Switch to the dwellings bank
    ld a, bank_dwellings_main
    ld [$2000], a
    call dwellings_load

    ;Create level
    call level_path_create
    call dwellings_map_generate

    ;Initialize player
    call_bank_m0 player_init

    ;Load camera position
    ld hl, w_player_x
    ld a, [hl+]
    ld b, a
    ld a, [hl+]
    ld c, a
    ld a, [hl+]
    ld d, a
    ld a, [hl+]
    ld e, a
    call camera_set_position
    ld c, d
    
    ;Copy the level to VRAM
    ld a, bank_dwellings_main
    ld [$2000], a
    ld d, $0D
    .maptomem
    push bc
    push de
    call dwellings_map_update_vertical
    pop de
    pop bc
    inc b
    dec d
    jr nz, .maptomem

    ;Enable audio
    ld a, $80
    ld [rAUDENA], a
    ld a, $FF
    ld [rAUDTERM], a
    ld a, $77
    ld [rAUDVOL], a

    ;Initialize hUGE
    ld hl, dwellings_bgm
    call hUGE_init

    ;Reenable LCD
    ld hl, rLCDC
    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_WINON | LCDCF_WIN9C00 | LCDCF_OBJON | LCDCF_OBJ16 ;Additional flags here, enable what you need :)
    ld [hl], a

    ;Set layer bank variable
    ld a, bank(level_foreground)
    ld [w_layer_bank], a

    ;Jump to main
    jp start
;



; Partially a setup function.
; Run when going to a new level.
setup_newlevel::

    ;Wait for Vblank
    di
    ld a, IEF_VBLANK
    ldh [rIE], a
    xor a
    ldh [rIF], a
    halt 

    ;Disable LCD
    ld hl, rLCDC
    res 7, [hl]

    ;Place stack pointer at the end of WRAM0
    ld sp, w_stack

    ;Clear entities and sprites
    xor a
    ld b, 40
    call sprite_free
    call entsys_clear

    ;

    ;Switch to the dwellings bank
    ld a, bank_dwellings_main
    ld [$2000], a
    ldh [h_bank_number], a
    call dwellings_load

    call hud_init

    ;Create level
    call level_path_create
    call dwellings_map_generate

    ;Initialize player
    call_bank_m0 player_init

    ;Load camera position
    ld hl, w_player_x
    ld a, [hl+]
    ld b, a
    ld a, [hl+]
    ld c, a
    ld a, [hl+]
    ld d, a
    ld a, [hl+]
    ld e, a
    call camera_set_position
    ld c, d
    
    ;Copy the level to VRAM
    ld a, bank_dwellings_main
    ld [$2000], a
    ld d, $0D
    .maptomem
    push bc
    push de
    call dwellings_map_update_vertical
    pop de
    pop bc
    inc b
    dec d
    jr nz, .maptomem

    ;Increment level number
    ld a, [w_level_number]
    inc a
    daa 
    ld [w_level_number], a

    ;Reset timer
    ld hl, w_level_timer
    ld [hl], $3C
    inc l
    ld [hl], $30
    inc l
    ld [hl], $01
    call hud_update

    ;Set player invincibility
    ld a, player_invincible_time
    ld [w_player_invincible], a

    ;Reenable LCD
    ld hl, rLCDC
    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_WINON | LCDCF_WIN9C00 | LCDCF_OBJON | LCDCF_OBJ16 ;Additional flags here, enable what you need :)
    ld [hl], a

    ;Set layer bank variable
    ld a, bank(level_foreground)
    ld [w_layer_bank], a

    jp start
;