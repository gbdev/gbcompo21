SECTION "SNIPPETS", ROM0

; Switches bank and calls a given address.
; Does NOT switch banks back after returning.
; Usefull when bankjumping from a non-bankable area.
;
; Input:
; - `b` = ROM bank number
; - `hl` = Address to jump to
;
; Destroys: `a`, unknown
bank_call_0::

    ;Switch banks
    ld a, b
    ldh [h_bank_number], a
    ld [$2000], a

    ;Jump
    jp hl
;



; Switches bank and calls a given address.
; Switches banks back after returning.
;
; Input:
; - `b` = ROM bank number
; - `hl` = Address to jump to
;
; Destroys: `af`, `de`, unknown
bank_call_x::

    ;Set up things for returning
    ldh a, [h_bank_number]
    push af
    ld de, bank_return
    push de

    ;Switch banks
    ld a, b
    ldh [h_bank_number], a
    ld [$2000], a

    ;Jump
    jp hl
;



; Switches bank and calls a given address.
; Switches banks back after returning.
; Also saves and restores WRAMX bank on GBC.
;
; Input:
; - `d` = ROM bank number
; - `hl` = Address to jump to
;
; Destroys: `af`, `de`, unknown
bank_call_xd::

    ;Store current bank number
    ldh a, [h_bank_number]
    ld e, a

    ;Switch banks
    ld a, d
    ldh [h_bank_number], a
    ld [$2000], a
    push de

    ;Set up for returning
    ld de, bank_return_d
    push de

    ;Jump
    jp hl
;



;DO NOT CALL MANUALLY! 
;Return address for `bank_call`. Switches bank and returns
;
; Destroys: `af`
bank_return:

    ;Returning after jump, reset bank number
    pop af
    ldh [h_bank_number], a
    ld [$2000], a

    ;Return
    ret
;



; DO NOT CALL MANUALLY! 
; Return address for `bank_call_xd`. Switches bank and returns
;
; Destroys: `af`, `de`
bank_return_d:

    ;Returning after jump, reset banks
    pop de
    ld a, e
    ldh [h_bank_number], a
    ld [$2000], a
    
    ;Return
    ret 
;



; Literally just jumps to the address of HL
; 
; Input:
; - `hl` = Address to jump to
; 
; Destroys: unknown
_hl_::
    jp hl