;Used with `w_player_direction` and `w_player_vspp`
physics_going_up equ 0

;Used with `w_player_direction` and `w_player_hspp`
physics_going_left equ 1

;Gravity pull
physics_gravity equ $03

;Max speed
physics_maxspeed equ $60

;Speed to be applied when bounced on
physics_bounce_x equ $10

;Speed to be applied when bounced on
physics_bounce_y equ $20

;How long should an entity be in tumble?
physics_tumbletime equ $FF

;Global tumble state
gstate_tumble equ $FF

; Macro for converting world coordinates into a level pointer.
; 
; Input:
; - 1: (r8) High X-position
; - 2: (r8) High Y-position
; - 3: (r8) Output high
; - 4: (r8) Output low
;
; Output:
; - 3: Output high
; - 4: Output low
;
; Destroys: `a`, 1, 3, 4
coordinates_level: MACRO
    xor a ;Resets carry flag
    ld \4, a
    rr \2
    rra
    rr \2
    rra
    add a, \1
    ld \4, a
    ld a, \2
    add a, high(level_foreground)
    ld \3, a
ENDM