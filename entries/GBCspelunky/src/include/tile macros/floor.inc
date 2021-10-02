
; Macro for a tiled floor.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette
; - 3: Block ID
tm_floor_experimental: MACRO
    
    ;Palettes               =21
    ;Switch VRAM bank
    ld a, 1                 ;2
    ldh [rVBK], a           ;3

    ;Place palette data
    ld a, \2                ;2
    ld [hl+], a             ;2
    ld [hl-], a             ;2
    set 5, l                ;2
    ld [hl+], a             ;2
    ld [hl-], a             ;2

    ;Reset VRAM bank
    xor a                   ;1
    ldh [rVBK], a           ;3



    ;Preparation            =14
    push de                 ;4
    push hl                 ;4
    ld h, d                 ;1
    ld l, e                 ;1
    ld c, 0                 ;2
    ld d, \3                ;2



    ;Middle left            =7/8
    dec l
    ld a, [hl+]
    cp a, d
    jr nz, :+
    set 4, c
    :

    ;Middle right           =7/8
    inc l
    ld a, [hl]
    cp a, d
    jr nz, :+
    set 3, c
    :
    
    ;Top middle             =13/14
    ld a, l
    sub a, 65
    ld l, a
    jr nc, :+
    dec h
    :
    ld a, [hl-]
    cp a, d
    jr nz, :++
        set 6, c

        ;Top left               =0/7/8
        ld a, [hl+]
        inc l
        cp a, d
        jr nz, :+
        set 7, c
        :

        ;Top right              =0/7/8
        ld a, [hl-]
        dec l
        cp a, d
        jr nz, :+
        set 5, c
    :

    ;Bottom middle          =13/14
    ld a, l
    add a, 129
    ld l, a
    jr nc, :+
    inc h
    :
    ld a, [hl-]
    cp a, d
    jr nz, :++
        set 1, c

        ;Bottom middle          =0/7/8
        ld a, [hl+]
        inc l
        cp a, d
        jr nz, :+
        set 2, c
        :

        ;Bottom right           =0/6/7
        ld a, [hl+]
        cp a, d
        jr nz, :+
        set 0, c
    :

    ;Start processing values
    pop hl                  ;3
    ld d, high(dwellings_autotile_lookup) ;2

    ;Bottom left            =10
    ld a, c
    and a, %00010110
    ld e, a
    ld a, [de]
    add a, \1 + $10
    ld [hl+], a

    ;Bottom right           =10
    ld a, c
    and a, %00001011
    ld e, a
    ld a, [de]
    add a, \1 + $18
    ld [hl-], a

    
    ;Top left               =12
    res 5, l
    ld a, c
    and a, %11010000
    ld e, a
    ld a, [de]
    add a, \1 + $00
    ld [hl+], a

    ;Top right              =10
    ld a, c
    and a, %01101000
    ld e, a
    ld a, [de]
    add a, \1 + $08
    ld [hl], a

    ;Finally, this thing
    pop de                  ;3
ENDM







