INCLUDE "hardware.inc"
INCLUDE "banks.inc"
INCLUDE "player.inc"
INCLUDE "entitysystem.inc"

;Allocate 256 bytes for the stack, just to be safe
stack_size equ $100
SECTION "STACK", WRAM0[$D000 - stack_size]
w_stack_begin:: ds stack_size
w_stack:: ds $00
ASSERT w_stack_begin + stack_size == $D000

;Foreground level data
SECTION "MAP FOREGROUND", WRAMX, BANK[1]
level_foreground:: ds $1000

;Background level data
SECTION "MAP BACKGROUND", WRAMX, BANK[2]
level_background:: ds $1000


SECTION "VARIABLE INITIALIZATION", ROMX, BANK[bank_variables]

; Initializes ALL variables.
variables_init::
    
    ;Copy WRAM0 variables
    ld hl, w_variables ;Start of variable space
    ld bc, var_data_w0 ;Initial variable data
    ld de, var_data_w0_end - var_data_w0 ;Data length
    call memcopy

    ;Copy entity system to WRAM3
    ld a, bank_entsys
    ldh [rSVBK], a
    ld hl, w_entsys
    ld bc, var_data_w3
    ld de, $1000
    call memcopy

    ;Copy HRAM variables
    ld hl, h_dma_routine
    ld bc, var_data_h ;Initial variable data
    ld de, var_data_h_end - var_data_h ;Data length
    call memcopy

    ;Return
    ret
;



; Contains the initial values of all variables in WRAM0.
var_data_w0:
    LOAD "WRAM0 VARIABLES", WRAM0, ALIGN[8]
        w_variables:
    
        ;Blockdata
        w_blockdata::
            REPT $100
            db $00
            ENDR
        ;

        ;Tile update queue
        w_screen_update_list_count:: db $00
        w_screen_update_list_head:: db $02
        w_screen_update_list::
            REPT $7F
            dw $FFFF
            ENDR
        ;

        ;Sprite stuff
        w_oam_mirror::
            REPT $A4
            db $00
            ENDR
        
        ASSERT low(w_oam_mirror) == 0
        w_spriteslot_lowest:: db $03
        ;

        ;Global variables
        w_globals::
        w_world_bank:: db bank_dwellings_main
        w_layer_bank:: db bank(level_foreground)
        w_debug_window:: db $00 ;none
        w_money:: dl $00000000
        w_level_number:: db $01
        w_level_timer:: db $3C, $30, $01
        w_player_health_last:: db $FF
        ;

        ;Level data
        w_level::
        w_chunk_buffer::
            REPT $50
            db $00
            ENDR

        w_chunk_type::
            REPT 36
            db $00
            ENDR
        
        w_chunk_info::
            REPT 36
            db $00
            ENDR
        
        w_level_width:: db $28
        w_level_height:: db $20
        w_level_width_chunks:: db $04
        w_level_height_chunks:: db $04
        w_level_entrance_x:: db $00
        w_level_entrance_y:: db $00
        w_chunk_id:: db $00
        w_chunk_x:: db $00
        w_chunk_y:: db $00
        w_chunk_last:: db $00
        ;

        ;Physics entity buffer
        w_intro_color_buffer::
        w_collision_buffer:: dw $0000, $0000, $0000, $0000, $0000, $0000, $0000, $0000
        ;

        ;Camera variables
        w_screen_update_enable:: db $00
        w_cam_update_x:: db $00
        w_cam_update_y:: db $00
        w_camera_position::
        w_camera_x:: dw $0000
        w_camera_y:: dw $0000
        w_camera_x_last:: db $00
        w_camera_y_last:: db $00
        w_camera_limit_x:: db $00
        w_camera_limit_y:: db $00
        w_camera_ulimit_x:: db $00
        w_camera_ulimit_y:: db $00
        w_camera_cull::
        w_camera_cull_x:: dw $0000
        w_camera_cull_y:: dw $0000
        w_camera_cull_x2:: dw $0000
        w_camera_cull_y2:: dw $0000
        ASSERT high(w_camera_cull) == high(@)
        ;

        ;Player variables
        w_player::
        w_player_state:: db pstate_default
        w_player_x:: dw $0100
        w_player_y:: dw $0000
        w_player_direction:: db $00
        w_player_hspp:: db $00
        w_player_vspp:: db $00
        w_player_sprite:: db $A0 ;invalid sprite
        w_player_health:: db $04
        w_player_jumps_remaining:: db $01 ;one jump
        w_player_jumps_total:: db $01 ;one jump
        w_player_grounded:: db $01 ;Isn't grounded
        w_player_animation:: db $00
        w_player_invincible:: db player_invincible_time
        w_player_tick:: db $00
        ASSERT high(w_player) == high(@)
        ;

        ;That intro thing
        w_intro_state:: db $00
        w_intro_timer:: db $00
        ;
    ENDL
    var_data_w0_end:
