INCLUDE "hardware.inc"
INCLUDE "player.inc"
INCLUDE "blockproperties.inc"
INCLUDE "banks.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "physics.inc"
INCLUDE "entitysystem.inc"

SECTION "PLAYER STATES", ROMX, BANK[bank_player]

; Handles player states.
; Exists in it's own file because I just don't want 
; to have a single file with 1000+ lines of code.
player_states::
    
    ;Do a state machine
    ld a, [w_player_state]
    cp a, pstate_default
    jp z, statecode_default

    cp a, pstate_ledge
    jp z, statecode_ledge
    cp a, pstate_whip
    jp z, statecode_whip
    cp a, pstate_ladder
    jp z, statecode_climbing

    cp a, pstate_stun
    jp z, statecode_stunned

    ;State doesn't match anything, breakpoint
    ld b, b
    ret 
;



; Default state.
; Platoforming and stuff.
statecode_default:
    
    ;Grab input and hspp
    ;HL points to direction
    ldh a, [h_input]
    ld b, a
    ld hl, w_player_hspp
    
    ;Is player grounded?
    ld a, [w_player_grounded]
    cp a, player_jumptimer
    jr z, :+

        ;Player is in the air
        call player_animate_jump
        jr .animate_done
    :

        ;Player is grounded
        ld a, [hl]
        cp a, 0
        jr nz, :+

            ;Player is standing still
            call player_animate_idle
            jr .animate_done
        :
            ;Player is moving
            call player_animate_walk
            ;Falls into label

    .animate_done
    ;Horizontal movement
    ;Is player not moving?
    ld a, [hl-]
    cp a, 0
    jr nz, .movingH

        ;Player is not moving, set direction according to input
        bit PADB_LEFT, b
        jr z, :+

            ;Player is moving left
            set player_going_left, [hl]
            set player_facing_left, [hl]
            jr .inchspp
        :
        bit PADB_RIGHT, b
        jr z, .storehspp
            
        ;Player is moving right
            res player_going_left, [hl]
            res player_facing_left, [hl]
            jr .inchspp
        
        jr .storehspp
    ;

    .movingH
    ;Friction
    sub a, player_friction
    jr nc, :+
        xor a
    :

    ;Is player moving left?
    bit player_going_left, [hl]
    jr z, :+
        
        ;Player is moving left
        set player_facing_left, [hl]
        bit PADB_LEFT, b
        jr nz, .inchspp
        bit PADB_RIGHT, b
        jr nz, .dechspp
        jr .storehspp
    :
        
        ;Player is moving right
        res player_facing_left, [hl]
        bit PADB_RIGHT, b
        jr nz, .inchspp
        bit PADB_LEFT, b
        jr nz, .dechspp
        jr .storehspp
    :

    ;Move faster
    .inchspp
        add a, player_accel
        cp a, player_maxspeed+1
        jr c, :+
        ld a, player_maxspeed
        :
        jr .storehspp
    
    ;Slow down
    .dechspp
        sub a, player_accel
        jr nc, :+
            xor a
        :
        ;Falls into label `.storehspp`.


    ;Store hspp
    .storehspp
        inc l
        ld [hl-], a
    ;


    ;Gravity
    bit player_going_up, [hl]
    jr z, :+

        ;Player is moving up, subtract speed
        inc l
        inc l
        ld a, [hl]
        sub a, player_gravity
        ld [hl], a
        jr nc, .donegravity

            ;Player is now moving down
            xor a
            ld [hl], a
            dec l
            dec l
            res player_going_up, [hl]
            inc l
            inc l
            jr .donegravity
    :
        ;Player is moving down, add speed
        inc l
        inc l
        ld a, [hl]
        add a, player_gravity
        cp a, player_maxspeed_fall
        jr c, :+
            ld a, player_maxspeed_fall
        :
        ld [hl], a
    .donegravity

    ;Jumping
    ld de, w_player_grounded
    ld a, [de]

    ;Has player not left the ground yet?
    cp a, player_jumptimer
    jr nz, .jumpprogress

        ;Initialize jump
        ld c, a
        ldh a, [h_input_pressed]
        bit PADB_A, a
        ld a, c
        jr z, .nojump
        bit PADB_DOWN, b
        jr nz, .crawl
        ld [hl], player_jump_force
        jr .dojump

        ;Crawl, drop through platform
        .crawl

            push hl
            push de

            ;Create pointer to stage data BELOW the player
            ld l, 0
            ld a, [w_player_y]
            inc a
            rra
            rr l
            rra 
            rr l
            add a, high(level_foreground)
            ld h, a
            ld a, [w_player_x]
            add a, l
            ld l, a

            ;Is this a platform?
            ld d, high(w_blockdata)
            ld e, [hl]
            ld a, [de]
            bit bpb_solid, a
            jr nz, .dropthrough_end

            ;This is a platform, check for tile overlap
            ld a, [w_player_x+1]
            add a, player_width
            jr nc, .dropthrough_go
            inc l
            ld e, [hl]
            ld a, [de]
            bit bpb_solid, a
            jr nz, .dropthrough_end

            ;Platform secured
            .dropthrough_go
                ld hl, w_player_y+1
                inc [hl]

            ;Return from this nonsense
            .dropthrough_end
                pop de
                pop hl
                jr .nojump


    .jumpprogress
    bit PADB_A, b
    jr z, .nojump

        ;The funny jump button was pressed
        .dojump
        dec a
        jr z, .nojump

            ;Player is grounded
            ;Remove gravity
            ld [de], a
            ld a, [hl]
            add a, player_gravity
            ld [hl], a
            dec l
            dec l
            set player_going_up, [hl]
            jr .postjump


    .nojump
    ;Set grounded variable to 0
    ld a, 1
    ld [de], a
    .postjump

    ;Whip
    ldh a, [h_input_pressed]
    bit PADB_B, a
    jr z, .nowhip
        
        ;Point HL to w_player_state
        ld l, low(w_player_state)
        ld a, [hl]

        ;Is this called from the whip state?
        cp a, pstate_whip
        jr z, .nowhip

            ;This is NOT called from the whip state
            ld [hl], pstate_whip
            ld a, player_whip_timer
            ld [w_player_animation], a

            ;Reset whip state
            ld l, low(w_player_direction)
            res player_whip_state, [hl]
            jr .postwhip

    .nowhip

        ;Load player position+offset into BC+DE
        ld l, low(w_player_x)
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        add a, player_width >> 1
        jr nc, :+
        inc b
        :
        ld e, b
        ld a, [hl+]
        ld d, a
        inc l

        coordinates_level b, d, b, c

        ld a, [bc]
        ld d, high(w_blockdata)
        ld e, a
        ld a, [de]
        bit bpb_ladder, a
        jr z, .noladder    

            ;Is up or down pressed?
            ldh a, [h_input]
            bit PADB_UP, a
            jr nz, :+
            bit PADB_DOWN, a
            jr z, .postwhip
            :

            ;If it was initiated with down, make sure it's valid
            bit PADB_UP, a
            jr nz, :++

                ;Is tile below a platform?
                ld a, c
                add a, 64
                ld c, a
                jr nc, :+
                inc b
                :
                ld a, [bc]
                ld e, a
                ld a, [de]
                bit bpb_platform, a
                jr z, .postwhip

                ;Check Y-sub position
                dec l
                ld a, [hl+]
                cp a, 256 - player_height
                jr nz, .postwhip

                ;Check vspp
                inc l
                inc l
                ld a, [hl-]
                dec l
                cp a, player_gravity
                jr nz, .postwhip

                ld a, c
                sub a, 64
                jr nc, :+
                dec b
            :

            ;Thing?
            bit player_laddertouch, [hl]
            jr z, :+

                bit player_going_up, [hl]
                jr nz, .postwhip
            :
            
            ;Set laddertouch flag
            set player_laddertouch, [hl]

            ;Set speed in both directins to 0
            inc l
            xor a
            ld [hl+], a
            ld [hl+], a
            
            ;Switch state
            ld l, low(w_player_state)
            ld [hl], pstate_ladder

            ;Set X-sub position
            inc l
            ld a, c
            and a, %00111111
            ld [hl+], a
            ld [hl], 128 - (player_width >> 1)
            jr .postwhip


    .noladder
    ld l, low(w_player_direction)
    res player_laddertouch, [hl]

    .postwhip
    ret 
