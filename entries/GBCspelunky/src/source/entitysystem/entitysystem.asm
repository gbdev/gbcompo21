INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "entitysystem.inc"
INCLUDE "physics.inc"
INCLUDE "blockproperties.inc"
INCLUDE "player.inc"

SECTION "ENTITYSYSTEM", ROM0

; Creates an entity from a given pointer.
; 
; Input:
; - `bc`: Entity initialization data
;
; Output:
; - `hl`: Entity pointer
entsys_alloc::

    ;Switch WRAMX bank
    ld a, bank_entsys
    ldh [rSVBK], a
    
    ;Find a free entity
    call w_entsys_alloc

    ;Ensure entity validity
    .realloc::
    ld a, h
    cp a, $FF
    jr nz, :+

        ;Entity list is full, crash game
        ld hl, error_entityoverflow
        rst v_error
    :

    ;Copy variable offset to DE
    ld d, h
    ld a, l
    add a, entity_variables
    ld e, a
    push hl

    ;Unclog execution
    xor a
    ld [hl+], a
    ld [hl+], a
    inc l
    inc l
    inc l
    inc l

    ;Set execution code bank
    ld a, [bc]
    ld [hl+], a
    inc l
    inc bc

    ;Set execution code pointer
    ld a, [bc]
    ld [hl+], a
    inc bc
    ld a, [bc]
    ld [hl+], a

    ;Write "jr @+66-2" in alloc section
    ld a, l
    add a, 5
    ld l, a
    ld a, _jr
    ld [hl+], a
    ld [hl], _next

    ;Write collision mask
    ld a, l
    add a, 5
    ld l, a
    ld a, _ldan8
    ld [hl+], a
    inc bc
    ld a, [bc]
    ld [hl+], a

    ;Write destruction code pointer
    ld a, l
    or a, %00111111
    dec a
    dec a
    ld l, a
    inc bc
    ld a, [bc]
    ld [hl+], a
    inc bc
    ld a, [bc]
    ld [hl+], a
    inc bc
    ld a, [bc]
    ld [hl], a

    ;Write variable pointer to DE
    ld d, h
    ld a, l
    and a, %11000000
    or a, entity_variables
    ld e, a

    ;Jump to entity initialization code
    inc bc
    ld a, [bc]
    ld l, a
    inc bc
    ld a, [bc]
    ld h, a
    call _hl_

    ;Return
    pop hl
    ret 
;



; Frees an allocated entity.
;
; Input: 
; - `hl`: Pointer to entity
;
; Destroys: `af`, `b`
entsys_free::

    ;Switch WRAMX bank
    ld a, bank_entsys
    ldh [rSVBK], a
    .skipbank
    
    ;Save this in B
    ld b, l
    
    ;Stop execution (write "jr @+66-2")
    ld a, _jr
    ld [hl+], a
    ld a, _next
    ld [hl+], a

    ;Re-enable allocation (write "nop" twice)
    ld a, b
    add a, entity_allocate
    ld l, a
    xor a
    ld [hl+], a
    ld [hl+], a

    ;Stop collision (write "jr @+66-2")
    ld a, b
    add a, entity_collision
    ld l, a
    ld a, _jr
    ld [hl+], a
    ld a, _next
    ld [hl+], a

    ;Return
    ld l, b
    ret
;



; Deletes ALL entities currently in the entity system.
entsys_clear::

    ;Switch WRAMX bank
    ld a, bank_entsys
    ldh [rSVBK], a
    
    ;Set initial values
    ld hl, w_entsys
    ld de, entity_size
    ld c, entity_count

    ;Call `entsys_free` for each entity
    .loop
        call entsys_free.skipbank
        add hl, de
        dec c
        jr nz, .loop
    
    ;Return
    ret
;



