INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "entitysystem.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "physics.inc"
INCLUDE "entities/snake.inc"
INCLUDE "blockproperties.inc"

SECTION "ENTITY SNAKE", ROMX, BANK[bank_entities]

;Snake initialization data
entity_snake_init::
    db bank_entities ;Bank ID
    dw entity_snake_execute ;Code address
    db entsys_col_visibleF | entsys_col_enemyF | entsys_col_bounceF | entsys_col_damagableF ;Collision mask
    dw entity_snake_destroy
    db bank_entities
    dw entity_snake_create ;Initialization code
;



; Snake creation code.
;
; Input:
; - `de`: Entity variable pointer
entity_snake_create::
    
    ;Write state
    ld h, d
    ld l, e

    ;Get a bit of RNG
    call rng_run_single
    ld b, a

    ;Reset physics variables
    xor a
    ld [hl+], a ;state
    ld [hl+], a ;x
    ld [hl+], a
    ld [hl+], a ;y
    ld [hl+], a
    ld [hl], b ;direction
    inc l
    ld [hl+], a ;hspp
    ld [hl+], a ;vspp
    ld [hl], snake_health ;health
    inc l
    ld [hl+], a ;width
    ld [hl], snake_width
    inc l
    ld [hl+], a ;height
    ld [hl], snake_height
    inc l
    ld [hl+], a ;visible
    ld [hl], $A0 ;sprite
    inc l
    ld [hl+], a ;grounded
    ld [hl], b ;timer

    ;Return
    ret 
;



; Snake destroy event.
;
; Input:
; - `bc`: Entity pointer (anywhere)
;
; Output:
; - `b`: Free entity (0 = no)
entity_snake_destroy::

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

    ;Return
    ld b, $FF
    ret 
;



; snake execution code.
;
; Input:
; - `de`: Entity variable pointer
entity_snake_execute::

    ;Move pointer to HL
    ld h, d
    ld l, e

    ;Move
        ;Load positions into BC(X) and DE(Y)
        inc l
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        ld c, a
        ld a, [hl+]
        ld d, a
        inc l

        ;Check direction
        ld a, [hl+]
        ld e, a
        bit physics_going_left, e
        jr nz, .checkleft

            ;Entity is going right
            inc b
            ld a, c
            add a, snake_width
            add a, [hl]
            jr c, .checkbottom
            dec b
            jr .checkspeed
        
        .checkleft

            ;Entity is going left
            dec b
            ld a, c
            sub a, [hl]
            jr c, .checkbottom
            inc b
            jr .checkspeed
        ;

        .checkbottom
            
            ;Grab stage pointer in BC
            dec l
            inc d
            coordinates_level b, d, d, e

            ;Check tile properties
            ld c, low(rSVBK)
            ldh a, [c]
            ld b, a
            ld a, bank_foreground
            ldh [c], a
            ld a, [de]
            ld e, a
            ld a, b
            ldh [c], a
            ld d, high(w_blockdata)
            ld a, [de]

            ;Is block solid?
            bit bpb_solid, a
            jr nz, .notsolid
            bit bpb_platform, a
            jr nz, .notsolid

                ;Block is solid
                ;Change direction
                ld a, [hl]
                xor a, 1 << physics_going_left
                ld [hl], a
            .notsolid

            ;Jump to timer
            jr .timer


        .checkspeed

            ;Check if speed is 0
            xor a
            cp a, [hl]

            ;Set speed and point to direction
            ld [hl], snake_walkspeed
            dec hl ;Avoid altering Z-flag

            ;Test previously set Z-flag
            jr nz, :+
                
                ;Switch direction
                ld a, e
                xor a, 1 << physics_going_left
                ld [hl], a
            :

            ;Falls into label `.timer`
        
        .timer

            ;Check if it's time to stop soon
            ld a, l
            ld e, l
            add a, entity_timer - entity_direction
            ld l, a
            inc [hl]
    

    ;Reset pointer
    ld a, l
    sub a, entity_timer - entity_variables
    ld l, a

    ;Do physics n stuff
    call entsys_gravity
    call entsys_physics

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
            ld a, s_snake + 2
            ld [de], a
            inc e
            ld a, OAMF_PAL1 | p_snake
            ld [de], a
            inc e
            inc e
            inc e
            ld a, s_snake + 2
            ld [de], a
            inc e
            ld a, OAMF_PAL1 | p_snake
            ld [de], a

            ;Jump
            jr .visible_regular

        .invisible

            ;Entity should disappear
            ;Free allocated sprite
            inc l
            ld a, [hl]
            ld b, 2
            call sprite_free

            ;Write invalid sprite
            ;Remove for optimization
            ld a, $A0
            ld [hl-], a
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
        inc l
        inc l
        ld b, [hl]

        ;Get player position
        ld a, l
        sub a, entity_timer - entity_state
        ld l, a
        ld c, [hl]
        inc l
        push bc

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
        add a, 4
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
        add a, 10
        ld c, a

        ;Get direction and speed
        ld a, [hl+]
        ld e, a ;Direction
        ld a, [hl+]

        ;Write sprite positions
        ld l, d
        ld d, a
        ld a, c
        ld h, high(w_oam_mirror)
        ld [hl+], a
        ld a, b
        ld [hl+], a
        add a, 8
        inc l
        inc l
        ld [hl], c
        inc l
        ld [hl+], a

        ;Write sprite data
        ld d, s_snake
        pop bc
        ld a, c

        ;Walking sprite
        ld a, b
        and a, %00011000
        rra 
        add a, s_snake
        ld d, a

        ld b, p_snake + OAMF_XFLIP + OAMF_PAL1
        bit physics_going_left, e
        jr nz, :+

            ;Entity is facing right
            inc d
            inc d
            res OAMB_XFLIP, b
        :

        ;Store the data
        ld [hl], d
        inc l
        ld [hl], b
        ld a, l
        sub a, 5
        ld l, a

        dec d
        dec d
        bit physics_going_left, e
        jr z, :+
            inc d
            inc d
            inc d
            inc d
        :

        ld [hl], d
        inc l
        ld [hl], b


    ;Return
    ret
;