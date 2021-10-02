INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "entitysystem.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "physics.inc"
INCLUDE "entities/treasure.inc"
INCLUDE "blockproperties.inc"

SECTION "ENTITY TREASURE", ROMX, BANK[bank_entities]

;Treasure initialization data
entity_treasure_init::
    db bank_entities ;Bank ID
    dw entity_treasure_execute ;Code address
    db entsys_col_visibleF | entsys_col_treasureF ;Collision mask
    dw entity_treasure_destroy ;Destruction code
    db bank_entities ;Destruction code bank
    dw entity_treasure_create ;Initialization code
;



; treasure creation code.
;
; Input:
; - `de`: Entity variable pointer
entity_treasure_create::
    
    ;Transfer pointer to HL
    ld h, d
    ld l, e

    ;Set X, Y, direction and speed to 0
    inc l
    xor a
    ld [hl+], a
    ld [hl], (256 - treasure_width) >> 1
    inc l
    ld [hl+], a
    ld [hl], 255 - treasure_height
    inc l
    ld [hl+], a
    ld [hl+], a
    ld [hl+], a
    inc l

    ;Write width
    xor a
    ld [hl+], a
    ld [hl], treasure_width
    inc l

    ;Write height
    ld [hl+], a
    ld [hl], treasure_height

    ;Get a random preset
    ld a, l
    add a, treasure_variable_sprite - (entity_height+1)
    ld l, a

    call rng_run_single
    bit 2, a
    jr z, .pile

        ;Gold ingot
        ;Write sprite ID
        ld [hl], s_treasure_goldbar

        ;Write value
        inc l
        xor a
        ld [hl+], a
        ld [hl+], a
        ld [hl], $05
        inc l
        ld [hl+], a

        ;Enable physics
        dec a
        ld [hl+], a

        ;Return
        ret 
    ;

    .pile
        ;Gold ingot pile
        ;Write sprite ID
        ld [hl], s_treasure_goldpile

        ;Write value
        inc l
        xor a
        ld [hl+], a
        ld [hl+], a
        ld [hl], $15
        inc l
        ld [hl+], a

        ;Enable physics
        dec a
        ld [hl+], a

        ;Return
        ret 

    ;Return
    .return
    ret 
;


; Treasure destroy event.
;
; Input:
; - `bc`: Entity pointer (anywhere)
;
; Output:
; - `b`: Free entity (0 = no)
entity_treasure_destroy::

    ;What
    ld a, c
    and a, %11000000
    or a, entity_sprite
    ld l, a
    ld h, b

    ;Free sprites
    ld a, [hl]
    cp a, $A0
    jr z, :+
        ld b, 2
        call sprite_free
    :

    ;Add money value
    ld a, l
    add a, (treasure_variable_value + 3) - entity_sprite
    ld c, a
    ld b, h
    call hud_money_add

    ;Return
    ld b, $FF
    ret 
;