; Damage an entity.
; Decreases entity health, and calls destroy 
; event if entity health reaches 0.
;
; Input:
; - `de`: Entity pointer (%xx000000)
; - `c`: Damage to deal
entsys_damage::

    push hl

    ;Put pointer into HL
    ld h, d

    ;Remove enemy property in collision flag
    ld a, e
    or a, entity_collision + 1
    ld l, a
    res entsys_col_enemyB, [hl]

    ;Set entity state to tumble
    ld a, e
    or a, entity_state
    ld l, a
    ld [hl], gstate_tumble

    ;Set enemy timer
    ld a, e
    or a, entity_timer
    ld l, a
    ld [hl], physics_tumbletime

    ;Set speed
    ld a, e
    or a, entity_direction
    ld l, a
    set physics_going_up, [hl]
    inc l
    ld [hl], physics_bounce_x
    inc l
    ld [hl], physics_bounce_y

    
    ;Get health pointer
    ld a, e
    or a, entity_health
    ld l, a

    ;Subtract damage
    ld a, [hl]
    inc c
    sub a, c
    ld [hl], a

    ;Did it reach 0 or below?
    call c, entsys_destroy
    inc [hl]

    ;Return
    pop hl
    ret 
;



; Runs a piece of code from the entity, then frees it.
;
; Input:
; - `hl`: Pointer to entity (anywhere)
;
; Destroys: `af`, `bc`, `d`, unknown
; Saves: `hl`
entsys_destroy::

    push hl
    
    ;Prepare pointer
    ld b, h
    ld c, l
    ld a, l
    and a, %11000000
    ld l, a
    push hl
    or a, %00111111
    ld l, a

    ;Read bank and address
    ld d, [hl]
    dec l
    ld a, [hl-]
    ld l, [hl]
    ld h, a

    ;Call destroy event code
    call bank_call_xd

    ;Free the entity from entsys
    pop hl
    xor a
    cp a, b
    call nz, entsys_free

    ;Return
    pop hl
    ret 
;



; Checks for a collision with the camera bounding box.
; Any entities collided with get their visibility flag set.
entsys_oobcheck::

    ;Prepare camera pseudo-entity
    ;Read camera XXYY position into BC and DE
    ld hl, w_camera_position
    ld a, [hl+]
    sub a, 2
    ld b, a
    jr nc, :+
        ld b, 0
    :
    ld a, [hl+]
    ld c, a
    ld a, [hl+]
    sub a, 2
    ld d, a
    jr nc, :+
        ld d, 0
    :
    ld a, [hl+]
    ld e, a

    ;Write those positions to pseudo entity
    ;Write XX and YY
    ld hl, w_camera_cull
    push hl
    ld a, b
    ld [hl+], a
    ld a, c
    ld [hl+], a
    ld a, d
    ld [hl+], a
    ld a, e
    ld [hl+], a

    ;Write xx and yy
    ld a, b
    add a, 14
    ld [hl+], a
    ld a, c
    ld [hl+], a
    ld a, d
    add a, 12
    ld [hl+], a
    ld a, e
    ld [hl+], a

    ld hl, w_entsys_collision

    .loop
        ;Load collision mask to check
        ld c, entsys_col_visibleF

        ;Check entity masks
        call _hl_ ;HL points to `w_entsys_collision`.
        ld a, d
        cp a, $FF
        jr z, .quit

        ;Refresh funny value
        pop bc
        push bc
        push de
        push hl

        ;Call collision
        call entsys_collision_RE
        ld b, 0
        jr z, :+
            set 7, b
        :
        pop hl
        pop de

        ;Get pointer to visible flag
        ld a, e
        and a, %11000000
        or a, entity_visible
        ld e, a

        ;Set/reset visibility flag
        ld a, [de]
        and a, %01111111
        or a, b
        ld [de], a

        ;Go back in
        jr .loop

    .quit
        
        ;Clean up stack and return
        pop bc
        ret 
;