; Macro for a tiled floor.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette
; - 3: Block ID
tm_floor: MACRO
    
    ;Palettes               =21
    ;Switch VRAM bank
    ld a, 1                 ;2
    ldh [rVBK], a           ;3

    ;Place palette data
    ld a, \2                ;2
    ld [hl+], a             ;2
    ld [hl-], a             ;2
    set 5, l                ;2
    ld [hl+], a             ;2
    ld [hl-], a             ;2

    ;Reset VRAM bank
    xor a                   ;1
    ldh [rVBK], a           ;3




    ;Save block pointer     =8
    push de                 ;4
    push de                 ;4

    ;Check block below (+63)=7
    ld a, e                 ;1
    add a, 63               ;2
    ld e, a                 ;1
    jr nc, :+               ;2
    inc d                   ;1
    :

    ;Write temporary data   =10
    ld a, [de]              ;2
    ld [hl+], a             ;1
    inc e                   ;1
    inc e                   ;1
    ld a, [de]              ;2
    ld [hl-], a             ;2
    dec e                   ;1




    ;Tile ID                ;2
    ld c, \1+16

    ;If block below is NOT floor =7-8
    ld a, [de]              ;2
    cp a, \3                ;2
    jr z, :+                ;3/4
        
        ;Switch to bottom tiles
        inc c                   ;1
        inc c                   ;1
    :

    ;Restore block pointer
    pop de                  ;3

    ;Check block to the left
    dec e                   ;1

    ;If block to the left is NOT floor
    ld a, [de]              ;2
    cp a, \3                ;2
    jr z, :+                ;2

    ;Switch to edge sprite
    inc c                   ;1
    :

    ;Tile is surrounded, check corner
    ld a, c                 ;1
    cp a, \1+16             ;2
    jr nz, :+               ;3/(8/9)

        ;Test if corner tile is floor
        ld a, [hl]              ;2
        cp a, \3                ;2
        ld a, c                 ;1
        jr z, :+                ;3/4

            ;Switch to inverted corner tile
            set 2, a                ;2
    :

    ;Place tile 3
    ld [hl+], a             ;2
    
    ;Switch to next tile
    and a, %11111010        ;2
    add a, 8                ;2
    ld c, a                 ;1

    ;Increment block pointer
    inc e                   ;1
    inc e                   ;1

    ;Check block to the right
    ;If block is NOT floor
    ld a, [de]              ;2
    cp a, \3                ;2
    jr z, :+                ;2

    ;Switch to edge tile
    inc c                   ;1
    :

    ;Tile is surrounded, check corner
    ld a, c                 ;1
    cp a, \1+24             ;2
    jr nz, :+               ;3/(7/10)

        ;Test if corner tile is floor
        ld a, [hl]              ;2
        cp a, \3                ;2
        jr z, :+                ;3/4

            ;Switch to inverted corner tile
            set 2, c                ;2
    :

    ;Place tile 4
    ld [hl], c              ;2

    
    
    
    ;Set block pointer and tile ID
    res 5, l                ;2
    ld a, c                 ;1
    and a, %11101001        ;2
    ld c, a                 ;1

    ;Read top corners
    ld a, e                 ;1
    sub a, 64               ;2
    ld e, a                 ;1
    jr nc, :+               ;2
    dec d                   ;1
    :

    ;Save corner blocks
    ld a, [de]              ;2
    ld [hl-], a             ;2
    dec e                   ;1
    dec e                   ;1
    ld a, [de]              ;2
    ld [hl+], a             ;2
    inc e                   ;1

    ;If block above is NOT floor
    ld a, [de]              ;2
    cp a, \3                ;2
    jr z, :+                ;3/4

        ;Switch to top tile
        inc c                   ;1
        inc c                   ;1
    :

    ;Is tile surrounded?
    ld a, c                 ;1
    cp a, \1+8              ;2
    jr nz, :+               ;3/8/9

        ;Is corner tile NOT floor?
        ld a, [hl]              ;2
        cp a, \3                ;2
        ld a, c                 ;1
        jr z, :+                ;3/4

            ;Switch to inverse corner tile
            add a, 4                ;2
    :

    ;Place tile 2
    ld [hl-], a             ;2

    ;Move on to the next tile
    and a, %11110010        ;2
    ld c, a                 ;1

    ;Get side thing
    set 5, l                ;2
    bit 0, [hl]             ;4
    res 5, l                ;2
    jr z, :+                ;2
    inc c                   ;1
    :

    ;Is tile surrounded?
    ld a, c                 ;1
    cp a, \1                ;2
    jr nz, :+               ;3/8/9

        ;Is corner tile NOT floor?
        ld a, [hl]              ;2
        cp a, \3                ;2
        ld a, c                 ;1
        jr z, :+                ;3/4

            ;Switch to inverse corner tile
            add a, 4                ;2
    :

    ;Place tile 1
    ld [hl+], a             ;2

    ;Restore tilemap pointer
    pop de                  ;3

ENDM