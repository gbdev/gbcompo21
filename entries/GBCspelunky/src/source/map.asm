INCLUDE "hardware.inc"
INCLUDE "chunktypes.inc"
INCLUDE "camera.inc"
INCLUDE "blockproperties.inc"
INCLUDE "dwellings/tileid.inc"

SECTION "level", ROM0

; Creates a level path.
; Maximum level size is 6x6.
; Currently locked to only generate 4x4 levels.
;
; Destroys: all
level_path_create::

    ;Clear chunk memory
    ld hl, w_chunk_type
    ld b, ct_invalid
    ld de, 36
    call memfill
    ld hl, w_chunk_info
    ld b, $00
    ld de, 36
    call memfill

    ;Is this a big level or not?
    ld bc, $0404 ;Force 4x4 level

    ;Write width to room variables
    ld a, b
    ld [w_level_width_chunks], a

    ;Multiply by 10 and save
    add a
    add a
    add a
    add b
    add b
    ld [w_level_width], a

    ;Set camera limits, update and scroll
    sub a, 7
    ld [w_camera_limit_x], a

    add a, 10
    ld [w_camera_ulimit_x], a


    ;Write height to room variables
    ld a, c
    ld [w_level_height_chunks], a
    ld e, c

    ;Multiply by 8 and save
    add a
    add a
    add a
    ld [w_level_height], a

    ;Set camera limits, update and scroll
    sub a, 6
    ld [w_camera_limit_y], a
    add a, 9
    ld [w_camera_ulimit_y], a






    ;Get starting room position
    call rng_run_single

    ;Make sure number is between 0 and level width
    :
    add a, b
    jr nc, :-
    
    ;Write number to chunk X position
    ld c, a

    ;Properly set chunk pointer
    ld hl, w_chunk_type
    add a, l
    ld l, a
    jr nc, :+
    inc h
    :
    push hl
    ld d, 1






    ;Loop
    .loop
    
        ;Chunk is by default horizontal
        ld [hl], ct_normal

        ;Save lasr chunk
        ld a, d
        ld [w_chunk_last], a
        
        ;Was last chunk a dropdown?
        cp a, ct_dropdown
        jr z, .dodropin
        cp a, ct_vertical
        jr nz, :+
        .dodropin

            ;Yes, switch to dropin
            ld [hl], ct_dropin
        :



        ;Get random number
        .getrng
            ;Is this chunk on the far left?
            ld d, $FE
            xor a
            cp a, c
            jr nz, :++

                ;Get random number between 0 and 2
                call rng_run_single
                :
                add a, 3
                jr nc, :-
                
                ;Make sure the number is in range
                cp a, 2
                jr nz, .process
                ld a, 4
                jr .process
            
            
            :
            ;Is this chunk on the far right?
            ld a, b
            dec a
            cp a, c
            jr nz, :++

                ;Get random number between 0 and 2
                call rng_run_single
                :
                add a, 3
                jr nc, :-
                
                ;Make sure the number is in range
                inc a
                inc a
                jr .process


            :

            ;Get a random number between 0 and 4
            call rng_run_single
            :
            add a, 5
            jr nc, :-
            dec d
        ;

        ;Process random number
        .process
        
            ;Check if we gotta go down
            cp a, 4
            jr z, .godown

            ;Jump if going left
            bit 1, a
            jr nz, .goleft

            .goright

                ;Increment D
                inc d
                jr z, .godown

                ;Check to the right
                inc c
                inc hl
                ld a, $FF
                cp a, [hl]
                jr z, .confirm

                ;Otherwise go left
                dec c
                dec hl
                jr .goleft
            ;

            .goleft
                    
                ;Increment D
                inc d
                jr z, .godown

                ;Check to the left
                dec c
                dec hl
                ld a, $FF
                cp a, [hl]
                jr z, .confirm

                ;Otherwise go right
                inc c
                inc hl
                jr .goright
            ;
        ;

        ;Get ready to go for another iteration
        .confirm
            ld d, 1
            jr .loop
        ;

    ;Move downwards
    .godown
        
        ;Set chunk to dropdown
        ld [hl], ct_dropdown

        ;Set chunk type to vertical if previous chunk went down
        ld a, [w_chunk_last]
        cp a, ct_vertical
        jr z, :+
        cp a, ct_dropdown
        jr nz, .wentnormal
        :
            ;Set chunk type to vertical
            ld [hl], ct_vertical

        .wentnormal
        ;Fill remaining slots with nopath's
        push hl
        ld a, l
        sub a, c
        ld l, a
        jr nc, :+
        dec h
        :
        dec hl

        ld d, b
        inc d
        .vertloop
            dec d
            jr z, :+    
        
            inc hl    
            ld a, [hl]
            cp a, $FF
            jr nz, .vertloop
            inc [hl]
            jr .vertloop
        :
        pop hl
        ld d, [hl]

        ;Leave the loop
        dec e
        jr z, .finish

        ;Move the pointer down
        ld a, l
        add a, 6
        ld l, a
        jr nc, :+
        inc h
        :

        ;Set previous chunk
        jp .loop


    .finish
        
        ;Make sure last chunk is either type 1 or type 3
        ;Turn type 4 into type 3
        ld a, [hl]
        cp a, ct_vertical
        jr nz, :+
            ld [hl], ct_dropin
        :

        ;Turn type 2 into type 1
        cp a, ct_dropdown
        jr nz, :+
            ld [hl], ct_normal
        :
    
        ;Mark the exit
        set ctb_exit, [hl]
        
        ;Mark the entrance
        pop hl
        set ctb_entrance, [hl]
    ;


    ;Flip some tiles
    ld b, 36
    ld hl, w_chunk_info
    .mirrorloop
        call rng_run_single
        bit cpb_mirror, a
        jr z, :+
        set cpb_mirror, [hl]
        :
        dec b
        jr nz, .mirrorloop

    ;Return
    ret
