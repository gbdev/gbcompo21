; Macro for a tiled floor.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette normal
; - 3: Block ID
; - 4: Palette top
tile_macro_floor_top: MACRO
    
    ;Save base palette in temp variable
    ld a, \2
    ldh [h_temp], a
    
    ;Set base tile offset
    ld e, \1

    ;Save block pointer
    push bc

    ;Check block above (-64)
    ld a, c
    sub a, 64
    ld c, a
    ld a, b
    sbc a, 0
    ld b, a

    ;If block above is NOT floor
    ld a, [bc]
    cp a, \3
    jr z, :+
        
        ;Switch to top tiles
        inc e
        inc e

        ;Switch to other palette
        ld a, \4
        ldh [h_temp], a
    :

    ;Grab tile in top-left
    dec c
    ld a, [bc]
    ldh [h_temp+1], a

    ;Grab tile in top-right
    inc c
    inc c
    ld a, [bc]
    ldh [h_temp+2], a

    ;Restore block pointer
    pop bc

    ;Check block to the left
    dec c

    ;If block to the left is NOT floor
    ld a, [bc]
    cp a, \3
    jr z, :+

        ;Switch to edge sprite
        inc e
    :

    ;Tile is surrounded, check corner
    ld a, e
    cp a, \1
    jr nz, :+

        ;Test if corner tile is floor
        ldh a, [h_temp+1]
        cp a, \3
        jr z, :+

            ;Switch to inverted corner tile
            ld a, e
            add a, 4
            ld e, a
    :




    ;Place tile 1
    ld [hl], e

    ;Place palette 1
    ld a, 1
    ldh [rVBK], a
    ldh a, [h_temp]
    ld [hl+], a

    ;Increment pointers
    inc c
    inc c


    ;Place palette 2
    ld [hl], a

    ;Increment tile ID
    ld a, %11111010
    and a, e
    add a, 8
    ld e, a
    xor a

    ;Check block to the right
    ;If block is NOT floor
    ld a, [bc]
    dec c
    cp a, \3
    jr z, :+

        ;Switch to edge sprite
        inc e
    :

    ;Tile is surrounded, check corner
    ld a, e
    cp a, \1+8
    jr nz, :+

        ;Test if corner tile is floor
        ldh a, [h_temp+2]
        cp a, \3
        jr z, :+

            ;Switch to inverted corner tile
            ld a, e
            add a, 4
            ld e, a
    :

    ;Place tile 2
    xor a
    ldh [rVBK], a
    ld [hl], e

    ;Return
    ret 
ENDM