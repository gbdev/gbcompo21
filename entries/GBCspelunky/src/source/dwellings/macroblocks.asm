INCLUDE "banks.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "chunktypes.inc"


SECTION "DWELLINGS MACROBLOCKS", ROMX, BANK[bank_dwellings_main], ALIGN[8]

;Macroblocks to be handled BEFORE worldscanning
dwellings_macroblock_lookup::
    dw dwellings_macroblock_chunk_air
    dw dwellings_macroblock_chunk_ground
    dw dwellings_macroblock_invalid
    dw dwellings_macroblock_door
    dw dwellings_macroblock_door
    dw dwellings_macroblock_chunk_door
    .end

REPT 128 - ((.end - dwellings_macroblock_lookup) >> 1)
    dw dwellings_macroblock_invalid
ENDR


; Places an air chunk in the chunk buffer.
;
; Input:
; - `bc`: Pointer to macroblock location + 1
;
; Destroys: all
dwellings_macroblock_chunk_air:
    
    ;Push macroblock location
    push bc

    ;Get segment pointer from lookup struct
    ld hl, dwellings_chunk_struct_lookup + 6
    ld a, [hl+]
    ld h, [hl]
    ld l, a
    inc hl
    inc hl
    inc hl
    inc hl
    inc hl
    inc hl

    ;Read segment metadata
    ld a, [hl+]
    ld b, a         ;Segment count is stored in B
    ld a, [hl+]
    ld e, a         ;Segment size is stored in DE
    ld d, 0
    push hl

    ;Get pointer to the segment data
    inc hl
    inc hl
    ld a, [hl+]
    ld h, [hl]
    ld l, a

    ;Get number from 0 - max segnent ID
    call rng_run_single
    .rngmod
    add a, b
    jr nc, .rngmod

    ;Add size to pointer A amount of times
    inc a
    .rngloop
    dec a
    jr z, :+
    add hl, de
    jr .rngloop
    :

    ;Load segment width and height into DE
    pop de
    ld a, [de]
    ld c, a
    inc de
    ld a, [de]
    ld e, a
    ld d, c

    ;Transfer segment pointer to BC
    ld b, h
    ld c, l

    ;Pop macroblock adress
    pop hl
    dec hl

    .copyloop
        
        ;Copy data
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        inc hl
        inc hl
        inc hl
        inc hl
        inc hl

        ;Check thing
        dec e
        jr nz, .copyloop
    
    
    ;Return

    ;Make sure to go over this tile again
    pop de
    pop hl
    pop bc

    dec hl
    inc b

    push bc
    push hl
    push de

    ;Return
    ret
;



; Places a ground chunk in the chunk buffer.
;
; Input:
; - `bc`: Pointer to macroblock location + 1
;
; Destroys: all
dwellings_macroblock_chunk_ground:
    
    ;Push macroblock location
    push bc

    ;Get segment pointer from lookup struct
    ld hl, dwellings_chunk_struct_lookup + 6
    ld a, [hl+]
    ld h, [hl]
    ld l, a

    ;Read segment metadata
    ld a, [hl+]
    ld b, a         ;Segment count is stored in B
    ld a, [hl+]
    ld e, a         ;Segment size is stored in DE
    ld d, 0
    push hl

    ;Get pointer to the segment data
    inc hl
    inc hl
    ld a, [hl+]
    ld h, [hl]
    ld l, a

    ;Get number from 0 - max segnent ID
    call rng_run_single
    .rngmod
    add a, b
    jr nc, .rngmod

    ;Add size to pointer A amount of times
    inc a
    .rngloop
    dec a
    jr z, :+
    add hl, de
    jr .rngloop
    :

    ;Load segment width and height into DE
    pop de
    ld a, [de]
    ld c, a
    inc de
    ld a, [de]
    ld e, a
    ld d, c

    ;Transfer segment pointer to BC
    ld b, h
    ld c, l

    ;Pop macroblock adress
    pop hl
    dec hl

    .copyloop
        
        ;Copy data
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        inc hl
        inc hl
        inc hl
        inc hl
        inc hl

        ;Check thing
        dec e
        jr nz, .copyloop
    
    ;Make sure to go over this tile again
    pop de
    pop hl
    pop bc

    dec hl
    inc b

    push bc
    push hl
    push de
    
    ;Return
    ret