; Entity collision check.
; Rectangle to entity.
;
; Input:
; - `bc`: Rectangle data pointer [XXYYxxyy]
; - `de`: Entity data pointer [-XXYY----WWHH]
;
; Output:
; - `fz`: Result (0 = yes, 1 = no)
;
; Destroys: all
entsys_collision_RE::
    
    ;Load rectangle pointer into HL
    ld h, b
    ld l, c
    inc l
    inc l
    
    ;Load Y-positions into BC
    inc e
    inc e
    inc e
    ld a, [de]
    ld b, a
    inc e
    ld a, [de]
    ld c, a

    ;Comparison #1
    ;cp rect_y, entity_y
    ld a, [hl+]
    cp a, b
    jr nz, :+

        ld a, [hl]
        cp a, c
    :

    jr c, .lesserY

        ;Rectangle is below entity Y-position
        ;Move entity pointer to sub-height
        ld a, e
        add a, 8
        ld e, a
        
        ;Add entity height to Y-position
        ld a, [de]
        dec e
        add a, c
        ld c, a
        ld a, [de]
        adc a, b
        ld b, a

        ;Compare again
        ;cp rect_y, entity_y + entity_height
        dec l ;rect_y
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        jr c, .checkx

        ;Otherwise return false
        xor a
        ret 
    ;

    .lesserY

        ;Rectangle is ABOVE entity Y-position
        inc l
        inc l
        inc l ;rect_y2

        ;Compare again
        ;cp rect_y + rect_height, entity_y
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        dec l
        dec l
        dec l
        dec l
        jr nc, .checkx

        ;Otherwise return false
        xor a
        ret 
    ;

    .checkx
    
    ;Move entity pointer
    ld a, e
    and a, %11000000
    or a, entity_x
    ld e, a

    ;Get rectangle X-position
    dec l
    dec l
    dec l
    
    ;Load X-positions into BC
    ld a, [de]
    ld b, a
    inc e
    ld a, [de]
    ld c, a

    ;Comparison #1
    ;cp rect_x, entity_x
    ld a, [hl+]
    cp a, b
    jr nz, :+

        ld a, [hl]
        cp a, c
    :

    jr c, .lesserX

        ;Player is to the left of entity X-position
        ;Move entity pointer to sub-width
        ld a, e
        add a, 8
        ld e, a
        
        ;Add entity width to X-position
        ld a, [de]
        dec e
        add a, c
        ld c, a
        ld a, [de]
        adc a, b
        ld b, a

        ;Compare again
        ;cp rect_x, entity_x + entity_width
        dec l
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        jr c, .checkd

        ;Otherwise return false
        xor a
        ret 
    ;

    .lesserX

        ;Rectangle is to the right of entity X-position
        inc l
        inc l
        inc l

        ;Compare again
        ;cp w_player_x, entity_x + entity_width
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        dec l
        dec l
        dec l
        dec l
        jr nc, .checkd

        ;Otherwise return false
        xor a
        ret 
    ;
    
    ;Collision was found, reset Z flag and return
    .checkd
    rra
    ret 
;



; Entity collision check.
; Rectangle to player.
; Input:
; - `bc`: Rectangle data pointer [XXYYxxyy]
;
; Output:
; - `fz`: Result (0 = yes, 1 = no)
;
; Destroys: all
entsys_collision_RP::

    ;Load player pointer into HL
    ld hl, w_player_x

    ;Compare X-positions
    ld a, [bc]
    cp a, [hl]
    jr nz, :+
        
        ;Compare Xsub positions
        inc c
        inc l
        ld a, [bc]
        ld d, [hl]
        dec c
        dec l
        cp a, d

        ;They are equal, small life hack
        jr z, .ycheck
    :

    ;Rectangle-X is lower than player-X
    jr nc, .higherX

        ;Rectangle X is lower than player X
        ld e, c
        ld a, c
        add a, 4
        ld c, a
        jr nc, :+
        inc b
        :

        ;Compare rectangle width
        ld a, [bc]
        cp a, [hl]
        jr nz, :+
            
            ;Compare Xsub positions
            inc c
            inc l
            ld a, [bc]
            ld d, [hl]
            dec c
            dec l
            cp a, d
        :

        ld c, e

        ;Rectangle X2 is NOT lower than player X
        jr nc, .ycheck

            ;It is lower, return
            xor a
            ret 
    ;

    ;Rectangle-X is higher than player-X
    .higherX
        
        ;Rectangle X is higher than player-X
        ;Load player X-coordinates into DE
        ld a, [hl+]
        ld d, a
        ld a, [hl]

        ;Add player width to player X-coordinates
        add a, player_width
        ld e, a
        jr nc, :+
        inc d
        :

        ;Compare again
        ld a, [bc]
        cp a, d
        jr nz, :+
            
            ;Compare Xsub positions
            inc c
            ld a, [bc]
            dec c
            cp a, e
        :

        ;Entity width overlaps rectangle X
        jr c, .ycheck

            ;No it doesn't, return false
            xor a
            ret 
    ;

    .ycheck
    ;Move pointers
    inc bc
    inc bc
    inc l
    inc l

    ;Compare Y-positions
    ld a, [bc]
    cp a, [hl]
    jr nz, :+
        
        ;Compare Ysub-positions
        inc c
        inc l
        ld a, [bc]
        ld d, [hl]
        dec c
        dec l
        cp a, d

        ;They are equal, small life hack
        jr z, .donecheck
    :

    ;Rectangle-Y is lower than player-Y
    jr nc, .higherY

        ;Rectangle-Y is lower than player-Y
        ld a, c
        add a, 4
        ld c, a
        jr nc, :+
        inc b
        :

        ;Compare rectangle width
        ld a, [bc]
        cp a, [hl]
        jr nz, :+
            
            ;Compare Ysub-positions
            inc c
            inc l
            ld a, [bc]
            ld d, [hl]
            dec c
            dec l
            cp a, d
        :

        ;Rectangle Y2 is NOT lower than player-Y
        jr nc, .donecheck

            ;It is lower, return false
            xor a
            ret 
    ;

    ;Rectangle-Y is higher than player-Y
    .higherY
        
        ;Rectangle Y is higher than player-Y
        ld a, [hl+]
        ld d, a
        ld a, [hl]

        ;Add player height to player Y-coordinates
        add a, player_height
        ld e, a
        jr nc, :+
        inc d
        :

        ;Compare again
        ld a, [bc]
        cp a, d
        jr nz, :+
            
            ;Compare Ysub positions
            inc c
            ld a, [bc]
            dec c
            cp a, e
        :

        ;Player width overlaps rectangle Y
        jr c, .donecheck

            ;No
            xor a
            ret 
    ;
    
    ;Collision was found, reset Z flag and return
    .donecheck
    rra
    ret 
