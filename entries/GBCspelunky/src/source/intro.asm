INCLUDE "hardware.inc"
INCLUDE "color.inc"

SECTION "INTRO", ROM0

intro_yellow equ $00
intro_red equ $40
intro_gray equ $80
intro_black equ $C0

intro::

    ;Wait for VBLANK
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ldh [rIE], a
    halt 

    ;There is Vblank!
    ;Disable LCD
    xor a
    ld [rLCDC], a

    ;Copy tiles to VRAM
    call hud_init

    ;Copy tilemap
    xor a
    ldh [rVBK], a
    ld hl, _SCRN0
    ld bc, intro_sukus
    ld de, $400
    call memcopy

    ;Load tile attributes
    ld a, 1
    ldh [rVBK], a 
    ld hl, _SCRN0
    ld b, OAMF_BANK1 | 1
    ld de, $400
    call memfill

    ;Make face use palette 0
        ;I am writing this code a few hours before the deadline,
        ;I don't have TIME for neat solutions!!!
        ld hl, _SCRN0 + 6 + (32 * 3)
        ld b, OAMF_BANK1
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 4)
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 5)
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 6)
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 7)
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 8)
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 9)
        ld e, 8
        call memfill
        ld hl, _SCRN0 + 6 + (32 * 10)
        ld e, 8
        call memfill
    ;

    ;Set intro flags
    xor a
    ld [w_intro_timer], a
    ld [w_intro_state], a

    ;Reenable LCD
    ld hl, rLCDC
    ld a, LCDCF_ON | LCDCF_BGON
    ld [hl], a

    ;Fade in
    .fade
    ld hl, w_intro_timer
    inc [hl]
    ld a, [hl]
    add a, a
    and a, %00111111
    ld c, a
    call intro_faderoutine
    ld a, e
    cp a, $3E
    jr z, .phase2

    ;Wait for Vblank
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ld [rIE], a
    halt 
    jr .fade

    .phase2

    ;Wait for Vblank
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ld [rIE], a
    halt 

    ;Now in Vblank, count down
    ld hl, w_intro_timer
    dec [hl]
    jr nz, .phase2

    ;Wait for Vblank (again)
    .fadeout
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ld [rIE], a
    halt 

    ;Fade out
    ld hl, w_intro_timer
    dec [hl]
    ld a, [hl]
    add a, a
    and a, %00111111
    ld c, a
    call intro_faderoutine
    ld a, e
    cp a, $00
    jr nz, .fadeout

    ;Wait for Vblank but again this time
    xor a
    ldh [rIF], a
    ld a, IEF_VBLANK
    ld [rIE], a
    halt 

    ;Disable LCD
    xor a
    ld [rLCDC], a

    ;Tile attributes
    ld hl, _SCRN0
    ld de, $400
    ld b, OAMF_BANK1
    call memfill

    ;Copy tilemap to thing thong bingamabong
    xor a
    ldh [rVBK], a
    ld hl, _SCRN0
    ld de, $400
    ld bc, intro_message
    call memcopy

    ;Copy text palette
    xor a
    ld hl, intro_palette_text
    call palette_copy_bg

    ;Re-enable LCD
    ld a, LCDCF_BGON | LCDCF_ON
    ldh [rLCDC], a

    ;Write RNG seeds
    ld a, $7E
    ldh [h_rng_seed], a
    ld a, $B2
    ldh [h_rng_seed+1], a

    ;Check input a bunch
    .inputwait
    call rng_run
    call input
    bit PADB_START, c
    jr z, .inputwait

    ;NOW WE'RE TALKING!
    ret 
;

intro_faderoutine:
    ld de, w_intro_color_buffer
    ld hl, intro_palettes + intro_yellow

    ;Palette 1
    add a, l
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl]
    ld [de], a
    inc e
    ld a, c
    add a, low(intro_palettes + intro_red)
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl]
    ld [de], a
    inc e
    ld a, c
    add a, low(intro_palettes + intro_gray)
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl]
    ld [de], a
    inc e
    ld a, c
    add a, low(intro_palettes + intro_black)
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl-]
    ld [de], a
    inc e

    ;Palette 2
    ld a, $FF
    ld [de], a
    inc e
    ld [de], a
    inc e
    ld a, c
    add a, low(intro_palettes + intro_black)
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl-]
    ld [de], a
    inc e
    ld a, c
    add a, low(intro_palettes + intro_gray)
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl]
    ld [de], a
    inc e
    ld a, c
    add a, low(intro_palettes + intro_black)
    ld l, a
    ld a, [hl+]
    ld [de], a
    inc e
    ld a, [hl-]
    ld [de], a
    ld e, c
    
    ;Copy palettes
    ld hl, w_intro_color_buffer
    xor a
    call palette_copy_bg
    call palette_copy_bg

    ;Return
    ret 
;



align 8
intro_palettes:
    INCBIN "intro/colorssuck.bin"
intro_sukus:
    INCBIN "intro/sukus.tlm"
intro_message:
    INCBIN "intro/message.tlm" 
intro_palette_text:
    color_t 31, 31, 31
    color_t 0, 0, 0
    color_t 21, 21, 21
    color_t 0, 0, 0