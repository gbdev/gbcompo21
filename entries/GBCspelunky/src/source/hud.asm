INCLUDE "hardware.inc"

;HUD constants
;No need for a .inc file, since this
;is the only place these are refrences.

;Tile ID's
RSRESET
hud_tile_font rb $10
hud_tile_dollarsign rb $01
hud_tile_dash rb $01
hud_tile_border rb $01
hud_tile_blank rb $01
hud_tile_hearts_test rb $14
hud_tile_heart_blank rb $04
hud_tile_colon rb $01
hud_tile_clock rb $01
hud_tile_level rb $01
hud_tile_border_ext rb $08

;Pointers
hud_pointer equ _SCRN1
hud_pointer_heart equ _SCRN1
hud_pointer_bombs equ _SCRN1 + 3
hud_pointer_ropes equ _SCRN1 + 5
hud_pointer_timer equ _SCRN1 + 8
hud_pointer_level equ _SCRN1 + 16
hud_pointer_money equ _SCRN1 + 32 + 3

SECTION "HUD", ROM0

; Initializes the hud.
hud_init::
    
    ;Switch VRAM bank
    ld a, 1
    ldh [rVBK], a

    ;Copy font to VRAM
    ld hl, $9000
    ld bc, tls_font
    ld de, tls_fonte - tls_font
    call memcopy

    ;Write screen data
    ld hl, hud_pointer
    ld b, OAMF_BANK1 | 3
    ld de, 64
    call memfill
    ld b, OAMF_BANK1 | 1
    ld de, $03C0
    call memfill

    ;Write heart palettes
    ld hl, hud_pointer_heart
    ld a, OAMF_BANK1 | 6 ;Heart palette
    ld [hl+], a
    ld [hl-], a
    set 5, l
    ld [hl+], a
    ld [hl-], a

    ld hl, _SCRN1+32*8+5
    ld b, OAMF_BANK1 | 3
    ld de, 10
    call memfill
    ld hl, _SCRN1+32*9+4
    ld de, 12
    call memfill
    ld hl, _SCRN1+32*10+4
    ld de, 12
    call memfill

    ;Switch VRAM bank back
    xor a
    ldh [rVBK], a

    ;Write blank tile to to the entire HUD
    ld hl, hud_pointer
    ld b, hud_tile_blank
    ld de, 64
    call memfill

    ;Write border at the bottom
    ld b, hud_tile_border
    ld de, 32
    call memfill

    ld b, hud_tile_blank
    ld de, $400 - 96
    call memfill

    ;Write GAME OVER
        ld hl, _SCRN1+32*8+5
        ld [hl], "G"
        inc l
        ld [hl], "A"
        inc l
        ld [hl], "M"
        inc l
        ld [hl], "E"
        inc l
        inc l
        inc l
        ld [hl], "O"
        inc l
        ld [hl], "V"
        inc l
        ld [hl], "E"
        inc l
        ld [hl], "R"
    ;Write PRESS START
        ld hl, _SCRN1+32*9+4
        ld [hl], "P"
        inc l
        ld [hl], "R"
        inc l
        ld [hl], "E"
        inc l
        ld [hl], "S"
        inc l
        ld [hl], "S"
        inc l
        inc l
        inc l
        ld [hl], "S"
        inc l
        ld [hl], "T"
        inc l
        ld [hl], "A"
        inc l
        ld [hl], "R"
        inc l
        ld [hl], "T"
    ;Write TO TRY AGAIN
        ld hl, _SCRN1+32*10+4
        ld [hl], "T"
        inc l
        ld [hl], "O"
        inc l
        inc l
        ld [hl], "T"
        inc l
        ld [hl], "R"
        inc l
        ld [hl], "Y"
        inc l
        inc l
        ld [hl], "A"
        inc l
        ld [hl], "G"
        inc l
        ld [hl], "A"
        inc l
        ld [hl], "I"
        inc l
        ld [hl], "N"
    ;

    ;Set LYC register/interupt
    ld a, 19
    ldh [rLYC], a
    ld hl, rSTAT
    set 6, [hl]

    ;rWX
    ld a, 7
    ldh [rWX], a

    ;Return
    ret
;



; Subroutine deserving of it's own label.
; Used with `hud_update`.
hud_update_money_remaining:
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, c
    and a, %00001111
    .lower
    ld [hl+], a

    ;Continue loop
    inc e
    ld a, e
    cp a, low(w_money + 4)
    ld a, [de]
    ld c, a
    jr nz, hud_update_money_remaining

    ;Done displaying money, fill out blank spaces
    .blankfill
    ld a, l
    cp a, low(hud_pointer_money+9)
    jr z, hud_update.donemoney

    ld [hl], hud_tile_blank
    inc l
    jr .blankfill