;



; Ledge-hanging state.
; Jump and whip and stuff.
statecode_ledge:
    
    ;Animate
    call player_animate_hang
    
    ;Get input
    ldh a, [h_input_pressed]
    bit PADB_B, a
    jr nz, .whip
    bit PADB_A, a
    ret z

    ;Restore player state
    ld a, pstate_default
    ld [w_player_state], a

    ;Is down being held?
    ldh a, [h_input]
    bit PADB_DOWN, a
    ret nz

    ;Down is not held, initialize a jump
    ld hl, w_player_direction
    set player_going_up, [hl]
    inc l
    inc l
    ld a, player_jump_force
    ld [hl], a
    ld a, player_jumptimer - 1
    ld [w_player_grounded], a
    ret

    ;Player is whipping, push away from wall
    .whip
        
        ;Reverse direction
        ld hl, w_player_direction
        ld a, 1 << player_going_left
        xor a, [hl]
        ld [hl+], a

        ;Set hspeed to $30
        ld a, $60
        ld [hl], a

        ;Initiate whip state
        ld a, pstate_whip
        ld [w_player_state], a
        ld a, player_whip_timer
        ld [w_player_animation], a

        ;Reset whip state
        ld l, low(w_player_direction)
        res player_whip_state, [hl]

        ;Return
        ret 
