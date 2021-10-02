; Macro for a basic tile.
; Places 4 tiles.
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_simple: MACRO
    
    ;Load tiles
    ld a, \1
    ld [hl+], a
    inc a
    ld [hl-], a
    inc a
    set 5, l
    ld [hl+], a
    inc a
    ld [hl-], a

    ;Load palettes
    ld a, 1
    ldh [rVBK], a
    ld a, \2
    ld [hl+], a
    ld [hl-], a
    res 5, l
    ld [hl+], a
    ld [hl], a
    xor a
    ldh [rVBK], a
ENDM



; Macro for a basic tile.
; Places 2 tiles.
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_simple_h: MACRO
    
    ;Load tile 1
    ld c, \1
    ld [hl], c
    
    ;Load palette 1
    ld a, 1
    ldh [rVBK], a
    ld a, \2
    ld [hl+], a

    ;Increment tile ID
    inc c

    ;Load palette 2
    ld [hl], a

    ;Load tile 2
    xor a
    ldh [rVBK], a
    ld [hl], c
ENDM