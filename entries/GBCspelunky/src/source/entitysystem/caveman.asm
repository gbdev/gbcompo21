INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "entitysystem.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "physics.inc"
INCLUDE "entities/caveman.inc"
INCLUDE "blockproperties.inc"

SECTION "ENTITY CAVEMAN", ROMX, BANK[bank_entities]

;Caveman initialization data
entity_caveman_init::
    db bank_entities ;Bank ID
    dw entity_caveman_execute ;Code address
    db entsys_col_visibleF | entsys_col_enemyF | entsys_col_bounceF | entsys_col_damagableF ;Collision mask
    dw entity_caveman_destroy
    db bank_entities
    dw entity_caveman_create ;Initialization code
;



; Caveman creation code.
;
; Input:
; - `de`: Entity variable pointer
entity_caveman_create::
    
    ;Write state
    ld h, d
    ld l, e
    ld [hl], caveman_state_idle
    inc l

    ;Get a bit of RNG
    call rng_run_single
    ld b, a

    ;Reset physics variables
    xor a
    ld [hl+], a ;x
    ld [hl+], a
    ld [hl+], a ;y
    ld [hl+], a
    ld [hl], b ;direction
    inc l
    ld [hl+], a ;hspp
    ld [hl+], a ;vspp
    ld [hl], caveman_health ;health
    inc l
    ld [hl+], a ;width
    ld [hl], caveman_width
    inc l
    ld [hl+], a ;height
    ld [hl], caveman_height
    inc l
    ld [hl+], a ;visible
    ld [hl], $A0 ;sprite
    inc l
    ld [hl+], a ;grounded
    ld [hl], b ;timer

    ;Return
    ret 
;



; Caveman destroy event.
;
; Input:
; - `bc`: Entity pointer (anywhere)
;
; Output:
; - `b`: Free entity (0 = no)
entity_caveman_destroy::

    ;Get sprite pointer
    ld a, c
    and a, %11000000
    ld l, a
    ld h, b

    ;Try to reallocate to a corpse
    ld bc, entity_corpse_init
    ld a, l
    and a, %11000000
    ld l, a
    call entsys_alloc.realloc

    ;Return
    ld b, 0
    ret 
;



; Caveman execution code.
;
; Input:
; - `de`: Entity variable pointer
entity_caveman_execute::

    ;Move pointer to HL
    ld h, d
    ld l, e

    ;Call state machine
    call caveman_states

    ;Reset pointer
    ld a, l
    and a, %11000000
    or a, entity_variables
    ld l, a

    ;Do physics n stuff
    call entsys_gravity
    ld a, [hl]
    cp a, gstate_tumble
    jr z, :+
        call entsys_physics
        rra ;Clear fz
    :
    call z, entsys_physics_tumble

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
            ld a, s_caveman_idle
            ld [de], a
            inc e
            ld a, OAMF_PAL1 | 2
            ld [de], a
            inc e
            inc e
            inc e
            ld a, s_caveman_idle+2
            ld [de], a
            inc e
            ld a, OAMF_PAL1 | 2
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

        ;A little bit of behaviour culling
        ;Check player in front, agro activation
            
            ;Move pointer to state
            ld a, l
            ld e, l
            sub a, entity_visible - entity_state
            ld l, a
            ld a, [hl+]

            ;Check if I'm in a valid state
            cp a, caveman_state_idle
            jr z, .goodstate
            cp a, caveman_state_walk
            jr z, .goodstate

            ;Bad state
            jr .badstate

            .goodstate
            ;Some good register shuffling
            ld a, l
            ld l, e
            push hl
            ld l, a

            ;Load positions
            ld a, [hl+]
            ld b, a
            ld a, [hl+]
            ld c, a
            ld a, [hl+]
            ld d, a
            ld a, [hl+]
            add a, $20
            ld e, a

            ;Offset X-position maybe
            bit physics_going_left, [hl]
            jr z, :+

                ld a, b
                sub a, 4
                ld b, a
            :

            ;Write these to rectangle buffer
            ld hl, w_collision_buffer
            ld a, b
            ld [hl+], a
            ld a, c
            ld [hl+], a
            ld a, d
            ld [hl+], a
            ld a, e
            ld [hl+], a

            ;Add offsets
            ld a, c
            add a, caveman_width
            ld c, a
            ld a, b
            adc a, 4
            ld b, a
            ld a, e
            add a, caveman_height - $20 - $10
            ld e, a
            jr nc, :+
            inc d
            :

            ;Write more stuff
            ld a, b
            ld [hl+], a
            ld a, c
            ld [hl+], a
            ld a, d
            ld [hl+], a
            ld a, e
            ld [hl+], a

            ;Check collision
            ld bc, w_collision_buffer
            call entsys_collision_RP
            pop hl
            ld e, l
            jr z, :+

                ;Collision was found! Enter agro state
                ld a, l
                sub a, entity_visible - entity_state
                ld l, a
                ld [hl], caveman_state_angry
                add a, entity_hspp - entity_state
                ld l, a
                ld [hl], caveman_runspeed
            :

        .badstate
        ;Restore pointer
        ld l, e


        ;Entity is visible, show that sprite!
        ;Grab sprite ID
        inc l
        ld d, [hl]
        inc l
        inc l
        ld b, [hl]

        ;Get entity position
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
        add a, 11
        ld c, a

        ;Get direction and speed
        ld e, [hl] ;Direction
        ld a, l
        sub a, entity_direction - entity_state
        ld l, a
        ld a, [hl]

        ;Is entity in tumble state?
        cp a, gstate_tumble
        jr nz, .noshake
            
            ;Yea, check timer
            ld a, l
            add a, entity_timer - entity_state
            ld l, a
            ld a, [hl]
            cp a, $40
            jr nc, .noshake_sub

                ;Shake a little
                bit 2, a
                jr z, :+
                    dec b
                    jr .noshake_sub
                :
                    inc b
                    ;Falls into label `.noshake_sub`

            ;Move HL back in place
            .noshake_sub
            ld a, l
            sub a, entity_timer - entity_state
            ld l, a

        .noshake
        
        ;Get grounded variable
        add a, entity_grounded - entity_state
        ld l, a
        ld a, [hl] ;Grounded

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
        pop bc
        ld a, c
        cp a, gstate_tumble
        jr nz, .notumble

            ;Tumble state
            ld a, d
            ld d, s_caveman_dead
            cp a, 0
            jr nz, .spriteplace
            ld d, s_caveman_tumble
            jr .spriteplace

        
        .notumble
        ld d, s_caveman_idle
        cp a, caveman_state_idle
        jr z, .spriteplace

            ;Walking sprite
            ld a, b
            cpl 
            and a, %00001100
            add a, s_caveman_walk
            ld d, a
            jr .spriteplace

        .spriteplace
        ld b, 2 + OAMF_XFLIP + OAMF_PAL1
        bit physics_going_left, e
        jr nz, :+

            ;Player is facing right
            inc d
            inc d
            res OAMB_XFLIP, b
        :

        ;Load the data
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