;

;Contains initial values for all entities.
var_data_w3:
    LOAD "WRAM3 VARIABLES", WRAMX, BANK[3]
        
        ;Regular entities
        w_entsys::
            REPT 63
                
                ;Execute
                IF !DEF(w_entsys_execute)
                    w_entsys_execute::
                ENDC
                w_entsys_execute_\@:
                jr @+(66 - 2) ;By default, entity is disabled
                ld de, (@ & $FFC0) | entity_variables ;Load DE with entity data pointer
                ld b, $FF ;Bank in which entity's code is stored
                ld hl, $FFFF ;Code address
                call bank_call_0 ;Bank switch and jump
                jr @+(66 - 15) ;Go to next entity

                ;Allocate
                IF !DEF(w_entsys_alloc)
                    
                    ; Loops through each entity in WRAMX.
                    ; Crash screen appears if no entity slots are free.
                    ;
                    ; Output:
                    ; `hl`: Starting address of free entity
                    w_entsys_alloc::
                    ;
                ENDC
                w_entity_alloc_\@:
                db $00, $00
                ld hl, (@ & $FFC0) | entity_pointer
                ret 

                ;Collision
                IF !DEF(w_entsys_collision)
                    w_entsys_collision::
                ENDC
                w_entity_collision_\@:
                jr @+(66 - 2)
                and a, c
                jr z, @+(66 - 5) ;Jump if collision mask doesn't match
                ld de, (@ & $FFC0) | entity_variables ;Load own variables
                ld hl, $40 + (@ & $FFC0) | entity_collision ;Store next entity address in HL
                ret

                ;Variables
                w_entsys_variables_\@:
                w_entity_state_\@: db $00
                w_entity_x_\@: dw $000C
                w_entity_y_\@: dw $0006
                w_entity_direction_\@: db $00
                w_entity_hspp_\@: db $00
                w_entity_vspp_\@: db $00
                w_entity_health_\@: db $FF
                w_entity_width_\@: dw $00FF
                w_entity_height_\@: dw $00FF
                w_entity_visible_\@: db $00
                w_entity_sprite_\@: db $A0
                w_entity_grounded_\@: db $00
                w_entity_timer_\@: db $00
                
                ;Slack
                ds entity_variable_slack
            ENDR
        ;

        ;Terminator entity
        w_entsys_terminator::
            
            ;Execute
            w_entsys_terminator_execute::
            ret 
            ds 14

            ;Allocate
            w_entsys_terminator_alloc::
            ld h, $FF
            ret 
            ds 3

            ;Collision
            w_entsys_terminator_collision::
            ld d, $FF
            ret
            ds 12

            ;Variables
            ds 28
        ;
    ENDL
;

; Contains the initial values for all HRAM variables.
var_data_h:
    LOAD "HRAM VARIABLES", HRAM
        h_variables::

        ;OAM DMA routine in HRAM
        h_dma_routine::
            
            ;Initialize OAM DMA
            ld a, HIGH(w_oam_mirror)
            ldh [rDMA], a

            ;Wait until transfer is complete
            ld a, 40
            .wait
            dec a
            jr nz, .wait

            ;Return
            ret
        ;

        ;Input
        h_input:: db $FF
        h_input_pressed:: db $00
        ;

        ;Important system variables
        h_setup:: db $FF
        h_is_color:: db $FF
        h_bank_number:: db bank_dwellings_main
        ;

        ;Shadow scrolling registers
        h_scx:: db $00
        h_scy:: db $00

        ;Temporary variables
        h_temp::
        h_temp1:: db $00
        h_temp2:: db $00
        h_temp3:: db $00
        h_temp4:: db $00
        h_temp5:: db $00
        h_temp6:: db $00
        h_temp7:: db $00
        h_temp_tile:: ;Used with screen scrolling routines
        h_temp8:: db $00
        h_temp9:: db $00
        h_tempA:: db $00
        ;

        ;RNG stuff
        h_rng::
        h_rng_seed:: db $7E, $B2
        h_rng_out:: db $00, $00
        ;
    ENDL
    var_data_h_end:
;