;



; Checks collision between player and an entity.
;
; Input:
; - `de`: Entity data pointer [-XXYY----WWHH]
;
; Output:
; - `fz`: Result (0 = yes, 1 = no)
;
; Destroys: all
entsys_collision_PE::

    ;Skip checking if entity isn't visible
    ld b, e
    ld a, e
    add a, entity_visible - entity_state
    ld e, a
    ld a, [de]
    cp a, 0
    ret z
    ld e, b
    
    ;Get player Y-position
    ld hl, w_player_y
    
    ;Load Y-positions into BC
    inc e
    inc e
    inc e
    ld a, [de]
    ld b, a
    inc e
    ld a, [de]
    ld c, a

    ;Comparison #1
    ;cp w_player_y, entity_y
    ld a, [hl+]
    cp a, b
    jr nz, :+

        ld a, [hl]
        cp a, c
    :

    jr c, .lesserY

        ;Player is below entity Y-position
        ;Move entity pointer to sub-height
        ld a, e
        add a, 8
        ld e, a
        
        ;Add entity height to Y-position
        ld a, [de]
        dec e
        add a, c
        ld c, a
        ld a, [de]
        adc a, b
        ld b, a

        ;Compare again
        ;cp w_player_y, entity_y + entity_height
        dec l
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        jr c, .checkx

        ;Otherwise return false
        xor a
        ret 
    ;

    .lesserY

        ;Player is ABOVE entity Y-position
        ld hl, w_collision_buffer+2 ;Player-Y + height

        ;Compare again
        ;cp w_player_y + player_height, entity_y
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        jr nc, .checkx

        ;Otherwise return false
        xor a
        ret 
    ;

    .checkx
    
    ;Move entity pointer
    ld a, e
    and a, %11000000
    or a, entity_x
    ld e, a

    ;Get player X-position
    ld hl, w_player_x
    
    ;Load X-positions into BC
    ld a, [de]
    ld b, a
    inc e
    ld a, [de]
    ld c, a

    ;Comparison #1
    ;cp w_player_x, entity_x
    ld a, [hl+]
    cp a, b
    jr nz, :+

        ld a, [hl]
        cp a, c
    :

    jr c, .lesserX

        ;Player is to the left of entity X-position
        ;Move entity pointer to sub-width
        ld a, e
        add a, 8
        ld e, a
        
        ;Add entity width to X-position
        ld a, [de]
        dec e
        add a, c
        ld c, a
        ld a, [de]
        adc a, b
        ld b, a

        ;Compare again
        ;cp w_player_x, entity_x + entity_width
        dec l
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        jr c, .checkd

        ;Otherwise return false
        xor a
        ret 
    ;

    .lesserX

        ;Player is to the right of entity X-position
        ld hl, w_collision_buffer ;Player-X + Width

        ;Compare again
        ;cp w_player_x, entity_x + entity_width
        ld a, [hl+]
        cp a, b
        jr nz, :+

            ld a, [hl]
            cp a, c
        :

        ;Branch maybe
        jr nc, .checkd

        ;Otherwise return false
        xor a
        ret 
    ;

    ;Return true
    .checkd
    rra 
    ret 