; Handles the different caveman states.
;
; Input:
; - `hl`: Variable pointer
caveman_states:

    ;Check state
    ld a, [hl+]
    cp a, caveman_state_idle
    jr z, .idle
    cp a, caveman_state_walk
    jr z, .walk
    cp a, caveman_state_angry
    jp z, .angry
    cp a, gstate_tumble
    jp z, .tumble
    
    ;Unknown state, break and stuff
    ld b, b
    ret 

    ;Caveman is idle
    .idle

        ;Am I grounded?
        ld a, l
        ld e, l
        add a, $0E
        ld l, a
        ld [hl+], a
        cp a, 0
        jr z, :+

            ;Yes, I am grounded
            ;Tick down timer
            dec [hl]
            jr nz, :+

                ;Timer reached 0
                ;Set walk timer and switch state
                ld a, caveman_walktime
                ld [hl], a
                ld l, e
                dec l
                ld [hl], caveman_state_walk

                ;Also reset speed
                ld a, l
                add a, entity_hspp - entity_state
                ld l, a
                ld [hl], caveman_walkspeed
        :

        ;Return
        ret
    
    .walk

        ;Load positions into BC(X) and DE(Y)
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
            add a, caveman_width
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
            ld [hl], caveman_walkspeed
            dec hl ;Done to avoid altering Z flag

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
            dec [hl]
            ret nz
                
                ;Timer reached 0
                ;Set idle timer and switch state
                ld a, caveman_idletime
                ld [hl], a
                ld a, e
                sub a, entity_direction - entity_state
                ld l, a
                ld [hl], caveman_state_idle

                ;Also reset speed
                ld a, l
                add a, entity_hspp - entity_state
                ld l, a
                ld [hl], 0

                ;Return
                ret
        ;
    .angry

        ;Load positions into BC(X) and DE(Y)
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        ld c, a
        ld a, [hl+]
        ld d, a
        inc l

        ;Grab direction
        ld a, [hl+]
        ld e, a

        ;Check if hspeed is 0
        xor a
        cp a, [hl]

        ;Set hspeed and point to direction
        ld [hl], caveman_runspeed
        dec hl ;Done to avoid altering Z flag

        ;Test previously set Z-flag
        jr nz, :+
            
            ;Switch direction
            ld a, e
            xor a, 1 << physics_going_left
            ld [hl], a
        :

        ;Animation timer
        ld a, l
        add a, entity_timer - entity_direction
        ld l, a
        dec [hl]
        dec [hl]
            
        ;Return
        ret
    
    .tumble

        ;Decrement timer
        dec l
        ld a, l
        ld e, l
        add a, entity_timer - entity_state
        ld l, a
        dec [hl]

        ;Did timer reach 0?
        ret nz

        ;It DID reach 0!
        ;Set state to walking again
        ld [hl], caveman_idletime
        ld l, e
        ld [hl], caveman_state_idle

        ;Reenable enemy collision mask
        sub a, entity_timer - (entity_collision+1)
        ld l, a
        set entsys_col_enemyB, [hl]

        ;Return
        ret 
    
    .bounce
        
        ;Make the guy bounce!
        ld a, l
        and a, %11000000
        or a, entity_hspp
        ld l, a

        ld a, [hl]
        cp a, 0
        ld [hl], 6
        jr nz, :+

            ;Change direction
            dec l
            ld a, 1 << physics_going_left
            xor a, [hl]
            ld [hl+], a
        :

        inc l
        ld a, [hl]
        cp a, 0
        ld [hl], 4
        jr nz, :+
            
            ;Change direction
            dec l
            dec l
            ld a, 1 << physics_going_up
            xor a, [hl]
            ld [hl+], a
            inc l
        :
        ld a, l
        sub a, 7
        ld l, a

        ret 
    ;
;