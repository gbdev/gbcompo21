INCLUDE "hardware.inc"

SECTION "DEBUGGING", ROMX

; Toggle window mode.
;
; Destroys: all
debug_window_toggle::

    ;Check if select was pressed
    ;Return if select isn'r pressed
    ldh a, [h_input_pressed]
    bit PADB_SELECT, a
    ret z

    ;Get new window state
    ld a, [w_debug_window]
    inc a
    cp a, 3
    jr c, :+
    xor a
    :

    ;Write this value
    ld [w_debug_window], a

    ;Return
    ret
;



; Draws things to the window depending on what's in the `w_debug_window` register.
; Assumes Vblank is active.
; Uses tiles 01:$10 - 01:$20 on tilemap $9C00.
;
; Destroys: all
debug_window_run::

    ;Check Vblank status
    ldh a, [rLY]
    cp a, $97
    ret nc
    
    ;Grab value
    ld a, [w_debug_window]

    ;Check which action to use
    cp a, 0 ;Disable window
    jp z, debug_window_none
    cp a, 1 ;Display path
    jp z, debug_window_path
    cp a, 2 ;Display player coordinates
    jp z, debug_window_coordinates

    ;Just in case anything goes wrong, return
    ret
;



; Disables the window entirely.
debug_window_none:
    
    ;Put the window off the screen
    ld a, $EE
    ldh [rWX], a
    ldh [rWY], a

    ;Return
    ret 
;



; Show level path on window layer.
; Uses tiles 01:$10 - 01:$20 on tilemap $9C00.
debug_window_path:
    
    ;Move window into position
    ld a, 160 - $28
    ldh [rWX], a
    ld a, 144 - $30
    ldh [rWY], a
    
    ;Prepare to copy
    ld hl, $9C00
    ld de, w_chunk_type
    ld bc, $0606
    
    ;Copy loop
    .loop

        ;Copy the thing itself
        ld a, [de]
        and a, $0F ;Ignore top 4 bits
        add a, 16 ;Add tile offset
        inc de
        ld [hl+], a
        dec c
        jr nz, .loop

        ;End of row
        ld a, l
        add a, 32 - 6
        ld l, a
        jr nc, :+
        inc h
        :

        ;End of column
        dec b
        ld c, 6
        jr nz, .loop

    ;End of loop, return
    ret 
;



; Shows the player's coordinates.
; Uses tiles 01:$10 - 01:$20 on tilemap $9C00.
debug_window_coordinates::
    
    ;Move window into position
    ld a, 160 - $18
    ldh [rWX], a
    ld a, 144 - $10
    ldh [rWY], a

    ;Display value
    ld hl, $9C00
    ld de, w_player_x

    ;High X
    ld a, [de]
    swap a
    and a, %00001111
    add a, $10
    ld [hl+], a
    ld a, [de]
    and a, %00001111
    add a, $10
    ld [hl+], a
    inc e

    ;Low X
    ld a, [de]
    swap a
    and a, %00001111
    add a, $10
    ld [hl+], a
    ld a, [de]
    and a, %00001111
    add a, $10
    ld [hl+], a
    inc e

    ld hl, $9C20
    ;High Y
    ld a, [de]
    swap a
    and a, %00001111
    add a, $10
    ld [hl+], a
    ld a, [de]
    and a, %00001111
    add a, $10
    ld [hl+], a
    inc e

    ;Low Y
    ld a, [de]
    swap a
    and a, %00001111
    add a, $10
    ld [hl+], a
    ld a, [de]
    and a, %00001111
    add a, $10
    ld [hl+], a

    ;Return
    ret
;