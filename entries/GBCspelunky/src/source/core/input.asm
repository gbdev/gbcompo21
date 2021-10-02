INCLUDE "hardware.inc"

SECTION "INPUT", ROM0
; Gets the current buttons pressed.
; Bits 0-3 = buttons, bits 4-7 = dpad.
;
; Output:
; - `b`: Byte of buttons held
; - `c`: Byte of buttons pressed
;
; Destroys: `af`, `bc`, `d`
input::
    ;Previous buttons pressed
    ldh a, [h_input]
    ld d, a

;Read the buttons
    ;Set up for reading the buttons
    ld c, 0
    ld a, %00100000
    ld [$FF00+c], a
    ld [$FF00+c], a
    nop 

    ;Read the buttons
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    swap a
    and a, %11110000
    ld b, a
;

;Read the DPAD
    ;Set up for reading the DPAD
    ld a, %00010000
    ld [$FF00+c], a
    ld [$FF00+c], a
    nop 

    ;Read the DPAD
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    ld a, [$FF00+c]
    and a, %00001111
    or a, b
    cpl
    ld b, a

    ;Get buttons pressed
    ldh [h_input], a
    ld c, a
    xor a, d
    and a, c
    ld c, a
    ldh [h_input_pressed], a

    ;Reset input register and return
    ld a, $FF
    ldh [rP1], a
    ret