;



; Whipping state.
; Normal platforming, but ignore ledges and don't change facing direction.
statecode_whip:

    ;Figure out what frame to display
    ld hl, w_player_animation
    dec [hl]
    ld a, [hl]
    cp a, 0
    jr nz, .notzero

        ;Time is up, revert to the default state
        ld a, pstate_default
        ld [w_player_state], a

        ;Reset all the funny business I did
        ld a, bank_entsys
        ldh [rSVBK], a
        ld de, 64
        ld hl, w_entsys_collision+1
        ld b, 63
        :

            ;Reset whip-states
            dec l
            ld a, [hl+]
            cp a, _ldan8
            jr nz, :+
            res entsys_col_whipB, [hl]
            :
            add hl, de
            dec b
            jr nz, :--
        
        ld a, bank_foreground
        ldh [rSVBK], a
        
        jp statecode_default
    .notzero

    ;Figure out what frame to play
    ld b, panim_whip + 4
    cp a, player_whip_timer-13
    jr c, .animate
    dec b
    cp a, player_whip_timer-10
    jr c, .animate
    dec b
    cp a, player_whip_timer-6
    jr c, .animate
    dec b
    cp a, player_whip_timer-3
    jr c, .animate
    dec b
    cp a, player_whip_timer-1
    jr c, .animate
    dec b

    ;Play the frame
    .animate
    call player_animate_frame

    ;If timer is a certain number, change whip state
    cp a, player_whip_timer-10
    jr nz, :+
        ld l, low(w_player_direction)
        set player_whip_state, [hl]
    :


    ;Grab input and hspp
    ;HL points to direction
    ldh a, [h_input]
    ld b, a
    ld hl, w_player_hspp

    ;Horizontal movement
    ;Is player not moving?
    ld a, [hl-]
    cp a, 0
    jr nz, .movingH

        ;Player is not moving, set direction according to input
        bit PADB_LEFT, b
        jr z, :+

            ;Player is moving left
            set player_going_left, [hl]
            jr .inchspp
        :
        bit PADB_RIGHT, b
        jr z, .storehspp
            
        ;Player is moving right
            res player_going_left, [hl]
            jr .inchspp
        
        jr .storehspp
    ;

    .movingH
    ;Friction
    sub a, player_friction
    jr nc, :+
        xor a
    :

    ;Is player moving left?
    bit player_going_left, [hl]
    jr z, :+
        
        ;Player is moving left
        bit PADB_LEFT, b
        jr nz, .inchspp
        bit PADB_RIGHT, b
        jr nz, .dechspp
        jr .storehspp
    :
        
        ;Player is moving right
        bit PADB_RIGHT, b
        jr nz, .inchspp
        bit PADB_LEFT, b
        jr nz, .dechspp
        jr .storehspp
    :

    ;Move faster
    .inchspp
        add a, player_accel
        cp a, player_maxspeed+1
        jr c, :+
        ld a, player_maxspeed
        :
        jr .storehspp
    
    ;Slow down
    .dechspp
        sub a, player_accel
        jr nc, :+
            xor a
        :
        jr .storehspp


    ;Store hspp
    .storehspp
        inc l
        ld [hl-], a
    ;


    ;Gravity
    bit player_going_up, [hl]
    jr z, :+

        ;Player is moving up, subtract speed
        inc l
        inc l
        ld a, [hl]
        sub a, player_gravity
        ld [hl], a
        jr nc, .donegravity

            ;Player is now moving down
            xor a
            ld [hl], a
            dec l
            dec l
            res player_going_up, [hl]
            inc l
            inc l
            jr .donegravity
    :
        ;Player is moving down, add speed
        inc l
        inc l
        ld a, [hl]
        add a, player_gravity
        cp a, player_maxspeed_fall
        jr c, :+
            ld a, player_maxspeed_fall
        :
        ld [hl], a
    .donegravity

    ;Jumping
    ld de, w_player_grounded
    ld a, [de]

    ;Has player not left the ground yet?
    cp a, player_jumptimer
    jr nz, .jumpprogress

        ;Initialize jump
        ld c, a
        ldh a, [h_input_pressed]
        bit PADB_A, a
        ld a, c
        jr z, .nojump
        bit PADB_DOWN, b
        jr nz, .crawl
        ld [hl], player_jump_force
        jr .dojump

        ;Crawl, drop through platform
        .crawl

            push hl
            push de

            ;Create pointer to stage data BELOW the player
            ld l, 0
            ld a, [w_player_y]
            inc a
            rra
            rr l
            rra 
            rr l
            add a, high(level_foreground)
            ld h, a
            ld a, [w_player_x]
            add a, l
            ld l, a

            ;Is this a platform?
            ld d, high(w_blockdata)
            ld e, [hl]
            ld a, [de]
            bit bpb_solid, a
            jr nz, .dropthrough_end

            ;This is a platform, check for tile overlap
            ld a, [w_player_x+1]
            add a, player_width
            jr nc, .dropthrough_go
            inc l
            ld e, [hl]
            ld a, [de]
            bit bpb_solid, a
            jr nz, .dropthrough_end

            ;Platform secured
            .dropthrough_go
                ld hl, w_player_y+1
                inc [hl]

            ;Return from this nonsense
            .dropthrough_end
                pop de
                pop hl
                jr .nojump


    .jumpprogress
    bit PADB_A, b
    jr z, .nojump

        ;The jump button was pressed
        .dojump
        dec a
        jr z, .nojump

            ;Player is grounded
            ;Set VSPP to jump speed
            ld [de], a
            ld a, [hl]
            add a, player_gravity
            ld [hl], a
            dec l
            dec l
            set player_going_up, [hl]
            jr .jumping_post


    .nojump
    ;Set grounded variable to 0
    ld a, 1
    ld [de], a
    .jumping_post

    ;Check for whip collision with destructible tiles
    ;Grab player location
    ld l, low(w_player_x)
    ld b, [hl]
    inc l
    ld c, [hl]
    inc l
    ld d, [hl]
    inc l
    ld e, [hl]
    inc l

    ;Add a bit of an offset to the height
    ld a, e
    add a, low(player_height - $80)
    ld e, a
    jr nc, :+
    inc d
    :

    ;Offset X-position a little
    ld a, c
    add a, player_width >> 1
    ld c, a
    jr nc, :+
    inc b
    :

    bit player_whip_state, [hl]
    jr nz, .forwardwhip

        ;Backwhip, place offset differently
        ld a, e
        sub a, $8F
        ld e, a
        jr nc, :+
        dec d
        :

        ld a, c
        bit player_facing_left, [hl]
        jr z, .bwright

            ;Player is facing left
            add a, $80
            jr nc, .forwardwhip
            inc b
            jr .forwardwhip
        .bwright

            ;Player is facing right
            sub a, $80
            jr nc, .forwardwhip
            dec b
            ;Falls into label

    .forwardwhip
    
    ;w_player_direction -> E
    ld c, [hl]

    ;Create stage pointer
    xor a
    ld l, a
    ld a, d
    rra
    rr l
    rra 
    rr l
    add a, high(level_foreground)
    ld h, a
    ld a, b
    add a, l
    ld l, a
    ld b, e

    .loopies

        ;Check tile properties
        ld d, high(w_blockdata)
        ld e, [hl]
        ld a, [de]
        bit bpb_destructible, a
        jr z, :+
            ld [hl], b_bg
            call map_update_block
        :

        ;Go either left or right
        bit player_facing_left, c
        jr z, :+

            ;Facing left
            dec l
            dec l
        :
        inc l

        ;Check tile properties
        ld d, high(w_blockdata)
        ld e, [hl]
        ld a, [de]
        bit bpb_destructible, a
        jr z, :+
            ld [hl], b_bg
            call map_update_block
        :
        
        ;Small hitbox overlap
        ld a, b
        add a, $20
        ld b, a
        jr nc, .postblocks

        ld a, l
        add a, 64
        ld l, a
        jr nc, :+
        inc h
        :

        ;Go either left or right
        bit player_facing_left, c
        jr z, :+

            ;Facing left
            inc l
            inc l
        :
        dec l
        jr .loopies
    
    .postblocks

    ;Check for entity collisions :)
    ;Set up a collision buffer
    ld hl, w_player_y+1
    ld a, [hl-]
    ld e, a
    ld a, [hl-]
    ld d, a
    ld a, [hl-]
    add a, player_width >> 1
    ld c, a
    ld b, [hl]
    jr nc, :+
    inc b
    :

    ld l, low(w_player_direction)
    bit player_whip_state, [hl]
    jr z, .backwhip

        ;Normal whip
        ;Move whip down a bit
        ld a, e
        add a, $28
        ld e, a
        jr nc, :+
        inc d
        :
        jr .horplace

    .backwhip

        ;Backwhip
        ;Move whip up a bit
        ld a, e
        sub a, $80
        ld e, a
        jr nc, :+
        dec d
        :
        ;Falls into label `.horplace`.

    ;Place whip X-position
    .horplace
    ld a, [hl]
    bit player_facing_left, a
    jr z, .wright
    .wleft

        ;Backwhip, make sure
        bit player_whip_state, a
        jr z, .wright1
        
        ;Facing left
        ;Write Y1 + X2
        .wleft1
        ld hl, w_collision_buffer+2
        ld [hl], d
        inc l
        ld [hl], e
        inc l
        ld [hl], b
        inc l
        ld [hl], c

        ;Place second X-coordinate
        dec b

        ld l, low(w_collision_buffer)
        ld [hl], b
        inc l
        ld [hl], c
        
        jr .wdone

    .wright

        ;Make sure there are no backwhip shenanigans
        bit player_whip_state, a
        jr z, .wleft1

        ;Facing right
        ;Write Y1 + X1
        .wright1
        ld hl, w_collision_buffer
        ld [hl], b
        inc l
        ld [hl], c
        inc l
        ld [hl], d
        inc l
        ld [hl], e

        ;Place second X-coordinate
        inc b
        inc l
        ld [hl], b
        inc l
        ld [hl], c
        ;Falls into label `.wdone`.
    .wdone

    ;Write Y2
    ld l, low(w_collision_buffer + 6)
    ld a, e
    add a, $80
    ld e, a
    jr nc, :+
    inc d
    :
    ld [hl], d
    inc l
    ld [hl], e

    ;Prepare collision loop
    ;Switch RAM bank
    ld a, bank_entsys
    ldh [rSVBK], a
    ld hl, w_entsys_collision

    .collisionloop

        ;Get a valid entity
        ld c, entsys_col_visibleF | entsys_col_damagableF | entsys_col_enemyF | entsys_col_whipF
        call _hl_
        ld b, a
        ld a, d
        cp a, $FF
        jr z, .postcol

        ;Make sure enemy is, you know, visible
        ;And also whipable
        bit entsys_col_visibleB, b
        jr z, .collisionloop
        bit entsys_col_whipB, b
        jr nz, .collisionloop

        ;Boxcheck
        push bc
        push de
        push hl
        ld bc, w_collision_buffer
        call entsys_collision_RE
        pop hl
        pop de
        pop bc
        jr z, .collisionloop

            ;THERE BE COLLISION!!!
            push hl
            ld h, d
            ld a, e
            sub a, entity_state - (entity_collision+1)
            ld l, a
            set entsys_col_whipB, [hl]
            add a, entity_direction - (entity_collision+1)
            ld l, a
            
            ;Set entity direction
            ld a, [w_player_direction]
            ld c, a
            xor a
            bit player_facing_left, c
            jr z, :+
            inc a
            :
            bit player_whip_state, c
            jr nz, :+
            inc a
            :
            res physics_going_left, [hl]
            bit 0, a
            jr z, :+
            set physics_going_left, [hl]
            :

            ;Set entity pointer to start of entity
            ld a, e
            and a, %11000000
            ld e, a
            
            ;Can this entity take damage?
            bit entsys_col_damagableB, b
            jr z, :+
            bit entsys_col_enemyB, b
            jr z, :+
                
                ;Deal damage
                ld c, 1
                call nz, entsys_damage
                jr :++
            :
                ;Set Hspeed and Vspeed
                set physics_going_up, [hl]
                inc l
                ld [hl], physics_bounce_x
                inc l
                ld [hl], physics_bounce_y
            :

            ;Pop and return to loop
            pop hl
            jp .collisionloop


    ;Return
    .postcol
    ld a, bank_foreground
    ldh [rSVBK], a
    ret 

    .postbox
