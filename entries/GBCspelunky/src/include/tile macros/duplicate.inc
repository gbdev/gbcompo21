; Macro for a basic tile.
; Places the given tile 4 times
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_duplicate: MACRO
    
    ;Place tiles
    ld a, \1
    ld [hl+], a
    ld [hl-], a
    set 5, l
    ld [hl+], a
    ld [hl-], a

    ;Switch VRAM bank
    ld a, 1
    ld [rVBK], a

    ;Place palettes
    ld a, \2
    ld [hl+], a
    ld [hl-], a
    res 5, l
    ld [hl+], a
    ld [hl], a

    ;Reset VRAM bank
    xor a
    ldh [rVBK], a
ENDM


; Macro for a basic tile.
; Places the given tile 2 times
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_duplicate_h: MACRO
    
    ;Place tiles
    ld a, \1
    ld [hl+], a
    ld [hl-], a

    ;Switch VRAM bank
    ld a, 1
    ld [rVBK], a

    ;Place palettes
    ld a, \2
    ld [hl+], a
    ld [hl], a

    ;Reset VRAM bank
    xor a
    ldh [rVBK], a
ENDM