;



; Copies a single stage chunk into stage data.
; 
; Input:
; - `a`: Chunk properties
; - `b`: Chunk X position (0-5)
; - `c`: Chunk Y position (0-7)
; - `hl`: Chunk position in ROM (10*8 = $50 bytes)
level_chunk_place::


    ;Push chunk address for later
    push af
    push hl

    ;Get correct chunk X coordinate
    ld a, b
    add a, a
    add a, a
    add a, a
    add a, b
    add a, b
    inc a
    inc a
    ld b, a

    ;Get correct chunk Y coordinate
    ld a, c
    add a, a
    add a, a
    add a, a
    add a, a

    ;Multiply Y coordinate
    ld h, 0
    ld l, a
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    ld de, 128
    add hl, de

    ;Add offsets together
    ld c, b
    ld b, 0
    add hl, bc

    ;Add WRAM offset
    ld de, $D000
    add hl, de
    pop de

    ;B is horizontal counter, C is vertical
    ld bc, $0A08

    ;Test if chunk is mirrored
    pop af
    bit cpb_mirror, a
    jr z, .loop_normal



    ;Copy data mirrored
    ld a, e
    add a, 9
    ld e, a
    jr nc, :+
    inc d
    :

    ;What
    .loop_mirror
    ld a, [de]
    dec de
    ld [hl+], a

    ;Decrease horizontal count
    dec b
    jr nz, .loop_mirror
    
    ;Add 54 to stage pointer
    ld a, 54
    add a, l
    ld l, a
    ld a, b ;B is 0 from the countdown
    adc a, h
    ld h, a

    ld a, e
    add a, 20
    ld e, a
    jr nc, :+
    inc d
    :

    ;Reset horizontal counter
    ld b, $0A

    ;Decrease and test vertical counter
    dec c
    jr nz, .loop_mirror
    jr .return
    


    ;Copy data normally
    .loop_normal
    ld a, [de]
    inc de
    ld [hl+], a

    ;Decrease horizontal count
    dec b
    jr nz, .loop_normal
    
    ;Add 54 to stage pointer
    ld a, 54
    add a, l
    ld l, a
    ld a, b ;B is 0 from the countdown
    adc a, h
    ld h, a

    ;Reset horizontal counter
    ld b, $0A

    ;Decrease and test vertical counter
    dec c
    jr nz, .loop_normal

    ;Return
    .return
    ret
;



; Copies an entire stage into WRAM
;
; Input:
; - `bc`: Foreground location in ROM
; - `de`: Background location in ROM
level_load::
    
    ;Push background location for later use
    push de

    ;Switch WRAM bank
    ld a, bank(level_foreground)
    ldh [rSVBK], a

    ;Load foreground
    ld hl, level_foreground
    ld de, $1000
    call memcopy

    ;Switch bank again
    ld a, bank(level_background)
    ldh [rSVBK], a

    ;Pop background location
    pop bc

    ;Load background
    ld hl, level_foreground
    ld de, $1000
    call memcopy
    
    ;Return
    ret 
