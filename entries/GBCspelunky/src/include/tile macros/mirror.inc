INCLUDE "hardware.inc"

; Macro for a basic tile.
; Both tiles share the same ID, but the second tile is mirrored.
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_mirror: MACRO
    
    ;Load tiles
    ld a, \1
    ld [hl+], a
    ld [hl-], a
    set 5, l
    ld [hl+], a
    ld [hl-], a

    ;Load palettes
    ld a, 1
    ldh [rVBK], a
    ld a, \2
    ld [hl+], a

    ;Load palette 2 + flip
    set OAMB_XFLIP, a
    ld [hl], a
    res 5, l
    ld [hl-], a
    res OAMB_XFLIP, a
    ld [hl+], a

    ;Restore VRAM bank
    xor a
    ldh [rVBK], a
ENDM



; Macro for a basic tile.
; Both tiles share the same ID, but the second tile is mirrored.
; Only copies half a tile
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_mirror_h: MACRO
    
    ;Load tile 1
    ld c, \1
    ld [hl], c

    ;Load palette 1
    ld a, 1
    ldh [rVBK], a
    ld a, \2
    ld [hl+], a

    ;Load palette 2 + flip
    add a, OAMF_XFLIP
    ld [hl], a

    ;Load tile 2
    xor a
    ldh [rVBK], a
    ld [hl], c
ENDM