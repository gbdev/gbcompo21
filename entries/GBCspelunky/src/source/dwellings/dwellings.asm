INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "blockproperties.inc"
INCLUDE "dwellings/tileid.inc"

SECTION "DWELLINGS DATA", ROMX, BANK[bank_dwellings_main], ALIGN[8]

;Dwellings 8-way autotiling lookup table
dwellings_autotile_lookup::
    ;  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    db 3, 3, 1, 1, 3, 9, 1, 9, 2, 2, 4, 0, 9, 9, 9, 9  ;0
    db 2, 9, 4, 9, 2, 9, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;1
    db 3, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9  ;2
    db 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;3
    db 1, 9, 9, 9, 9, 9, 9, 9, 4, 9, 9, 9, 9, 9, 9, 9  ;4
    db 4, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;5
    db 1, 9, 9, 9, 9, 9, 9, 9, 0, 9, 9, 9, 9, 9, 9, 9  ;6
    db 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;7
    db 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;8
    db 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;9
    db 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;A
    db 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;B
    db 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;C
    db 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;D
    db 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;E
    db 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9  ;F
;



;Dwellings block properties
dwellings_blockdata::
    db bpf_indestructible               ;Background
    db bpf_solid                        ;Dirt
    db bpf_ladder                       ;Ladder
    db bpf_spike                        ;Spikes
    db bpf_ladder | bpf_platform        ;Ladderplatform
    db bpf_platform                     ;Platform
    db bpf_solid                        ;Wood
    db 0                                ;Wood background
    db bpf_solid | bpf_destructible     ;Pushblock
    db bpf_solid                        ;Arrowtrap left
    db bpf_solid                        ;Altar
    db bpf_destructible | bpf_solid     ;Bone block
    db 0                                ;Platform support
    db bpf_solid                        ;Arrowtrap right
    db bpf_solid | bpf_indestructible   ;Border
    db bpf_indestructible               ;Backdoor entrance
    db 0                                ;Campfire
    db bpf_indestructible               ;Door left
    db bpf_indestructible               ;Door right
    db bpf_indestructible               ;Door entrance
    db bpf_indestructible               ;Door exit
    db bpf_spike                        ;Spikes (support)
    db bpf_spike                        ;Spikes (wood BG)
    db bpf_indestructible               ;Door left (support)
    db bpf_indestructible               ;Door right (support)
    db bpf_indestructible               ;Door left (wood)
    db bpf_indestructible               ;Door right (wood)
    .blockdata_end:

    ;Fill remaining space with 0's to prevent invalid data
    REPT 256 - (.blockdata_end - dwellings_blockdata)
    db bpf_solid
    ENDR
.end


;Graphics

;Dwellings tileset
dwellings_tiles:
    INCBIN "dwellings/gfx/tiles.tls"
    .end

;Dwellings entities
dwellings_sprites:
    INCBIN "entities/caveman.tls"
    INCBIN "entities/snake.tls"
    INCBIN "entities/treasure.tls"
    .end

;Dwellings background color palette
dwellings_palettes_start:
    INCBIN "dwellings/gfx/palettes.pal"

;Dwellings entity initialization pointers
dwellings_entities_ground::
    dw entity_caveman_init
    dw entity_caveman_init
    dw entity_snake_init
    dw entity_snake_init
    ASSERT high(@) == high(dwellings_entities_ground)
;

; Loads Dwellings into VRAM.
;
; Assumes the LCD is turned off when ran
;
; Destroys: all
dwellings_load::

    ;Set VRAM bank
    xor a
    ldh [rVBK], a

    ;Set world bank variable
    ld a, bank_dwellings_main
    ld [w_world_bank], a

    ;Copy blockdata to WRAM0
    ld hl, w_blockdata
    ld bc, dwellings_blockdata
    ld de, $100
    call memcopy
    
    ;Copy tiles through HDMA
    ld hl, rHDMA1
    ld a, high(dwellings_tiles)
    ld [hl+], a
    ld a, low(dwellings_tiles)
    ld [hl+], a
    ld a, $90
    ld [hl+], a
    xor a
    ld [hl+], a
    ld a, ((dwellings_tiles.end - dwellings_tiles) >> 4) - 1
    ld [hl], a

    ;Copy entities
    ld hl, $8000 + (s_entities << 4)
    ld bc, dwellings_sprites
    ld de, dwellings_sprites.end - dwellings_sprites
    call memcopy

    ;Copy palettes
    ld hl, dwellings_palettes_start
    xor a
    call palette_copy_bg
    call palette_copy_bg
    call palette_copy_bg
    call palette_copy_bg
    call palette_copy_bg
    call palette_copy_bg
    call palette_copy_bg
    call palette_copy_bg

    ;Return
    ret
;