;



; Adds a block to the list of tiles to be updated next V-blank cycle.
;
; Input:
; - `hl`: Block address
; 
; Destroys: `af`, `de`
map_update_block::

    ;Check tile above real quick
    push hl
    ld a, l
    sub a, 64
    ld l, a
    jr nc, :+
    dec h
    :

    ;Is this tile platform related?
    ld a, [hl]
    ld e, a
    cp a, b_platform
    jr z, .update
    cp a, b_platform_support
    jr z, .update

    ;Is this tile destructable or a spike?
    ld d, high(w_blockdata)
    ld a, [de]
    and a, bpf_destructible | bpf_spike
    jr z, :+

        ;Update that stuff right now!!
        .update
        ld [hl], 0
        call map_update_block
    :
    pop hl

    
    ;Get pointer to new data
    ld de, w_screen_update_list_head
    ld a, [de]
    ld e, a

    ;Write address + terminator byte to the list
    ld a, h
    ld [de], a
    inc e
    ld a, l
    ld [de], a
    inc e

    ;Update list head position and counter
    xor a ;Resets carry flag
    ld e, a
    ld a, [de]

    ;Is this the first block to be written?
    cp a, 0
    jr nz, :+

        ;Write update flag
        ld a, [w_screen_update_enable]
        set camb_update_list, a
        ld [w_screen_update_enable], a
        xor a
    :

    ;Update block count and list head
    inc a
    ld [de], a
    inc a
    rlca 
    inc e
    ld [de], a

    ;Return
    ret 
;



; Converts an entity's XXYY coordinates into XY screen coordinates
;
; Input:
; - `bc` = Entity pointer
;
; Output:
; - `hl` = Entity pointer + 4
; - `de` = sprite X/Y
worldspace_to_screenspace: MACRO 
    

    ;19
    ;Camera position
    ld hl, w_camera_x   ;12
    ld a, [hl+]         ;8
    ld d, a             ;4
    ld a, [hl+]         ;8
    ld e, a             ;4
    push de             ;16
    ld a, [hl+]         ;8
    ld d, a             ;4
    ld a, [hl]          ;8
    ld e, a             ;4

    ;8
    ;Player X position
    ld h, b             ;4
    ld l, c             ;4
    ld a, [hl+]         ;8
    ld b, a             ;4
    ld a, [hl+]         ;8
    ld c, a             ;4

    ;17
    ;Convert from world space to screen space
    ld a, c             ;4
    sub a, e            ;4
    ld c, a             ;4
    ld a, b             ;4
    sbc a, d            ;4
    and a, %00001111    ;8
    ld b, a             ;4
    ld a, c             ;4
    and a, %11110000    ;8
    or a, b             ;4
    swap a              ;8
    ldh [h_temp], a     ;12

    ;9
    ;Player Y position
    ld a, [hl+]         ;8
    ld b, a             ;4
    ld a, [hl+]         ;8
    ld c, a             ;4
    pop de              ;12

    ;14
    ;Convert from world space to screen space
    ld a, c             ;4
    sub a, e            ;4
    ld c, a             ;4
    ld a, b             ;4
    sbc a, d            ;4
    and a, %00001111    ;8
    ld b, a             ;4
    ld a, c             ;4
    and a, %11110000    ;8
    or a, b             ;4
    swap a              ;8

    ;5
    ;Store positions in BC
    ld e, a             ;4
    ldh a, [h_temp]     ;12
    ld d, a             ;4
ENDM

test:
    ;iBC = Pointer to entity data

    worldspace_to_screenspace

    ;6
    ;Grab sprite slots
    ld a, [hl]          ;8
    rlca                ;4
    rlca                ;4
    ld h, high(w_oam_mirror) ;8
    ld l, a             ;4

    ;16
    ;Write sprite data
    ld a, e             ;4
    ld [hl+], a         ;8
    ld a, d             ;4
    ld [hl+], a         ;8
    inc l               ;4
    inc l               ;4
    ld a, e             ;4
    ld [hl+], a         ;8
    ld a, d             ;4
    add a, 8            ;8
    ld [hl], a          ;8
;