;



; Climbing state.
; Used for both ladders and ropes.
; Ropes use a differet spriteset tough.
statecode_climbing:
    
    ld hl, w_player_vspp
    ld [hl], 0
    ldh a, [h_input]
    ld b, a
    bit PADB_UP, b
    jr z, .checkdown

        ;Make sure tile at position above is a ladder
        ld l, low(w_player_x)
        ld a, [hl+]
        ld d, a
        inc l
        ld a, [hl+]
        ld c, a
        ld a, [hl+]

        sub a, player_ladder_speed
        jr nc, :+
        dec c
        :

        ;Turn into a pointer
        coordinates_level d, c, d, e

        ld a, [de]
        ld e, a
        ld d, high(w_blockdata)
        ld a, [de]
        bit bpb_ladder, a
        jr nz, :+

            ;Set player Y-position
            dec l
            ld [hl], 0
            jr .nodirection
        :

        set player_going_up, [hl]
        inc l
        inc l
        ld [hl], player_ladder_speed
        
        jr .nodirection


    .checkdown
    bit PADB_DOWN, b
    jr z, .nodirection
        
        ;Make sure tile at position above is a ladder
        ld l, low(w_player_x)
        ld a, [hl+]
        ld d, a
        inc l
        ld a, [hl+]
        ld c, a
        ld a, [hl+]

        add a, player_height + player_ladder_speed
        jr nc, :+
        inc c
        :

        ;Turn into a pointer
        coordinates_level d, c, d, e

        ld a, [de]
        ld e, a
        ld d, high(w_blockdata)
        ld a, [de]
        bit bpb_ladder, a
        jr nz, :+

            ;Set player Y-position
            dec l
            ld [hl], 256 - player_height
            bit bpb_solid, a
            jr z, .nodirection
                
                ;Block below is solid, revert to default state
                ld e, l
                ld l, low(w_player_state)
                ld [hl], pstate_default
                ld l, e
                jr .nodirection
        :

        res player_going_up, [hl]
        inc l
        inc l
        ld [hl], player_ladder_speed
        ;Falls into label `.nodirection`

    .nodirection

    ;Animate
    call player_animate_ladder

    ;Get input
    ldh a, [h_input_pressed]
    bit PADB_A, a
    ret z

    ;Restore player state
    ld a, pstate_default
    ld [w_player_state], a

    ;Is down being held?
    ldh a, [h_input]
    bit PADB_DOWN, a
    ret nz

    ;Down is not held, initialize a jump
    ld hl, w_player_direction
    set player_going_up, [hl]
    inc l
    inc l
    ld a, player_jump_force
    ld [hl], a
    ld a, player_jumptimer - 1
    ld [w_player_grounded], a

    ;Return
    ret 
;



; Stunned state.
; Used when taking damage.
; Goes back to neutral state when animation counter reaches 0.
statecode_stunned:
    
    ;Animate a bit
    ld b, panim_jump_begin
    call player_animate_frame
    
    ;Gravity
    ld hl, w_player_direction
    bit player_going_up, [hl]
    jr z, :+

        ;Player is moving up, subtract speed
        inc l
        inc l
        ld a, [hl]
        sub a, player_gravity
        ld [hl], a
        jr nc, .donegravity

            ;Player is now moving down
            xor a
            ld [hl], a
            dec l
            dec l
            res player_going_up, [hl]
            inc l
            inc l
            jr .donegravity
    :
        ;Player is moving down, add speed
        inc l
        inc l
        ld a, [hl]
        add a, player_gravity
        cp a, player_maxspeed_fall
        jr c, :+
            ld a, player_maxspeed_fall
        :
        ld [hl], a
    .donegravity

    ;Slowly subtract hspeed
    ld l, low(w_player_hspp)
    ld a, [hl]
    sub a, player_stun_friction
    jr nc, :+
    xor a
    :
    ld [hl], a

    ;Decrement stun timer
    ld l, low(w_player_animation)
    dec [hl]
    ret nz

    ;Timer reached 0
    ld l, low(w_player_state)
    ld [hl], pstate_default

    ;Return
    ret
;