INCLUDE "hardware.inc"

SECTION "SPRITES", ROM0
; DMA routune to be copied to HRAM
;
; DO NOT CALL!!!
dma_routine:
    
    ;Initialize OAM DMA
    ld a, HIGH(w_oam_mirror)
    ldh [rDMA], a
    ld a, 40

    ;Wait until transfer is complete
    .wait
    dec a
    jr nz, .wait

    ;Return
    ret
;



; Copy the DMA routine to HRAM
;
; Destroys: all
sprite_setup::
    
    ;Copy DMA routine to HRAM
    ld hl, h_dma_routine
    ld bc, dma_routine
    ld de, 10
    call memcopy

    ;Clear WRAM at shadow OAM
    ld hl, w_oam_mirror
    ld b, 0
    ld de, $9F
    call memfill

    ;Reset lowest sprite slot variable
    ld a, 3
    ld [w_spriteslot_lowest], a

    ;Return
    ret
;



; Allocates a single sprite slot.
;
; Output:
; - `a` = sprite slot (0-39) * 4
;
; Destroys: `hl`, `b`
sprite_alloc_single::
    
    ;Initialize pointer
    ld hl, w_oam_mirror + 3

    ;Get bit to check with
    ld b, %00010000
    ld a, [h_is_color]
    cp a, 0
    jr nz, .cgb
    ld b, %00000001
    .cgb

    .loop
    ;check data
    ld a, b
    and a, [hl]
    cp a, 0

    ;Jump is tile is free
    jr z, .found

    ;If not, count more
    ld a, 4
    add a, l
    ld l, a

    ;If tile is out of range, return invalid slot
    cp a, 163
    jr z, .found

    jr .loop

    .found
    ;Return the sprite ID * 4
    ld a, b
    or a, [hl]
    ld [hl], a
    ld a, l
    sub a, 3
    ret
;



; Allocates multiple sprite slots.
;
; Input:
; - `a`: Sprite count
;
; Output:
; - `a`: Sprite slot (0-39 * 4, $FF if failed)
;
; Destroys: `de`
sprite_alloc_multi::
    
    push hl
    
    ;Set sprite count
    ld e, a
    ld d, e

    ;Set shadow OAM pointer
    ld h, high(w_oam_mirror)
    ld l, 03

    ;Start loopin'
    .loop
        
        ;Test if sprite is already allocated
        bit OAMB_PAL1, [hl]
        ld d, e
        jr z, .subloop

        ;Sprite was allocated
        .gonext
        ld a, l
        add a, 4
        ld l, a
        jr .loop


        ;Unallocated sprite was found!
        ;Check following sprites
        .subloop
            
            ;Decrement counter
            dec d
            jr z, .final

            ;Go to next sprite
            ld a, l
            add a, 4
            ld l, a

            ;Sprite is not allocated, keep going
            bit OAMB_PAL1, [hl]
            jr z, .subloop

            ;Sprite was allocated, go back
            jr .gonext
    
    ;Enough sprites were found
    .final

        ;Flag sprite as allocated
        ld [hl], OAMF_PAL1
        inc d
        ld a, d
        cp a, e
        jr z, .return

        ;Decrement pointer
        ld a, l
        sub a, 4
        ld l, a
        jr .final
    
    ;Return
    .return

        ;Return
        ld a, l
        sub a, 3
        pop hl
        ret
;



; Frees a previously allocated sprite
;
; Input:
; - `a` = sprite slot (0-39) * 4
; - `b` = sprite count
;
; Destroys: `a`, `b`
sprite_free::

    push hl
    
    ;Get sprite address
    ld h, high(w_oam_mirror)
    ld l, a

    ;Clear sprite data
    xor a
    :
    ld [hl+], a
    ld [hl+], a
    ld [hl+], a
    ld [hl+], a

    ;Loop
    dec b
    jr nz, :-

    ;return
    pop hl
    ret
;