; treasure execution code.
;
; Input:
; - `de`: Entity variable pointer
entity_treasure_execute::

    ;Move pointer to HL
    ld h, d
    ld a, e
    add a, treasure_variable_physics - entity_state
    ld l, a

    ;Check if physics are enabled
    ld a, [hl]
    cp a, 0
    jr nz, .physics

        ;Physics are disabled, run a few checks
        ;Check speeds
        ld a, e
        add a, entity_hspp - entity_state
        ld l, a
        xor a
        cp a, [hl]
        jr nz, .physics
        inc l
        cp a, [hl]
        jr nz, .physics

        ;Speeds are OK, check tile(s) below
        dec l
        dec l
        dec l
        dec l
        ld a, [hl-]
        dec l
        dec a
        ld c, a
        ld d, [hl]
        coordinates_level d, c, d, e

        ;Switch RAM bank
        ld a, bank_foreground
        ldh [rSVBK], a

        ;Check tile properties
        ld a, [de]
        ld c, a
        ld b, high(w_blockdata)
        ld a, [bc]
        bit bpb_solid, a
        jr nz, .nofall

        ;Tile was not solid, is there a neighbor tile?
        inc l
        ld a, [hl-]
        add a, treasure_width
        jr nc, .skipextra

        ;There is, check that out
        inc e
        ld a, [de]
        ld c, a
        ld a, [bc]
        bit bpb_solid, a
        jr nz, .nofall

        ;Switch RAM banks and jump to physics routine
        .skipextra
        ld a, bank_entsys
        ldh [rSVBK], a
        jr .physics


        .nofall
        ;Switch RAM banks back
        ld a, bank_entsys
        ldh [rSVBK], a

        ;Jump
        jr .donephysics

    ;Do physics n stuff
    .physics
        ld a, l
        and a, %11000000
        or a, entity_variables
        ld l, a
        call entsys_gravity
        call entsys_physics_tumble

        ;Check speeds
        ld a, l
        and a, %11000000
        or a, entity_variables
        ld e, a
        add a, entity_hspp - entity_state
        ld l, a
        xor a
        cp a, [hl]
        jr nz, .activephys
        inc l
        cp a, [hl]
        jr nz, .activephys

        ;If both speeds are 0, decrement physics counter
        ld a, e
        add a, treasure_variable_physics - entity_state
        ld l, a
        dec [hl]
        jr .donephysics

        ;Otherwise, reset physics counter
        .activephys
        ld a, e
        add a, treasure_variable_physics - entity_state
        ld l, a
        ld [hl], treasure_physics_enabled
        ;Falls into label `.donephysics`.

    .donephysics

    ;Check animation flag
    ld a, l
    and a, %11000000
    or a, entity_visible
    ld l, a
    ld a, [hl]
    sra a
    res 5, a
    cp a, [hl]
    ld [hl], a

    ;Flags are not the same
    ;Appear or disappear
    jr z, .visible_regular

        ;Visible flag changed
        or a, a ;Sets Z flag if A is 0
        jr z, .invisible

            ;Entity should appear on screen
            ;Allocate sprites
            ld a, 2
            call sprite_alloc_multi
            inc l
            ld [hl-], a

            ld e, a
            ld d, high(w_oam_mirror)
            inc e
            inc e
            ld a, s_treasure + 2
            ld [de], a
            inc e
            ld a, OAMF_PAL1 | p_gold
            ld [de], a
            inc e
            inc e
            inc e
            ld a, s_treasure + 2
            ld [de], a
            inc e
            ld a, OAMF_PAL1 | p_gold
            ld [de], a

            ;Jump
            jr .visible_regular

        .invisible

            ;Entity should disappear
            ;Free allocated sprite
            inc l
            ld a, [hl]
            ld [hl], $A0
            dec l
            ld b, 2
            call sprite_free
            ;Falls into label `.visible_regular`
    ;

    ;Is visible flag set? ;HL = `entity_visible`
    .visible_regular
    bit entsys_visible_currentB, [hl]
    ret z

        ;Entity is visible, show that sprite!
        ;Grab sprite ID
        inc l
        ld d, [hl]

        ;Get player position
        ld a, l
        sub a, entity_sprite - entity_x
        ld l, a

        ;Convert X-position
        ldh a, [h_scx]
        ld e, a
        ld a, [hl+]
        and a, %00001111
        ld b, a
        ld a, [hl+]
        and a, %11110000
        or a, b
        swap a
        sub a, e
        add a, 5
        ld b, a

        ;Convert Y-position
        ldh a, [h_scy]
        ld e, a
        ld a, [hl+]
        and a, %00001111
        ld c, a
        ld a, [hl+]
        and a, %11110000
        or a, c
        swap a
        sub a, e
        add a, 9
        ld c, a

        ;Write sprite positions
        ;Register shuffling
        ld a, l
        add a, treasure_variable_sprite - entity_direction
        ld e, a
        ld a, d
        ld d, h
        ld l, a
        ld h, high(w_oam_mirror)

        ld a, c
        ld [hl+], a
        ld a, b
        ld [hl+], a
        add a, 8
        inc l
        inc l
        ld [hl], c
        inc l
        ld [hl+], a

        ;Get sprite data
        ld b, p_gold + OAMF_PAL1
        ld a, [de]
        ld c, a
        inc a
        inc a

        ;Store the data
        ld [hl+], a
        ld [hl], b
        ld a, l
        sub a, 5
        ld l, a

        ld [hl], c
        inc l
        ld [hl], b


    ;Return
    ret
;