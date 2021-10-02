INCLUDE "hardware.inc"

; Places the left side of a door.
; Automatically knows if it's the top or bottom.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Background tile ID
; - 5: Background palette ID
tm_door_left: MACRO

    ;Place palettes =25
    ld a, 1         ;2
    ld [rVBK], a    ;3
    ld a, \5        ;2
    ld [hl], a      ;2
    set 5, l        ;2
    ld [hl+], a     ;2
    ld a, \2        ;2
    ld [hl], a      ;2
    res 5, l        ;2
    ld [hl-], a     ;2
    xor a           ;1
    ld [rVBK], a    ;3

    
    ;Get vertical offset (bit 2-3)
    ld a, l
    and a, %01100000
    swap a
    rlca 
    ld c, a

    ;Add horizontal offset (bit 0-1)
    ld a, l
    and a, %00000011
    add a, c

    ;Add base tile offset
    add a, \4

    ;Place backgrounds (tile 1 + 3)
    ld [hl], a
    set 5, l
    add a, 4
    ld [hl+], a

    ;Check tile below
    ld c, 64
    ld a, e
    add a, c
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is tile a door?
    ld a, [de]
    cp a, \3
    ld a, \1+1
    jr z, :+
        
        ;Tile is NOT door, this is a bottom tile
        add a, 2
    :

    ;Place door tiles (4+2)
    ld [hl], a
    dec a
    res 5, l
    ld [hl], a
    
    ;Move block pointer back
    ld a, e
    sub a, c
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the right side of a door.
; Automatically knows if it's the top or bottom.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Background tile ID
; - 5: Background palette ID
tm_door_right: MACRO

    ;Place palettes =25
    ld a, 1         ;2
    ld [rVBK], a    ;3
    ld a, \2 + OAMF_XFLIP ;2
    ld [hl], a      ;2
    set 5, l        ;2
    ld [hl+], a     ;2
    ld a, \5        ;2
    ld [hl], a      ;2
    res 5, l        ;2
    ld [hl], a      ;2
    xor a           ;1
    ld [rVBK], a    ;3

    
    ;Get vertical offset (bit 2-3)
    ld a, l
    and a, %01100000
    swap a
    rlca 
    ld c, a

    ;Add horizontal offset (bit 0-1)
    ld a, l
    and a, %00000011
    add a, c

    ;Add base tile offset
    add a, \4

    ;Place backgrounds (tile 1 + 3)
    ld [hl], a
    set 5, l
    add a, 4
    ld [hl-], a

    ;Check tile below
    ld c, 64
    ld a, e
    add a, c
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is tile a door?
    ld a, [de]
    cp a, \3
    ld a, \1+1
    jr z, :+
        
        ;Tile is NOT door, this is a bottom tile
        add a, 2
    :

    ;Place door tiles (4+2)
    ld [hl], a
    dec a
    res 5, l
    ld [hl+], a
    
    ;Move block pointer back
    ld a, e
    sub a, c
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the left side of a door.
; Automatically knows if it's the top or bottom.
; Places a 1strip background.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Background tile ID
; - 5: Background palette ID
tm_door_left_1strip: MACRO

    ;Place palettes =25
    ld a, 1         ;2
    ld [rVBK], a    ;3
    ld a, \5        ;2
    ld [hl], a      ;2
    set 5, l        ;2
    ld [hl+], a     ;2
    ld a, \2        ;2
    ld [hl], a      ;2
    res 5, l        ;2
    ld [hl-], a     ;2
    xor a           ;1
    ld [rVBK], a    ;3

    ;Check tile below
    ld a, e
    add a, 64
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is tile a door?
    ld a, [de]
    cp a, \3
    ld a, \1+1
    jr z, :+
        
        ;Tile is NOT door, this is a bottom tile
        add a, 2
        ld [hl], \4
        set 5, l
        ld [hl], \4+2
    :
    jr nz, :+
        ld [hl], \4+1
        set 5, l
        ld [hl], \4
    :

    inc l

    ;Place door tiles (4+2)
    ld [hl], a
    dec a
    res 5, l
    ld [hl], a
    
    ;Move block pointer back
    ld a, e
    sub a, 64
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the right side of a door.
; Automatically knows if it's the top or bottom.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Background tile ID
; - 5: Background palette ID
tm_door_right_1strip: MACRO

    ;Place palettes =25
    ld a, 1         ;2
    ld [rVBK], a    ;3
    ld a, \2 + OAMF_XFLIP ;2
    ld [hl], a      ;2
    set 5, l        ;2
    ld [hl+], a     ;2
    ld a, \5        ;2
    ld [hl], a      ;2
    res 5, l        ;2
    ld [hl], a      ;2
    xor a           ;1
    ld [rVBK], a    ;3

    ;Check tile below
    ld a, e
    add a, 64
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is tile a door?
    ld a, [de]
    cp a, \3
    ld a, \1+1
    jr z, :+
        
        ;Tile is NOT door, this is a bottom tile
        add a, 2

        ;Write tile 2+4
        ld [hl], \4
        set 5, l
        ld [hl], \4+2
    :
    jr nz, :+
        
        ;Write tile 2+4
        ld [hl], \4+1
        set 5, l
        ld [hl], \4
    :

    ;Place door tiles (3+1)
    dec l
    ld [hl], a
    dec a
    res 5, l
    ld [hl+], a
    
    ;Move block pointer back
    ld a, e
    sub a, 64
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the left side of a door.
; Automatically knows if it's the top or bottom.
; Places a 1strip background.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Background tile ID
; - 5: Background palette ID
tm_door_left_duplicate: MACRO

    ;Place palettes =25
    ld a, 1         ;2
    ld [rVBK], a    ;3
    ld a, \5        ;2
    ld [hl], a      ;2
    set 5, l        ;2
    ld [hl+], a     ;2
    ld a, \2        ;2
    ld [hl], a      ;2
    res 5, l        ;2
    ld [hl-], a     ;2
    xor a           ;1
    ld [rVBK], a    ;3

    ;Check tile below
    ld a, e
    add a, 64
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is tile a door?
    ld a, [de]
    cp a, \3
    ld a, \1+1
    jr z, :+
        
        ;Tile is NOT door, this is a bottom tile
        add a, 2
        ld [hl], \4
        set 5, l
        ld [hl], \4
    :
    jr nz, :+
        ld [hl], \4
        set 5, l
        ld [hl], \4
    :

    inc l

    ;Place door tiles (4+2)
    ld [hl], a
    dec a
    res 5, l
    ld [hl], a
    
    ;Move block pointer back
    ld a, e
    sub a, 64
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the right side of a door.
; Automatically knows if it's the top or bottom.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Background tile ID
; - 5: Background palette ID
tm_door_right_duplicate: MACRO

    ;Place palettes =25
    ld a, 1         ;2
    ld [rVBK], a    ;3
    ld a, \2 + OAMF_XFLIP ;2
    ld [hl], a      ;2
    set 5, l        ;2
    ld [hl+], a     ;2
    ld a, \5        ;2
    ld [hl], a      ;2
    res 5, l        ;2
    ld [hl], a      ;2
    xor a           ;1
    ld [rVBK], a    ;3

    ;Check tile below
    ld a, e
    add a, 64
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is tile a door?
    ld a, [de]
    cp a, \3
    ld a, \1+1
    jr z, :+
        
        ;Tile is NOT door, this is a bottom tile
        add a, 2

        ;Write tile 2+4
        ld [hl], \4
        set 5, l
        ld [hl], \4
    :
    jr nz, :+
        
        ;Write tile 2+4
        ld [hl], \4
        set 5, l
        ld [hl], \4
    :

    ;Place door tiles (3+1)
    dec l
    ld [hl], a
    dec a
    res 5, l
    ld [hl+], a
    
    ;Move block pointer back
    ld a, e
    sub a, 64
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the middle part of an entrance door.
; Automatically knows if it's the top or bottom.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Blockade tile ID
; - 5: Blockade palette
tm_door_blockade: MACRO

    ;Move pointer down
    ld c, 64
    ld a, e
    add a, c
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is this tile a door?
    ld a, [de]
    cp a, \3
    jr nz, :+

        ;This tile is a top tile
        ;Place palettes
        ld a, 1
        ldh [rVBK], a
        ld a, \2
        ld [hl+], a
        add a, OAMF_XFLIP
        ld [hl], a
        set 5, l
        ld a, \5
        ld [hl-], a
        ld [hl], a
        xor a
        ld [rVBK], a

        ;Place blockade tiles
        ld a, \4
        ld [hl+], a
        inc a
        ld [hl-], a
        res 5, l

        ;Place top tiles
        ld a, \1
        ld [hl+], a
        ld [hl], a

        ;Jump to finish
        jr :++        


    :
        ;This tile is a bottom tile
        ;Place palettes
        ld a, 1
        ldh [rVBK], a
        ld a, \5
        ld [hl+], a
        ld [hl], a
        set 5, l
        ld [hl-], a
        ld [hl+], a
        xor a
        ld [rVBK], a

        ;Place tiles
        ld a, \4+5
        ld [hl-], a
        dec a
        ld [hl+], a
        dec a
        res 5, l
        ld [hl-], a
        dec a
        ld [hl+], a
    :

    ;Move pointer back into position
    ld a, e
    sub a, c
    ld e, a
    jr nc, :+
    dec d
    :
