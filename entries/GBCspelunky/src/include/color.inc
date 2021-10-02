; Color macro, useful for defining palettes.
; Expands to a single `dw` command.
; 
; Input:
; - 1: Red (0-31)
; - 2: Green (0-31)
; - 3: Blue (0-31)
color_t: MACRO
    dw ((\3) << 10) + ((\2) << 5) + (\1)
ENDM

;The color that closest matches DMG black.
color_dmg_blk: MACRO
    color_t 0, 0, 0
ENDM

;The color that closest matches DMG dark gray.
color_dmg_dkg: MACRO
    color_t 7, 7, 7
ENDM

;The color that closest matches DMG light gray.
color_dmg_ltg: MACRO
    color_t 13, 13, 13
ENDM

;The color that closest matches DMG white.
color_dmg_wht: MACRO
    color_t 31, 31, 31
ENDM