; Macro for a 4x4 tile background.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette
tm_bg4x4: MACRO
    
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
    add a, \1



    ;Place tiles
    ;Top-left
    ld [hl+], a

    ;Top-right
    inc a
    ld [hl], a

    ;Bottom-right
    add a, 4
    set 5, l
    ld [hl-], a

    ;Bottom-left
    dec a
    ld [hl], a



    ;Switch VRAM bank
    ld c, low(rVBK)
    ld a, 1
    ld [c], a

    ;Apply palettes
    ld a, \2

    ;Bottom-left
    ld [hl+], a
    ld [hl-], a
    res 5, l
    ld [hl+], a
    ld [hl], a
    
    ;Reset VRAM bank
    xor a
    ld [c], a
ENDM




; Macro for a 4x4 tile background.
; Only fills in a half.
; L.5 will not be set/reset, must be done manually.
; 
; Input:
; - 1: Base tile ID
; - 2: Palette
tm_bg4x4_h: MACRO
    
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
    add a, \1



    ;Place tiles
    ld [hl+], a
    inc a
    ld [hl-], a

    ;Switch VRAM bank
    ld a, 1
    ldh [rVBK], a

    ;Place palettes
    ld a, \2
    ld [hl+], a
    ld [hl], a

    ;Switch VRAM bank again
    xor a
    ldh [rVBK], a
ENDM