ENDM



; Places the middle part of an entrance door.
; Automatically knows if it's the top or bottom.
; 
; Input:
; - 1: Door tile ID
; - 2: Door palette
; - 3: Door block ID
; - 4: Free tile ID
; - 5: Free palette
tm_door_open: MACRO

    ;Move pointer down
    ld c, 64
    ld a, e
    add a, c
    ld e, a
    jr nc, :+
    inc d
    :

    ;Is this tile a door?
    ld a, [de]
    cp a, \3
    jr nz, :+

        ;This tile is a top tile
        ;Place palettes
        ld a, 1
        ldh [rVBK], a
        ld a, \2
        ld [hl+], a
        add a, OAMF_XFLIP
        ld [hl], a
        set 5, l
        ld a, \5
        ld [hl-], a
        ld [hl], a
        xor a
        ld [rVBK], a

        ;Place empty tiles
        ld a, \4
        ld [hl+], a
        ld [hl-], a
        res 5, l

        ;Place top tiles
        ld a, \1
        ld [hl+], a
        ld [hl], a

        ;Jump to finish
        jr :++        


    :
        ;This tile is a bottom tile
        ;Place palettes
        ld a, 1
        ldh [rVBK], a
        ld a, \5
        ld [hl+], a
        ld [hl], a
        set 5, l
        ld [hl-], a
        ld [hl+], a
        xor a
        ld [rVBK], a

        ;Place tiles
        ld a, \4
        ld [hl-], a
        ld [hl+], a
        res 5, l
        ld [hl-], a
        ld [hl+], a
    :

    ;Move pointer back into position
    ld a, e
    sub a, c
    ld e, a
    jr nc, :+
    dec d
    :
ENDM