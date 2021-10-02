INCLUDE "hardware.inc"
INCLUDE "player.inc"
INCLUDE "banks.inc" 

SECTION "PLAYER ANIMATION", ROMX, BANK[bank_player]

; Player is walking
player_animate_walk::
    
    ;Push these
    push af
    push bc
    push hl

    ;Cycle animation
    ld hl, w_player_animation
    inc [hl]

    ;Get frame
    ld a, [hl]
    and a, %00011100
    rra 
    add a, panim_walk*2
    ld b, a
    swap b
    xor a
    rl b
    rla
    add a, high(player_sprite)

    ;Transfer this frame through HDMA
    ld hl, rHDMA1
    ld [hl+], a
    ld a, b
    ld [hl+], a
    ld a, $80
    ld [hl+], a
    xor a
    ld [hl+], a

    ;Start HDMA, copy 4 tiles
    ld [hl], 3

    ;Cleanup and return
    pop hl
    pop bc
    pop af
    ret 
;

; Player is idle
player_animate_idle::

    ;Reset animation timer
    push af
    push hl
    xor a
    ld [w_player_animation], a
    
    ;Transfer idle frame through HDMA
    ld hl, rHDMA1
    ld a, high(player_sprite)
    ld [hl+], a
    ld a, panim_idle
    ld [hl+], a
    ld a, $80
    ld [hl+], a
    xor a
    ld [hl+], a

    ;Start HDMA, copy 4 tiles
    ld [hl], 3

    ;Cleanup and return
    pop hl
    pop af
    ret 
;

; Player is jumping
player_animate_jump::
    
    ;Push things
    push af
    push hl

    ;Check player movement direction
    ld hl, rHDMA1
    ld a, [w_player_direction]
    bit player_going_up, a
    jr z, .down
    .up
        ;Going up
        ld a, high(player_sprite) + high(panima_jump_begin)
        ld [hl+], a
        ld a, low(panima_jump_begin)
        ld [hl+], a
        jr .common
    
    .down

        ;Going down
        ;Make sure speed is above a certain threshold
        ld a, [w_player_vspp]
        cp a, $08
        jr c, .up

        ;Write DMA data
        ld a, high(player_sprite) + high(panima_jump_end)
        ld [hl+], a
        ld a, low(panima_jump_end)
        ld [hl+], a

    .common
    ;Set up the rest of the transfer
    ld a, $80
    ld [hl+], a
    xor a
    ld [hl+], a

    ;Start HDMA, copy 4 tiles
    ld [hl], 3

    ;Cleanup and return
    pop hl
    pop af
    ret 
;

; Player is hanging from a ledge
player_animate_hang::

    ;Push stuff
    push af
    push hl
    
    ;Transfer idle frame through HDMA
    ld hl, rHDMA1
    ld a, high(player_sprite + panima_hanging)
    ld [hl+], a
    ld a, low(panima_hanging)
    ld [hl+], a
    ld a, $80
    ld [hl+], a
    xor a
    ld [hl+], a

    ;Start HDMA, copy 4 tiles
    ld [hl], 3

    ;Cleanup and return
    pop hl
    pop af
    ret 
;

;Player is climbing a ladder
player_animate_ladder::

    ;Check vspeed
    ld hl, w_player_vspp
    ld a, [hl-]
    cp a, 0
    jr nz, :+

        ;Play single frame
        ld b, panim_climb_idle
        jp player_animate_frame
    :

    ;Grab direction
    dec l
    ld d, [hl]
    
    ;Cycle animation
    ld l, low(w_player_animation)
    bit player_going_up, d
    jr z, :+
        inc [hl]
        ld a, [hl]
        and a, %00011100
        cp a, 6 << 2
        jr c, :++
            xor a
            ld [hl], a
            jr :++
    :
        dec [hl]
        ld a, [hl]
        and a, %00011100
        cp a, 6 << 2
        jr c, :+
            ld a, (5 << 2)
            ld [hl], (5 << 2) + %11
    :

    ;Get frame
    add a, panim_climb << 2
    ld l, a
    ld h, 0
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    ld a, h
    add a, high(player_sprite)
    ld b, l

    ;Transfer this frame through HDMA
    ld hl, rHDMA1
    ld [hl+], a
    ld a, b
    ld [hl+], a
    ld a, $80
    ld [hl+], a
    xor a
    ld [hl+], a

    ;Start HDMA, copy 4 tiles
    ld [hl], 3

    ;Return
    ret 
;


; Displays a specific frame
; 
; Input:
; `b`: player sprite ID (panim_X)
player_animate_frame::
    
    ;Push stuff
    push af
    push hl

    ;Rotate things
    ld l, b
    ld h, 0
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    ld a, l
    add a, low(player_sprite)
    ld b, a
    ld a, h
    adc a, high(player_sprite)
    
    ;Transfer idle frame through HDMA
    ld hl, rHDMA1
    ld [hl+], a
    ld a, b
    ld [hl+], a
    ld a, $80
    ld [hl+], a
    xor a
    ld [hl+], a

    ;Start HDMA, copy 4 tiles
    ld [hl], 3

    ;Cleanup and return
    pop hl
    pop af
    ret 
;



;Player graphics
SECTION "PLAYER SPRITES", ROMX, BANK[bank_player], ALIGN[8]

;Roffy sprites
player_sprite::
    INCBIN "player/roffy.tls"
    .end

;Whip sprites
player_sprite_whip::
    INCBIN "player/whip.tls"
    .end::

;Player palletes
player_palette::
    INCBIN "player/roffy.pal"