;



; Entity physics.
; Used to give entities physics that look like what the player has.
; 
; Temporary variables used:
; - 1: vspp
; - 2: ysub
; - 3: width
; - 4: height
; - 5: direction
; 
; Input:
; - `hl`: `w_entity_variables`
entsys_physics::

    ;Horizontal collision handling
    .horizontal
        
        ;Save width and height in temporary variables
        ld a, l
        add a, 12
        ld l, a
        ld a, [hl-]
        ldh [h_temp4], a
        dec l
        ld a, [hl-]
        ldh [h_temp3], a
        ld a, l
        sub a, 8
        ld l, a
        
        
        ;Grab entity X-position to BC
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        ld c, a
    
        ;Grab Y-position to DE and push it
        ld a, [hl+]
        ld d, a
        ld a, [hl+]
        ldh [h_temp2], a
        ld e, a
        push de
    
        ;Offset x-position it a bit if moving right
        ld a, [hl+]
        ldh [h_temp5], a
        bit physics_going_left, a
        jr nz, :++
    
            ;Player is moving right, add offset + hspeed
            ldh a, [h_temp3]
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
    
        ;Push this X-position and pointer to entity data
        push bc
        push hl
    
        ;Turn this into a pointer to stage data in HL
        coordinates_level b, d, h, l
    
        ;Quickly switch RAM bank
        ld a, bank_foreground
        ldh [rSVBK], a
    
        ;Is this block solid?
        ld c, [hl]
        ld b, high(w_blockdata)
        ld a, [bc]
        bit bpb_solid, a
        jr nz, .Hcollision
    
            ;Block is not solid, check if there is a tile overlap
            ;Is entity in the middle of 2 blocks?
            ldh a, [h_temp4]
            cpl 
            inc a
            add a, e
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
                jr z, .add_hspp
                ;Falls into label `.Hcollision`
        ;
    
    
        ;Collision was found!
        .Hcollision
    
            ;Switch banks back
            ld a, bank_entsys
            ldh [rSVBK], a
        
            ;Set speed to 0
            ld a, l
            pop hl
            ld [hl], 0
    
            ;Get direction
            dec l
            bit physics_going_left, [hl]
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
                ld d, a
    
                ;Set subblock coordinate
                ldh a, [h_temp3]
                cpl 
                ld [hl], a
    
                ;Load this into BC
                pop bc
                ld b, d
                ld c, a
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
    
        ;Add hspp to entity X-position
        .add_hspp
            
        ;Load modified X-position into the WRAM variable
        
            ld a, bank_entsys
            ldh [rSVBK], a
        
            ;Retrieve pointer and X-position itself from stack
            pop hl
            pop bc
    
            ;Move pointer to X-position
            dec l
            ld a, [hl-]
            dec l
            dec l
            dec l
    
            ;If moving right, subtract entity width
            bit physics_going_left, a
            jr nz, :+
                ldh a, [h_temp3]
                ld d, a
                ld a, c
                sub a, d
                ld c, a
                jr nc, :+
                    dec b
            :
    
            ;Write position to ram and stack
            ld [hl], b
            inc l
            ld [hl], c
            push bc
    
            ;Falls into label `.vertical`
    ;
    
    ;Vertical collision handling
    .vertical:
    
        ;Pop entity X-position to DE
        pop de
    
        ;Pop Y-position to BC
        pop bc
    
        ;Check entity direction
        inc l
        inc l
        inc l
        ld a, [hl+]
        inc l
        bit physics_going_up, a
        jr nz, :++
    
            ;Entity is moving down, add offset + vspeed
            ldh a, [h_temp4]
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
            ;Player is moving up, subtract vspeed
            ld a, c
            sub a, [hl]
            ld c, a
            jr nc, :+
            dec b
        :
    
        ;Save vspeed in HRAM
        ld a, [hl]
        ldh [h_temp1], a
    
        ;Push this Y-position (and X-position) and pointer to entity data
        push bc
        push hl
    
        ;Turn this into a pointer to stage data in DE
        ld c, e
        coordinates_level d, b, h, l
    
        ;Switch RAM bank
        ld a, bank_foreground
        ldh [rSVBK], a
    
        ;Is this block solid?
        ld d, high(w_blockdata)
        ld e, [hl]
        ld a, [de]
        bit bpb_solid, a
        jr nz, .Vcollision
        bit bpb_platform, a
        call nz, .platform
    
            ;Block is not solid, check if there is a tile overlap
            ;Is entity in the middle of 2 blocks?
            ldh a, [h_temp3]
            add a, c
            jr nc, .add_vspp
    
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
            
            ;Return if entity is moving up
            ldh a, [h_temp5] ;Direction
            bit physics_going_up, a
            ret nz
    
            ;Return if there isn't block overlap
            ldh a, [h_temp2] ;ysub
            ld e, a
            ldh a, [h_temp4] ;height
            add a, e
            ld e, a
            dec e
            ldh a, [h_temp1] ;vspp
            add a, e
            ret nc
    
            ;There is a block overlap, normal collision
            pop af
            ;Falls into label `.Vcollision`
        ;
    
    
        ;Collision was found!
        .Vcollision
    
            ;Switch ram bank
            ld a, bank_entsys
            ldh [rSVBK], a
        
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
            ld [hl], 0
    
            ;Get direction
            dec l
            dec l
            bit physics_going_up, [hl]
            jr nz, .goingup
    
                ;Entity is moving down
                ;Set block coordinate
                dec l
                dec l
                and a, %00111111
                dec a
                ld [hl+], a
                ld e, a
    
                ;Set subblock coordinate
                ldh a, [h_temp4]
                cpl 
                inc a
                ld [hl], a
    
                ;Load this into BC
                pop bc
                ld b, e
                ld c, [hl]
    
                ;Set grounded variable
                ld a, l
                add a, entity_grounded - (entity_y+1)
                ld l, a
                ld [hl], 1
    
                ;Return
                ret
            
            .goingup
                ;Entity is moving up
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
                ld a, l
                add a, entity_grounded - (entity_y+1)
                ld l, a
                ld [hl], 0
    
                ;Return
                ret
        ;
    
        ;Add vspp to entity Y-position
        .add_vspp
            
            ;Switch RAM banks
            ld a, bank_entsys
            ldh [rSVBK], a
        
            ;Retrieve pointer and Y-position itself from stack
            pop hl
            pop bc
    
            ;Move pointer to Y-location
            dec l
            dec l
            ld a, [hl-]
            dec l
    
            ;If moving down, subtract entity height
            bit physics_going_up, a
            jr nz, :+
                
                ldh a, [h_temp4]
                ld d, a
                ld a, c
                sub a, d
                ld c, a
                jr nc, :+
                    dec b
            :
    
            ;Write Y-position to ram
            ld [hl], b
            inc l
            ld [hl], c

            ;Set grounded variable
            ld a, l
            add a, entity_grounded - (entity_y+1)
            ld l, a
            ld [hl], 0
    
            ;Return
            ret 
