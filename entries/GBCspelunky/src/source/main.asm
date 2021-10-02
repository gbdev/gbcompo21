INCLUDE "hardware.inc"
INCLUDE "header.inc"
INCLUDE "camera.inc"
INCLUDE "banks.inc"

SECTION "ERROR HANDLER", ROM0[$0038]

    ;Error handling
    v_error::
    ld a, bank(errorhandler)
    ld [$2000], a
    jp errorhandler
;

SECTION "VBLANK INTERRUPT", ROM0[$0040]
    
    ;No
    ld hl, error_vblank
    rst v_error 
;

SECTION "STAT INTERRUPT", ROM0[$0048]

    ;Disable interupts and push everything
    di 
    push af
    push bc
    push de
    push hl

    ;Move the window out of the way
    xor a
    ldh [rIF], a
    dec a
    ldh [rWX], a
    ld a, IEF_VBLANK
    ldh [rIE], a
    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_WINON | LCDCF_WIN9C00 | LCDCF_OBJON | LCDCF_OBJ16
    ldh [rLCDC], a

    ;Do sound
    call _hUGE_dosound

    ;Pop everything and return
    pop hl
    pop de
    pop bc
    pop af
    ret
;



SECTION "ENTRY POINT", ROM0[$0100]
    
    ;Disable interupts and jump
    di
    jp setup_complete
;



SECTION "MAIN", ROM0[$0150]

;Entrypoint of the program, jumped to after setup is complete
start::

;Endless loop, replace with game code
main:
    
    ld a, [rIE]
    and a, IEF_LCDC
    jr z, :+
    halt
    :
    
    ;Cancel all interupts requests
    halt

    ;Shenanigans
    ld a, 7
    ldh [rWX], a
    xor a
    ldh [rWY], a
    ldh [rIF], a
    ld a, IEF_LCDC
    ldh [rIE], a
    ld hl, rLCDC
    res 1, [hl]
    ei
    
    ;Fetch update enable
    ld a, [w_screen_update_enable]
    bit camb_update, a
    jr z, :+

        ;Should it be updates horizontally or vertically?
        bit camb_vertical, a

        ;Fetch camera update coordinates
        ld hl, w_cam_update_x
        ld a, [hl+]
        ld c, [hl]
        ld b, a

        ;Switch banks around
        ld a, [w_world_bank]
        ld [$2000], a
        ld a, [w_layer_bank]
        ldh [rSVBK], a

        ;Call update functions
        push af
        call z, dwellings_map_update_vertical
        pop af
        call nz, dwellings_map_update_horizontal
        jr .dma
    :

    ;Update blocks from the updatelist instead
    bit camb_update_list, a
    jr z, :+

        ;There are items to be updated in the list
        ld a, [w_world_bank]
        ld [$2000], a
        ld a, [w_layer_bank]
        ldh [rSVBK], a
        call dwellings_map_update_list
        jr .dma
    :

    ;I have spre VRAM-time, update HUD
    call hud_update

    ;Run sprite DMA
    .dma
    call h_dma_routine

    ;Load shadow scroll registers
    ldh a, [h_scx]
    ldh [rSCX], a
    ldh a, [h_scy]
    ldh [rSCY], a
    
    ;Get input
    call input

    ;Reset the entire thing maybe
    ;bit PADB_START, c
    ;jp nz, setup_newlevel

    ;Player code
    call_bank_m0 player

    ;Entity code
    ld a, bank_entities
    ldh [rSVBK], a

    ;Make sure entities are within the screen
    call entsys_oobcheck

    ;Execute entity code
    call w_entsys_execute

    ;Go back to the start
    jp main
;



; Call this to end the game
gameover::

    ;Wait for Vblank
    di 
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ldh [rIE], a
    halt 

    ;Update hud once
    call hud_update

    ;Wait for vblank again
    .wait
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ldh [rIE], a
    halt 

    ;Vblank active
    call input
    bit PADB_START, c
    jr z, .wait

    jp setup_partial
;