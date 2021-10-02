INCLUDE "hardware.inc"

; Macro for a basic tile.
; Same as `tm_simple`, but flips it.
; 
; Input:
; - 1: Tile 1 ID
; - 2: Palette
tm_flip: MACRO
    
    ;Load tiles
    ld a, \1+1
    ld [hl+], a
    dec a
    ld [hl-], a
    add a, 3
    set 5, l
    ld [hl+], a
    dec a
    ld [hl-], a

    ;Load palettes
    ld a, 1
    ldh [rVBK], a
    ld a, \2 | OAMB_XFLIP
    ld [hl+], a
    ld [hl-], a
    res 5, l
    ld [hl+], a
    ld [hl], a
    xor a
    ldh [rVBK], a
ENDM