;



; Entity physics.
; Used to give entities physics that look like what the player has.
; 
; Temporary variables used:
; - 1: vspp
; - 2: ysub
; - 3: width
; - 4: height
; - 5: direction
; 
; Input:
; - `hl`: `w_entity_variables`
entsys_physics_tumble::

    ;Horizontal collision handling
    .horizontal
        
        ;Save width and height in temporary variables
        ld a, l
        add a, 12
        ld l, a
        ld a, [hl-]
        ldh [h_temp4], a
        dec l
        ld a, [hl-]
        ldh [h_temp3], a
        ld a, l
        sub a, 8
        ld l, a
        
        
        ;Grab entity X-position to BC
        ld a, [hl+]
        ld b, a
        ld a, [hl+]
        ld c, a
    
        ;Grab Y-position to DE and push it
        ld a, [hl+]
        ld d, a
        ld a, [hl+]
        ldh [h_temp2], a
        ld e, a
        push de
    
        ;Offset x-position it a bit if moving right
        ld a, [hl+]
        ldh [h_temp5], a
        bit physics_going_left, a
        jr nz, :++
    
            ;Player is moving right, add offset + hspeed
            ldh a, [h_temp3]
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
    
        ;Push this X-position and pointer to entity data
        push bc
        push hl
    
        ;Turn this into a pointer to stage data in HL
        coordinates_level b, d, h, l
    
        ;Quickly switch RAM bank
        ld a, bank_foreground
        ldh [rSVBK], a
    
        ;Is this block solid?
        ld c, [hl]
        ld b, high(w_blockdata)
        ld a, [bc]
        bit bpb_solid, a
        jr nz, .Hcollision
    
            ;Block is not solid, check if there is a tile overlap
            ;Is entity in the middle of 2 blocks?
            ldh a, [h_temp4]
            cpl 
            inc a
            add a, e
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
                jr z, .add_hspp
                ;Falls into label `.Hcollision`
        ;
    
    
        ;Collision was found!
        .Hcollision
    
            ;Switch banks back
            ld a, bank_entsys
            ldh [rSVBK], a
        
            ;Set speed to a quarter
            xor a
            ld a, l
            pop hl
            srl [hl]
            srl [hl]
    
            ;Get direction
            dec l
            bit physics_going_left, [hl]
            jr nz, .goingleft
    
                ;Player is moving right
                ;Switch direction
                set physics_going_left, [hl]

                ;Set block coordinate
                dec l
                dec l
                dec l
                dec l
                and a, %00111111
                dec a
                ld [hl+], a
                ld d, a
    
                ;Set subblock coordinate
                ldh a, [h_temp3]
                cpl 
                ld [hl], a
    
                ;Load this into BC
                pop bc
                ld b, d
                ld c, a
                push bc
    
                ;Jump
                jr .vertical
            
            .goingleft
                ;Player is moving left
                ;Switch direction
                res physics_going_left, [hl]

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
    
        ;Add hspp to entity X-position
        .add_hspp
            
        ;Load modified X-position into the WRAM variable
        
            ld a, bank_entsys
            ldh [rSVBK], a
        
            ;Retrieve pointer and X-position itself from stack
            pop hl
            pop bc
    
            ;Move pointer to X-position
            dec l
            ld a, [hl-]
            dec l
            dec l
            dec l
    
            ;If moving right, subtract entity width
            bit physics_going_left, a
            jr nz, :+
                ldh a, [h_temp3]
                ld d, a
                ld a, c
                sub a, d
                ld c, a
                jr nc, :+
                    dec b
            :
    
            ;Write position to ram and stack
            ld [hl], b
            inc l
            ld [hl], c
            push bc
    
            ;Falls into label `.vertical`
    ;
    
    ;Vertical collision handling
    .vertical:
    
        ;Pop entity X-position to DE
        pop de
    
        ;Pop Y-position to BC
        pop bc
    
        ;Check entity direction
        inc l
        inc l
        inc l
        ld a, [hl+]
        inc l
        bit physics_going_up, a
        jr nz, :++
    
            ;Entity is moving down, add offset + vspeed
            ldh a, [h_temp4]
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
            ;Player is moving up, subtract vspeed
            ld a, c
            sub a, [hl]
            ld c, a
            jr nc, :+
            dec b
        :
    
        ;Save vspeed in HRAM
        ld a, [hl]
        ldh [h_temp1], a
    
        ;Push this Y-position (and X-position) and pointer to entity data
        push bc
        push hl
    
        ;Turn this into a pointer to stage data in DE
        ld c, e
        coordinates_level d, b, h, l
    
        ;Switch RAM bank
        ld a, bank_foreground
        ldh [rSVBK], a
    
        ;Is this block solid?
        ld d, high(w_blockdata)
        ld e, [hl]
        ld a, [de]
        bit bpb_solid, a
        jr nz, .Vcollision
        bit bpb_platform, a
        call nz, .platform
    
            ;Block is not solid, check if there is a tile overlap
            ;Is entity in the middle of 2 blocks?
            ldh a, [h_temp3]
            add a, c
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
            
            ;Return if entity is moving up
            ldh a, [h_temp5] ;Direction
            bit physics_going_up, a
            ret nz
    
            ;Return if there isn't block overlap
            ldh a, [h_temp2] ;ysub
            ld e, a
            ldh a, [h_temp4] ;height
            add a, e
            ld e, a
            dec e
            ldh a, [h_temp1] ;vspp
            add a, e
            ret nc
    
            ;There is a block overlap, normal collision
            pop af
            ;Falls into label `.Vcollision`
        ;
    
    
        ;Collision was found!
        .Vcollision
    
            ;Switch ram bank
            ld a, bank_entsys
            ldh [rSVBK], a
        
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
            ld d, a
    
            ;Check speed
            pop hl
            ld a, [hl]
            cp a, $25
            ld a, 0 ;Avoid altering C flag
            jr c, :+
                ld a, [hl]
                srl a
                srl a
                inc a
            :
            ld [hl-], a
    
            ;Get direction
            dec l
            bit physics_going_up, [hl]
            jr nz, .goingup
    
                ;Entity is moving down
                ;Switch direction maybe
                cp a, 0
                jr z, :+
                    set physics_going_up, [hl]
                    jr :++
                :
                    ;Set hspeed to 0
                    inc l
                    ld [hl], 0
                    dec l
                :

                ;Set block coordinate
                dec l
                dec l
                ld a, d
                and a, %00111111
                dec a
                ld [hl+], a
                ld e, a
    
                ;Set subblock coordinate
                ldh a, [h_temp4]
                cpl 
                inc a
                ld [hl], a
    
                ;Load this into BC
                pop bc
                ld b, e
                ld c, [hl]
    
                ;Set grounded variable
                ld a, l
                add a, entity_grounded - (entity_y+1)
                ld l, a
                ld [hl], 1
    
                ;Return
                ret
            
            .goingup
                ;Entity is moving up
                ;Switch direction maybe
                cp a, 0
                jr z, :+
                res physics_going_up, [hl]
                :
                
                ;Set block coordinate
                dec l
                dec l
                ld a, d
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
                ld a, l
                add a, entity_grounded - (entity_y+1)
                ld l, a
                ld [hl], 0
    
                ;Return
                ret
        ;
    
        ;Add vspp to entity Y-position
        .add_vspp
            
            ;Switch RAM banks
            ld a, bank_entsys
            ldh [rSVBK], a
        
            ;Retrieve pointer and Y-position itself from stack
            pop hl
            pop bc
    
            ;Move pointer to Y-location
            dec l
            dec l
            ld a, [hl-]
            dec l
    
            ;If moving down, subtract entity height
            bit physics_going_up, a
            jr nz, :+
                
                ldh a, [h_temp4]
                ld d, a
                ld a, c
                sub a, d
                ld c, a
                jr nc, :+
                    dec b
            :
    
            ;Write Y-position to ram
            ld [hl], b
            inc l
            ld [hl], c

            ;Set grounded variable
            ld a, l
            add a, entity_grounded - (entity_y+1)
            ld l, a
            ld [hl], 0
    
            ;Return
            ret 
;



; Does gravity like what the player has.
; That's it I think.
;
; Input:
; - `hl`: `entity_variables`
;
; Output:
; - `hl`: `entity_variables`
entsys_gravity::

    ;Check direction
    ld a, l
    add a, 5
    ld l, a
    bit physics_going_up, [hl]
    jr z, .falling

        ;Entity is moving up, subtract speed
        inc l
        inc l
        ld a, [hl]
        sub a, physics_gravity
        ld [hl], a
        jr nc, .return

            ;Entity is now moving down
            xor a
            ld [hl], a
            dec l
            dec l
            res physics_going_up, [hl]
            jr .return
    ;
        
    .falling

        ;Entity is moving down, add speed
        inc l
        inc l
        ld a, [hl]
        add a, physics_gravity

        ;Cap speed
        cp a, physics_maxspeed
        jr c, :+
            ld a, physics_maxspeed
        :
        ld [hl], a
        ;Falls into label `.return`
    ;

    .return

        ;Set pointer and return peacefully
        ld a, l
        and a, %11000000
        or a, entity_variables
        ld l, a
        ret 
    ;
;