;




; Updates the hud.
; Assumes Vblank is active.
;
; Destroys: all maybe
hud_update::

    ;Write dollar sign
    ld hl, hud_pointer_money
    ld [hl], hud_tile_dollarsign
    inc l
    ld b, hud_tile_font

    ;Update money count
        ld de, w_money
        ld a, [de]
        ld c, a
        and a, %11110000
        jr nz, hud_update_money_remaining
        ld a, c
        and a, %00001111
        jr nz, hud_update_money_remaining.lower
        inc e

        ld a, [de]
        ld c, a
        and a, %11110000
        jr nz, hud_update_money_remaining
        ld a, c
        and a, %00001111
        jr nz, hud_update_money_remaining.lower
        inc e

        ld a, [de]
        ld c, a
        and a, %11110000
        jr nz, hud_update_money_remaining
        ld a, c
        and a, %00001111
        jr nz, hud_update_money_remaining.lower
        inc e

        ld a, [de]
        ld c, a
        and a, %11110000
        jr nz, hud_update_money_remaining
        ld a, c
        and a, %00001111
        add a, b
        ld [hl+], a
        jr hud_update_money_remaining.blankfill

    .donemoney

    ;Write health
    ld hl, hud_pointer_heart
    ld a, [w_player_health]
    add a, a
    add a, a
    add a, hud_tile_hearts_test
    ld [hl+], a
    inc a
    ld [hl-], a
    inc a
    set 5, l
    ld [hl+], a
    inc a
    ld [hl-], a

    ;Update timer
    ld hl, w_level_timer
    ld e, l
    dec [hl]
    jr nz, :+
        
        ;Subtract a second
        ld [hl], 60
        inc l
        ld a, [hl]
        dec a
        daa 
        ld [hl], a
        jr nz, :+
            
            ;Subtract a minute
            ld [hl], $60
            inc l
            dec [hl]

            ld a, [hl]
            cp a, $FF
            jr nz, :+

                ;Timer has reached 0
                ;Don't return, jump to gameover
                pop af
                jp gameover
    :
    
    ;Show timer on screen
    ;Clock tile
    ld de, w_level_timer+2
    ld hl, hud_pointer_bombs
    ld [hl], hud_tile_clock
    inc l

    ;Minute
    ld a, [de]
    ld [hl+], a
    dec e
    ld [hl], hud_tile_colon
    inc l

    ;Second
    ld a, [de]
    dec a
    daa 
    ld c, a
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, c
    and a, %00001111
    ld [hl], a

    ;Level counter
    ld hl, hud_pointer_level
    ld [hl], hud_tile_level
    inc l
    ld a, [w_level_number]
    ld c, a
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, c
    and a, %00001111
    ld [hl], a

    ;Return
    ret

    ;Display player XY-coordinates
    ld bc, w_player_x
    ld hl, hud_pointer + 16
    ld a, [bc]
    ld d, a
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, d
    and a, %00001111
    ld [hl+], a
    inc c
    ld a, [bc]
    ld d, a
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, d
    and a, %00001111
    ld [hl+], a
    inc c
    ld hl, hud_pointer + 48
    ld a, [bc]
    ld d, a
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, d
    and a, %00001111
    ld [hl+], a
    inc c
    ld a, [bc]
    ld d, a
    and a, %11110000
    swap a
    ld [hl+], a
    ld a, d
    and a, %00001111
    ld [hl+], a

    ;Return
    ret
;



; Adds money
;
; Input:
; - `bc`: Pointer to lowest byte of number
;
; Destroys: `af`, `bc`
hud_money_add::

    ;Start going
    push hl
    ld hl, w_money + 3

    ;Lowest byte
    ld a, [bc]
    add a, [hl]
    daa 
    ld [hl-], a
    dec bc

    ;Next lowest byte
    ld a, [bc]
    adc a, [hl]
    daa 
    ld [hl-], a
    dec bc

    ;Next highest byte
    ld a, [bc]
    adc a, [hl]
    daa 
    ld [hl-], a
    dec bc

    ;Highest byte
    ld a, [bc]
    adc a, [hl]
    ld [hl], a

    ;Pop and return
    pop hl
    ret 
;



;Font for window layer
tls_font:
    INCBIN "font.tls"
    tls_fonte: