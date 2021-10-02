INCLUDE "hardware.inc"
INCLUDE "color.inc"
INCLUDE "player.inc"
INCLUDE "blockproperties.inc"
INCLUDE "entitysystem.inc"
INCLUDE "entities/treasure.inc"
INCLUDE "banks.inc"

INCLUDE "physics.inc"
INCLUDE "dwellings/tileid.inc"

SECTION "PLAYER", ROMX, BANK[bank_player]

; Prepares a platformer player.
; Assumes entrance variables are set correctly.
player_init::
    
    ;Allocate sprites
    ld a, 4
    call sprite_alloc_multi
    ld [w_player_sprite], a

    ;Load in whip tiles
    ld hl, $8040
    ld bc, player_sprite_whip
    ld de, player_sprite_whip.end - player_sprite_whip
    call memcopy
    
    ;Grab entrance X/Y coordinates
    ld a, [w_level_entrance_x]
    ld [w_player_x], a
    ld a, [w_level_entrance_y]
    dec a
    ld [w_player_y], a

    ;Copy player palettes
    xor a
    ld hl, player_palette
    call palette_copy_spr
    call palette_copy_spr
    call palette_copy_spr
    call palette_copy_spr
    call palette_copy_spr
    call palette_copy_spr
    call palette_copy_spr
    call palette_copy_spr 

    ;Return
    ret
;



; Test for a platformer engine
; 
; Destroys: all(probably)
player::

    ;Decrement invincibility
    ld hl, w_player_invincible
    ld a, [hl]
    cp a, 0
    jr z, :+
    dec [hl]
    :
    
    ;Switch RAM bank to active layer
    ld a, [w_layer_bank]
    ldh [rSVBK], a

    ;Player state
    ;call player_animate_walk
    call player_states

    ;Horizontal collision handling
    .horizontal
        ;Grab player X-position to BC
        ld hl, w_player_x
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        ld c, a

        ;Grab Y-position to DE and push it
        ld a, [hl+]
        ld d, a
        ld a, [hl+]
        ld e, a
        push de

        ;Offset x-position it a bit if moving right
        ld a, [hl+]
        bit player_going_left, a
        jr nz, :++

            ;Player is moving right, add offset + hspeed
            ld a, player_width
            add a, [hl]
            jr nc, :+
            inc b
            :
            add a, c
            ld c, a
            jr nc, :++
            inc b
            jr :++
        :
            ;Player is moving left, subtract hspeed
            ld a, c
            sub a, [hl]
            ld c, a
            jr nc, :+
            dec b
        :

        ;Push this X-position and pointer to player data
        push bc
        push hl

        ;Turn this into a pointer to stage data in HL
        xor a ;Resets carry flag
        ld c, a
        rr d
        rr c
        rr d
        rr c
        ld a, b
        add a, c
        ld l, a
        ld a, d
        add a, high(level_foreground)
        ld h, a

        ;Is this block solid?
        ld c, [hl]
        ld b, high(w_blockdata)
        ld a, [bc]
        bit bpb_solid, a
        jr nz, .Hcollision

            ;Block is not solid, check if there is a tile overlap
            ;Is player in the middle of 2 blocks?
            ld a, e
            add a, 256 - player_width
            jr nc, .add_hspp

                ;There is a block overlap, check new block
                ld a, l
                add a, 64
                jr nc, :+
                inc h
                :
                ld l, a
                ld c, [hl]
                ld a, [bc]
                bit bpb_solid, a
                jr nz, .Hcollision

                ;No collision
                jr .add_hspp
        ;


        ;Collision was found!
        .Hcollision

            ;Set speed to 0
            ld a, l
            pop hl
            ld [hl], 0

            ;Get direction
            dec l
            bit player_going_left, [hl]
            jr nz, .goingleft

                ;Player is moving right
                ;Set block coordinate
                dec l
                dec l
                dec l
                dec l
                and a, %00111111
                dec a
                ld [hl+], a

                ;Set subblock coordinate
                ld [hl], 256 - player_width - 1

                ;Load this into BC
                pop bc
                ld b, a
                ld c, [hl]
                push bc

                ;Jump
                jr .vertical
            
            .goingleft
                ;Player is moving left
                ;Set block coordinate
                dec l
                dec l
                dec l
                dec l
                and a, %00111111
                inc a
                ld [hl+], a

                ;Set subblock coordinate
                ld [hl], 0

                ;Load this into BC
                pop bc
                ld b, a
                ld c, [hl]
                push bc

                ;Jump
                jr .vertical
        ;

        ;Add hspp to player X-position
        .add_hspp
            
        ;Load modified X-position into the WRAM variable
        
            ;Retrieve pointer and X-position itself from stack
            pop hl
            pop bc

            ;Move pointer to X-position
            dec l
            ld a, [hl-]
            dec l
            dec l
            dec l

            ;If moving right, subtract player width
            bit player_going_left, a
            jr nz, :+
                ld a, c
                sub a, player_width
                ld c, a
                jr nc, :+
                    dec b
            :

            ;Write position to ram and stack
            ld [hl], b
            inc l
            ld [hl], c
            push bc

            ;Jump
            jr .vertical
    ;
    
    ;Vertical collision handling
    .vertical:

        ;Pop player X-position to DE
        pop de

        ;Pop Y-position to BC
        pop bc

        ;Check player direction
        inc l
        inc l
        inc l
        ld a, [hl+]
        inc l
        bit player_going_up, a
        jr nz, :++

            ;Player is moving down, add offset + vspeed
            ld a, c
            sub a, player_ledgehang_offset
            add a, [hl]
            push af
            ld a, c
            add a, [hl]
            jr nc, :+
            inc b
            :
            add a, player_height
            ld c, a
            jr nc, :++
            inc b
            jr :++
        :
            ;Player is moving up, subtract vspeed
            xor a
            push af
            ld a, c
            sub a, [hl]
            ld c, a
            jr nc, :+
            dec b
        :

        ;Push this Y-position (and X-position) and pointer to player data
        push bc
        push hl

        ;Turn this into a pointer to stage data in DE
        xor a ;Resets carry flag
        ld c, e
        ld e, a
        rr b
        rr e
        rr b
        rr e
        ld a, d
        add a, e
        ld l, a
        ld a, b
        add a, high(level_foreground)
        ld h, a

        ;Is this block solid?
        ld d, high(w_blockdata)
        ld e, [hl]
        ld a, [de]
        bit bpb_solid, a
        jr nz, .Vcollision
        bit bpb_platform, a
        call nz, .platform

            ;Block is not solid, check if there is a tile overlap
            ;Is player in the middle of 2 blocks?
            ld a, c
            add a, player_width
            jp nc, .add_vspp

                ;There is a block overlap, check new block
                inc l
                ld e, [hl]
                ld a, [de]
                bit bpb_solid, a
                jr nz, .Vcollision
                bit bpb_platform, a
                call nz, .platform

                ;No collision
                jr .add_vspp
        ;

        ;Platform handling
        .platform
            
            ;Return if player is moving up
            ld a, [w_player_direction]
            bit player_going_up, a
            ret nz

            ;Return if there isn't block overlap
            ld a, [w_player_y+1]
            add a, player_height
            ld e, a
            dec e
            ld a, [w_player_vspp]
            add a, e
            ret nc

            ;But the player state doe
            ld a, [w_player_state]
            cp a, pstate_ladder
            ret z

            ;There is a block overlap, normal collision
            pop af
            ;No jump needed, falls into .Vcollision
        ;


        ;Collision was found!
        .Vcollision

            ;Get collision coordinate
            ld a, l
            rlca 
            rlca 
            and a, %00000011
            ld l, a
            ld a, h
            rla 
            rla 
            and a, %00111100
            add a, l

            ;Set speed to 0
            pop hl
            ld e, [hl]
            ld [hl], 0

            ;Get direction
            dec l
            dec l
            bit player_going_up, [hl]
            jr nz, .goingup

                ;Player is moving down
                ;Set block coordinate
                dec l
                dec l
                and a, %00111111
                dec a
                ld [hl+], a

                ;Set subblock coordinate
                ld [hl], 256 - player_height

                ;Load this into BC
                pop bc
                ld b, a
                ld c, [hl]

                ;Set grounded variable
                ld a, player_jumptimer
                ld [w_player_grounded], a

                ;Was speed high enough for a spike check?
                ld a, e
                cp a, $28
                jr c, .ledge

                ;DID you land on spikes you dumb dumb?
                ld hl, w_player_y
                ld a, [hl-]
                ld c, a
                ld a, [hl-]
                add a, player_width/2
                ld a, [hl]
                jr nc, :+
                inc a
                :
                ld b, a

                coordinates_level b, c, d, e
                ld a, [de]
                ld e, a
                ld d, high(w_blockdata)
                ld a, [de]
                bit bpb_spike, a
                jr z, .ledge
                jp gameover


                ;Jump
                jr .ledge
            
            .goingup
                ;Player is moving up
                ;Set block coordinate
                dec l
                dec l
                and a, %00111111
                inc a
                ld [hl+], a

                ;Set subblock coordinate
                ld [hl], 0

                ;Load this into BC
                pop bc
                ld b, a
                ld c, [hl]

                ;Set grounded variable
                ld a, 1
                ld [w_player_grounded], a

                ;Jump
                jr .ledge
        ;

        ;Add vspp to player Y-position
        .add_vspp
            
            ;Retrieve pointer and Y-position itself from stack
            pop hl
            pop bc

            ;Move pointer to Y-location
            dec l
            dec l
            ld a, [hl-]
            dec l

            ;If moving down, subtract player height
            bit player_going_up, a
            jr nz, :+
                ld a, c
                sub a, player_height
                ld c, a
                jr nc, :+
                    dec b
            :

            ;Write Y-position to ram and stack
            ld [hl], b
            inc l
            ld [hl], c
    ;

    ;Ledge-hanging
    .ledge
        
        ;Get "ledge-state" (carry flag)
        pop af

        ;Jump if it just doesn't matter
        jr nc, .ledge_post

        ;If player is whipping, ignore ledges
        ld hl, w_player_state
        ld a, [hl+]
        cp a, pstate_whip
        jr z, .ledge_post

        ;Set up pointer to stage data (ugh)
        ;Grab player coordinates
        ld a, [hl+]
        ld c, a
        ld a, [hl+]
        ld e, a ;Save player sub-X-position in the stack
        ld d, [hl]
        dec d
        inc l
        inc l ;Move HL pointer to w_player_direction
        xor a

        ;Stage data conversion into BC
        rr d
        rra 
        rr d
        rra 
        add a, c
        ld c, a
        ld a, d
        add a, high(level_foreground)
        ld b, a

        ;Test player X-position
        ld a, e
        cp a, 255 - player_width
        jr z, .hang_right
        cp a, 0
        jr nz, .ledge_post

        ;Hang onto block to the left
            ;Make sure player is facing the right direction
            bit player_facing_left, [hl]
            jr z, .ledge_post

            ;Check tile above
            ld d, high(w_blockdata)
            ld a, [bc]
            ld e, a
            ld a, [de]
            bit bpb_solid, a
            jr nz, .ledge_post

            ;Move to offset
            dec c

            ;Jump to common ledge code
            jr .hang_common

        ;Hang onto block to the right
        .hang_right
            bit player_facing_left, [hl]
            jr nz, .ledge_post

            ;Check tile above
            ld d, high(w_blockdata)
            ld a, [bc]
            ld e, a
            ld a, [de]
            bit bpb_solid, a
            jr nz, .ledge_post

            ;Move block offset
            inc c
        
        ;General hanging code
        .hang_common
            
            ;Make sure block above side ISN'T solid
            ld a, [bc]
            ld e, a
            ld a, [de]
            bit bpb_solid, a
            jr nz, .ledge_post

            ;Move pointer down
            ld a, c
            add a, 64
            ld c, a
            jr nc, :+
            inc b
            :

            ;Make sure block below side IS solid
            ld a, [bc]
            ld e, a
            ld a, [de]
            bit bpb_solid, a
            jr z, .ledge_post

            ;Set Y-position
            dec l
            ld a, player_ledgehang_offset 
            ld [hl+], a

            ;Reset hspp and vspp
            inc l
            xor a
            ld [hl+], a
            ld [hl+], a
            ld a, pstate_ledge
            ld [w_player_state], a
        .ledge_post
    ;

    ;Camera movement
    .camera
        
        ;Load player X-position into a variable
        ld hl, w_player_y+1
        ld a, [hl-]
        add a, $80
        ld e, a
        ld a, [hl-]
        ld d, a
        jr nc, :+
        inc d
        :
        ld a, [hl-]
        add a, $80
        ld c, a
        ld a, [hl]
        ld b, a
        jr nc, :+
        inc b
        :


        call camera_handle
    ;

    ;Sprite handling
    .sprite_handle

        ;Is player invincible?
        ld l, low(w_player_invincible)
        bit 2, [hl]
        jr z, .spritenormal

            ;Make sprite go away
            ld l, low(w_player_sprite)
            ld l, [hl]
            ld h, high(w_oam_mirror)
            xor a
            ld b, OAMF_PAL1
            ld [hl+], a
            ld [hl+], a
            ld [hl+], a
            ld [hl], b
            inc l
            ld [hl+], a
            ld [hl+], a
            ld [hl+], a
            ld [hl], b
            inc l
            ld [hl+], a
            ld [hl+], a
            ld [hl+], a
            ld [hl], b
            inc l
            ld [hl+], a
            ld [hl+], a
            ld [hl+], a
            ld [hl], b
            inc l
            jp .sprite_post
    
        ;Convert from worldspace to screenspace
        .spritenormal
        ld l, low(w_player_x)

        ldh a, [h_scx]
        ld b, a
        ld a, [hl+]
        and a, %00001111
        ld e, a
        ld a, [hl+]
        and a, %11110000
        or a, e
        swap a
        sub a, b
        add a, $08 - $04
        ld e, a

        ldh a, [h_scy]
        ld b, a
        ld a, [hl+]
        and a, %00001111
        ld d, a
        ld a, [hl+]
        and a, %11110000
        or a, d
        swap a
        sub a, b
        add a, $08 + $03
        ld d, a

        ;Update sprite
        ld a, [w_player_sprite]
        ld l, a
        ld h, high(w_oam_mirror)
        ld [hl], d
        inc l
        ld [hl], e
        inc l

        ;Test sprite direction
        ld a, [w_player_direction]
        bit player_facing_left, a
        jr z, :+
            
            ;Flip sprite
            ld a, 2
            ld [hl+], a
            set OAMB_XFLIP, [hl]
            jr .sprite2
        :
            ;Don't flip sprite
            xor a
            ld [hl+], a
            res OAMB_XFLIP, [hl]

        .sprite2
        inc l
        ld [hl], d
        inc l
        ld a, e
        add a, 8
        ld [hl], a
        inc l

        ;Test sprite direction
        ld a, [w_player_direction]
        bit player_facing_left, a
        jr z, :+
            
            ;Flip sprite
            xor a
            ld [hl+], a
            set OAMB_XFLIP, [hl]
            jr :++
        :
            ;Don't flip sprite
            ld a, 2
            ld [hl+], a
            res OAMB_XFLIP, [hl]
        :

        ;Display whip
        ld a, [w_player_state]
        cp a, pstate_whip
        jr nz, .whip_remove

            ;Get sprite 1 screen coordinates
            dec l
            dec l
            dec l
            ld a, [hl+] ;Sprite 1 Y-position-8 -> E
            sub a, 4
            ld e, a
            ld a, [hl+] ;Sprite 1 X-position -> D
            ld d, a

            ld a, [w_player_direction]
            ld c, a
            bit player_facing_left, c
            jr z, .whipright

                ;Player is facing left
                ld a, d
                sub a, $0A
                bit player_whip_state, c
                ld c, $08
                jr nz, :+
                    ;Backwhip, move whip a little
                    add a, $14
                    ld c, $04
                :
                ld d, a
                ld b, player_palette_whip + OAMF_XFLIP + OAMF_PAL1
                jr .placewhip
            
            .whipright
                
                ;Player is facing right
                ld a, d
                add a, $0A
                bit player_whip_state, c
                ld c, $0A
                jr nz, :+
                    ;Backwhip, move whip a little
                    sub a, $14
                    ld c, $06
                :
                ld d, a
                ld b, player_palette_whip + OAMF_PAL1
                ;Falls into label `.placewhip`

            .placewhip
            ;Move sprite pointer
            inc l
            inc l
            
            ;Whip sprite 0
            ld a, e
            ld [hl+], a
            ld a, d
            ld [hl+], a
            ld [hl], c
            inc l
            ld [hl], b

            ;Move pointer and write to whip sprite 1
            inc l
            ld a, e
            ld [hl+], a
            ld a, d
            bit player_facing_left, c
            sub a, 8
            ld [hl+], a
            bit 1, c
            jr nz, :+
                inc c
                inc c
                jr :++
            :
                dec c
                dec c
            :
            ld [hl], c
            inc l
            ld [hl], b
            jr .sprite_post
            
        .whip_remove

            ;Is player standing on top of a door?
            ld de, w_player_x+1
            ld a, [de]
            dec e
            add a, player_width/2
            ld a, [de]
            jr nc, :+
            inc a
            :
            ld b, a
            inc e
            inc e
            ld a, [de]
            ld c, a
            coordinates_level b, c, d, e
            ld a, [de]
            cp a, b_door_middle_exit
            jr nz, .nodoor

                ld hl, w_player_tick
                inc [hl]
                ld e, [hl]
            
                ;There is a door here!
                ;Make the sprite thingy appear
                ;Grab player X/Y
                ld hl, w_oam_mirror
                ld a, [hl+]
                sub a, 19
                ld b, [hl]
                ld c, a

                ;Write those coordinates to the helper sprites :)
                ;Place sprite 1
                ld l, low(w_oam_mirror + 8)
                ld [hl], c ;Sprite 1 Y
                inc l
                ld [hl], b ;Sprite 1 X
                inc l
                ld [hl], s_button ;Sprite 1 tile
                inc l
                ld [hl], OAMF_PAL1 | p_player ;Sprite 1 attibutes
                inc l

                ;Place sprite 2
                ld [hl], c ;Sprite 2 Y
                inc l
                ld a, b
                add a, 8
                ld [hl], a ;Sprite 2 X
                inc l
                ld [hl], s_button ;Sprite 2 tile
                ld a, [w_player_tick]
                inc a
                ld [w_player_tick], a
                bit 5, a
                jr z, :+
                ld [hl], s_button+2
                :
                inc l
                ld [hl], OAMF_PAL1 | p_player

                ;Is key pressed?
                ldh a, [h_input_pressed]
                bit PADB_START, a
                jr z, .nodoor

                    ;Go through door
                    jp setup_newlevel

            .nodoor

            ;Move both sprites off the screen
            inc l
            xor a
            ld [hl+], a
            inc l
            inc l
            inc l
            ld [hl+], a
            ;Falls into label `.sprite_post`.

        .sprite_post
    ;

    ;Entity collisions
    .entityhandle

        ;Create a collision buffer
            ld bc, w_collision_buffer+3
            ld hl, w_player_y+1

            ;Player-Y + height
            ld a, [hl-]
            add a, player_height
            ld [bc], a
            ld a, [hl-]
            adc a, 0
            dec c
            ld [bc], a
            dec c

            ;Player-X + width
            ld a, [hl-]
            add a, player_width
            ld [bc], a
            ld a, [hl-]
            adc a, 0
            dec c
            ld [bc], a
        ;

        ;Switch RAM bank
        ld a, bank_entsys
        ldh [rSVBK], a
        ld hl, w_entsys_collision
        .collisionloop

            ;Get entity collision and check if it's valid
            ld c, entsys_col_treasureF | entsys_col_damagableF | entsys_col_bounceF | entsys_col_enemyF
            call _hl_
            ld b, a
            ld a, d
            cp a, $FF
            jp z, .entity_post

            ;There is a valid entity
            ;Check collision for real
            push bc
            push de
            push hl
            ld bc, w_collision_buffer
            call entsys_collision_PE
            pop hl
            pop de
            pop bc
            jr z, .collisionloop

            ;There is a collision
            ;Check flag
            bit entsys_col_treasureB, b
            jr z, .notreasure

                ;This is treasure
                ;Destroy entity
                push hl
                ld a, e
                and a, %11000000
                ld l, a
                ld h, d
                call entsys_destroy
                pop hl
                
                ;Return to loop
                jr .collisionloop
            
            .notreasure
            bit entsys_col_enemyB, b
            jp z, .noenemy

                ;This is an enemy
                push hl

                ;Can you bounce on it?
                bit entsys_col_bounceB, b
                jr z, .nobounce
                
                    ld a, b
                    ldh [h_temp7], a

                    ;You can bounce on it!
                    ;Check if possible
                    inc e
                    inc e
                    inc e
                    
                    ;Subtract player Vspeed from Y+height
                    ld a, [w_player_vspp]
                    add a, $20
                    ld b, a
                    ld hl, w_collision_buffer+3
                    ld a, [hl-]
                    sub a, b
                    ld c, a
                    ld a, [hl+]
                    sbc a, 0
                    ld b, a

                    ;Compare values
                    ;cp entity_y, w_player_y+height-vspp
                    ;fc = entity_y < w_player_y
                    ld a, [de]
                    cp a, b
                    jr nz, :+

                        inc e
                        ld a, [de]
                        dec e
                        cp a, c
                    :

                    ;Bounce failed :(
                    jr c, .bouncefail

                    ;Bounce succeeded, do thing
                    ld hl, w_player_direction
                    bit player_going_up, [hl]
                    jr nz, .bouncefail
                    
                    ;Set player direction
                    set player_going_up, [hl]
                    ldh a, [h_input]
                    ld b, a
                    inc l
                    inc l
                    xor a
                    ld a, [hl]
                    rra 
                    cp a, player_jump_force + 16
                    jr nc, :+
                    ld a, player_jump_force + 16
                    :
                    ld [hl], a
                    ld l, low(w_player_grounded)
                    ld [hl], player_jumptimer-4

                    ;Give player a single frame of invincibility
                    ld l, low(w_player_invincible)
                    ld a, [hl]
                    cp a, 0
                    jr nz, :+
                    ld [hl], 1
                    :

                    ld a, e
                    and a, %11000000
                    ld e, a
                    
                    ;Can this entity take damage?
                    ldh a, [h_temp7]
                    bit entsys_col_damagableB, a
                    ld c, 1
                    call nz, entsys_damage

                    ;Pop and return to loop
                    pop hl
                    jp .collisionloop

                    .bouncefail
                    dec e
                    dec e
                    dec e
                    
                .nobounce
                
                ;Is player invincible?
                ld hl, w_player_invincible
                ld a, [hl]
                cp a, 0
                jr nz, .nohurt
                
                    ;Decrement player health
                    ld hl, w_player_health
                    dec [hl]
                    jp z, gameover

                    ;Set animation timer and invincible counter
                    ld l, low(w_player_invincible)
                    ld [hl], player_invincible_time
                    dec l
                    ld [hl], player_stun_time

                    ;Change direction
                    ld l, low(w_player_direction)
                    set player_going_up, [hl]
                    bit player_going_left, [hl]
                    set player_going_left, [hl]
                    jr z, :+
                    res player_going_left, [hl]
                    :
                    
                    ;Set Hspeed and Vspeed
                    inc l
                    ld [hl], player_stun_pushX
                    inc l
                    ld [hl], player_stun_pushY

                    ;Set player state to stun
                    ld l, low(w_player_state)
                    ld a, [hl]
                    cp a, pstate_whip
                    ld [hl], pstate_stun
                    jr nz, .wasnotwhipping

                        ;Reset all the funny business I did
                        push de
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
                        
                        pop de
                    .wasnotwhipping
                    ;Falls into label `.nohurt`.
                
                ;Pop this
                .nohurt
                pop hl

            .noenemy
            jp .collisionloop

            .entity_post
    ;

    ;Wait for DMA transfer to complete
    ld hl, rHDMA5
    ld a, $FF
    .dmawait
    cp a, [hl]
    jr nz, .dmawait
    
    ;Return
    ret 
; 