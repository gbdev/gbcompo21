INCLUDE "hardware.inc"

; Macro for a tiled ground.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette normal
; - 3: Block ID
; - 4: Special bottom palette
tm_ground: MACRO
    
    ;Initialize C
    ld c, \1+8
    
    ;Load top palettes
    ld a, 1
    ldh [rVBK], a
    ld a, \2
    ld [hl+], a
    ld [hl-], a
    set 5, l

    ;Check block below (+64)
    push de
    ld a, e
    add a, 64
    ld e, a
    jr nc, :+
        inc d
    :

    ;Load bottom palette
    ld a, [de]
    cp a, \3
    ld a, \2
    jr z, :+
        ld a, \4
        inc c
        inc c
    :
    
    ;Write bottom palette
    ld [hl+], a
    ld [hl-], a


    ;Switch VRAM bank
    xor a
    ldh [rVBK], a

    ;Write tiles 3+4
    ld a, c
    ld [hl+], a
    add a, 4
    ld [hl-], a

    ;Switch tile pointer to top tiles
    res 5, l



    ;Move pointer to top tile (-128)
    ld a, e
    sub a, 128
    ld e, a
    jr nc, :+
        dec d
    :

    ;Check if tile is ground
    ld a, [de]
    cp a, \3
    ld a, \1
    jr z, :+
        
        ;Switch to top tile
        add a, 2
    :

    ;Place top tiles
    ld [hl+], a
    add a, 4
    ld [hl-], a

    ;Test block to the left
    pop de
    dec e
    ld a, [de]
    cp a, \3
    jr z, :+
        
        ;Switch to edge tiles
        inc [hl]
        set 5, l
        inc [hl]
        res 5, l
    :

    ;Test block to the right
    inc l
    inc e
    inc e
    ld a, [de]
    cp a, \3
    jr z, :+
        
        ;Switch to edge tiles
        inc [hl]
        set 5, l
        inc [hl]
        res 5, l
    :

    ;Re-center block pointer
    dec e
ENDM



; Macro for a tiled ground.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette normal
; - 3: Block ID
; - 4: Palette top
tile_macro_ground_top: MACRO
    
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

    ;If block above is NOT ground
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

    ;Restore block pointer
    pop bc

    ;Check block to the left
    dec c

    ;If block to the left is NOT ground
    ld a, [bc]
    cp a, \3
    jr z, :+

        ;Switch to edge sprite
        inc e
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
    ld a, %11111110
    and a, e
    add a, 4
    ld e, a
    xor a

    ;Check block to the right
    ;If block is NOT ground
    ld a, [bc]
    dec c
    cp a, \3
    jr z, :+

        ;Switch to edge sprite
        inc e
    :

    ;Place tile 2
    xor a
    ldh [rVBK], a
    ld [hl], e

    ;Return
    ret
ENDM

; Macro for a tiled ground.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette normal
; - 3: Block ID
; - 4: Palette bottom
tile_macro_ground_bottom: MACRO
    
    ;Save base palette in temp variable
    ld a, \2
    ldh [h_temp], a
    
    ;Set base tile offset
    ld e, \1

    ;Save block pointer
    push bc

    ;Check block below (+64)
    ld a, c
    add a, 64
    ld c, a
    ld a, b
    adc a, 0
    ld b, a

    ;If block below is NOT ground
    ld a, [bc]
    cp a, \3
    jr z, :+
        
        ;Switch to bottom tiles
        inc e
        inc e

        ;Switch to other palette
        ld a, \4
        ldh [h_temp], a
    :

    ;Restore block pointer
    pop bc

    ;Check block to the left
    dec c

    ;If block to the left is NOT ground
    ld a, [bc]
    cp a, \3
    jr z, :+

        ;Switch to edge sprite
        inc e
    :



    ;Place tile 1
    ld [hl], e

    ;Place palette 1
    ld a, 1
    ldh [rVBK], a
    ld a, [h_temp]
    ld [hl+], a

    ;Increment pointers
    inc c
    inc c


    ;Place palette 2
    ld [hl], a

    ;Increment tile ID
    ld a, %11111110
    and a, e
    add a, 4
    ld e, a
    xor a

    ;Check block to the right
    ;If block is NOT ground
    ld a, [bc]
    dec c
    cp a, \3
    jr z, :+

        ;Switch to edge sprite
        inc e
    :

    ;Place tile 2
    xor a
    ldh [rVBK], a
    ld [hl], e

    ;Return
    ret
ENDM