;



; Places a door chunk in the chunk buffer.
;
; Input:
; - `bc`: Pointer to macroblock location + 1
;
; Destroys: all
dwellings_macroblock_chunk_door:
    
    ;Push macroblock location
    push bc

    ;Get segment pointer from lookup struct
    ld hl, dwellings_chunk_struct_lookup + 6
    ld a, [hl+]
    ld h, [hl]
    ld l, a
    ld de, 12
    add hl, de

    ;Read segment metadata
    ld a, [hl+]
    ld b, a         ;Segment count is stored in B
    ld a, [hl+]
    ld e, a         ;Segment size is stored in DE
    ld d, 0
    push hl

    ;Get pointer to the segment data
    inc hl
    inc hl
    ld a, [hl+]
    ld h, [hl]
    ld l, a

    ;Get number from 0 - max segnent ID
    call rng_run_single
    .rngmod
    add a, b
    jr nc, .rngmod

    ;Add size to pointer A amount of times
    inc a
    .rngloop
    dec a
    jr z, :+
    add hl, de
    jr .rngloop
    :

    ;Load segment width and height into DE
    pop de
    ld a, [de]
    ld c, a
    inc de
    ld a, [de]
    ld e, a
    ld d, c

    ;Transfer segment pointer to BC
    ld b, h
    ld c, l

    ;Pop macroblock adress
    pop hl
    dec hl

    .copyloop
        
        ;Copy data
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        ld a, [bc]
        inc bc
        ld [hl+], a
        inc hl
        inc hl
        inc hl
        inc hl

        ;Check thing
        dec e
        jr nz, .copyloop
    
    ;Make sure to go over this tile again
    pop de
    pop hl
    pop bc

    dec hl
    inc b

    push bc
    push hl
    push de
    
    ;Return
    ret
;



; Places a door.
;
; Input:
; - `bc`: Pointer to macroblock location + 1
;
; Destroys: all
dwellings_macroblock_door:
    
    ;Get middle type in D
    ld a, [w_chunk_id]
    bit ctb_entrance, a
    ld d, b_door_middle_exit
    jr z, :+
        
        ;This is an entrance door
        ld d, b_door_middle_entrance

        ;Convert block pointer to X/Y coordinates
        push bc
        push de
        ld b, 0
        ld d, b

        ;Yea
        dec c
        ld a, c
        sub a, low(w_chunk_buffer)

        ;Seperate X and Y positions
        .subloop
            sub a, 10
            inc b
            jr nc, .subloop
        add a, 10
        dec b
        ld c, a

        ;Load chunk x position
        ld a, [w_chunk_x]
        add a, a
        ld d, a
        add a, a
        add a, a
        add a, d
        add a, c
        add a, 2
        ld [w_level_entrance_x], a

        ;Load chunk Y position
        ld a, [w_chunk_y]
        add a, a
        add a, a
        add a, a
        add a, b
        add a, 2
        ld [w_level_entrance_y], a
        pop de
        pop bc
    :

    ;Start writing
    ld h, b
    ld l, c

    ;Write lower blocks
    ld [hl], b_door_right
    dec hl
    ld [hl], d
    dec hl
    ld [hl], b_door_left

    ;Switch to upper blocks
    ld a, l
    sub a, 10
    ld l, a
    jr nc, :+
    dec h
    :

    ;Write upper blocks
    ld [hl], b_door_left
    inc hl
    ld [hl], d
    inc hl
    ld [hl], b_door_right

    ;Return
    ret
;



;Should not be called. If it is, something has gone wrong.
dwellings_macroblock_invalid:
    
    ;Trigger a debug and return
    ;ld b, b
    ret 