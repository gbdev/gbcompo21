INCLUDE "hardware.inc"
INCLUDE "camera.inc"

;Camera movement speed
cam_speed equ $30

SECTION "CAMERA TEST", ROM0

; Can be called directly after `input` to automatically sort out the inputs.
; Controls the camera
;
; Input:
; - `bc`: Attempted X-position
; - `de`: Attempted Y-position
;
; Destroys: all
camera_handle::

    call camera_set_position

    ;Store block X-position in E (Y-position is in D)
    ld e, b

    ;Compare current camera position to last camera position
    ld a, [w_screen_update_enable]
    xor a, %10000000
    res camb_update, a
    ld c, a
    bit camb_vertical, c
    jr z, .horizontal

        ;Check difference between current and previous camera block position
        inc l
        ld a, [hl]
        ld b, a
        cp a, d
        jr z, .nocheck

            ;There is a difference
            ld [hl], d
            set camb_update, c

            ;Write X position
            ld a, e
            ld [w_cam_update_x], a

            ;Figure out which direction the camera moved
            ld a, b
            cp a, d
            jr c, :+
                
                ;Camera moved up
                ld a, d
                ld [w_cam_update_y], a
                set camb_moved_up, c
                jr .nocheck

            :
                ;Camera moved down
                ld a, d
                add a, 10
                ld [w_cam_update_y], a
                res camb_moved_up, c
                jr .nocheck

    ;Camera moved horizontally
    .horizontal

        ;Check difference between current and previous camera block position
        ld a, [hl]
        ld b, a
        cp a, e
        jr z, .nocheck

            ;There is a difference
            ld [hl], e
            set camb_update, c

            ;Write Y position
            ld a, d
            ld [w_cam_update_y], a

            ;Figure out which direction camera moved
            ld a, b
            cp a, e
            jr c, :+

                ;Camera moved left
                ld a, e
                ld [w_cam_update_x], a
                set camb_moved_left, c
                jr .nocheck
            
            :
                ;Camera moved right
                ld a, e
                add a, 11
                ld [w_cam_update_x], a
                res camb_moved_left, c
                jr .nocheck

    ;Save update enable register
    .nocheck
    ld a, c
    ld [w_screen_update_enable], a

    


    ;Turn worldspace into screenspace
    ;Get camera X position
    ld hl, w_camera_x
    ld a, [hl+]
    ld b, a
    ld a, [hl+]
    ld c, a

    ;Turn it into screenspace
    ld a, c
    and a, $F0
    ld c, a
    ld a, b
    and a, $0F
    or a, c
    swap a

    ;Add offset and save
    add a, 8
    ldh [h_scx], a

    ;Get camera Y position
    ld a, [hl+]
    ld b, a
    ld a, [hl+]
    ld c, a

    ;Turn it into screenspace
    ld a, c
    and a, $F0
    ld c, a
    ld a, b
    and a, $0F
    or a, c
    swap a

    ;Add offset and save
    add a, 8
    ldh [h_scy], a
    
    ;Make sure camera doesn't load invalid tiles
    ld hl, w_cam_update_x
    ld a, [hl]
    
    ;Snap to 0
    cp a, $FF
    jr nz, :+
        xor a
        ld [hl], a
        jr .checky
    :

    ;Snap to level width
    ld b, a
    ld a, [w_camera_ulimit_x]
    cp a, b
    jr nc, :+
        dec a
        ld [hl], a
    :

    ;Snap to 0
    .checky
    inc hl
    ld a, [hl]
    cp a, $FF
    jr nz, :+
        xor a
        ld [hl], a
        jr .checkd
    :

    ;Snap to level height
    ld b, a
    ld a, [w_camera_ulimit_y]
    cp a, b
    jr nc, :+
        dec a
        ld [hl], a
    :
    .checkd




    ;Return
    ret 
;



; Sets camera's position to the given coordinates.
; The coordinates given are at the CENTER of the camera.
; Does not modify `rSCX` or `rSCY`.
;
; Input:
; `bc`: X-position
; `de`: Y-position
;
; Output:
; `bc`: Modified X-position
; `de`: Modified Y-position
;
; Destroys: all
camera_set_position::

    ;Move camera to the corner
    ld a, b
    sub a, 5
    ld b, a
    ld a, d
    sub a, 5
    ld d, a
    ld a, c
    sub a, $C0
    ld c, a
    jr nc, :+
    dec b
    :

    ;OOB check to the left
    ld a, $F0 ;"lowest" camera X-position - 1
    cp a, b
    jr nc, :+
        xor a
        ld b, a
        ld c, a
        jr .vertical
    :

    ;OOB check to the right
    ld a, [w_camera_limit_x]
    dec a
    cp a, b
    jr nc, :+
        ld b, a
        inc b
        ld c, 0
        jr .vertical
    :

    .vertical
    ;OOB check at the top
    ld a, $F0 ;"lowest" camera X-position - 1
    cp a, d
    jr nc, :+
        xor a
        ld d, a
        ld e, a
        jr .store
    :

    ;OOB check at the bottom
    ld a, [w_camera_limit_y]
    dec a
    cp a, d
    jr nc, :+
        ld d, a
        inc d
        ld e, 0
        jr .store
    :

    ;Store these positions in WRAM
    .store
    ld hl, w_camera_x
    ld a, b
    ld [hl+], a
    ld a, c
    ld [hl+], a
    ld a, d
    ld [hl+], a
    ld a, e
    ld [hl+], a

    ;Return
    ret
;