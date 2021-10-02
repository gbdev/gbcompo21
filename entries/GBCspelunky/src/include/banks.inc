;ROMX bank ID's
bank_errorhandler       equ 1
bank_variables          equ bank_errorhandler
bank_dwellings_main     equ 2
bank_player             equ 3
bank_entities           equ bank_player

;WRAMX bank ID's
bank_foreground         equ 1
bank_background         equ 2
bank_entsys             equ 3



; Macro instruction for calling a label.
; Uses `bank_call_x`.
; 
; Input:
; - 1: Label to call
;
; Destroys: `b`, `hl`, unknown
call_bank_m: MACRO
    ld hl, \1
    ld b, bank(\1)
    call bank_call_x
ENDM



; Macro instruction for calling a label.
; Uses `bank_call_0`.
; 
; Input:
; - 1: Label to call
;
; Destroys: `b`, `hl`, unknown
call_bank_m0: MACRO
    ld hl, \1
    ld b, bank(\1)
    call bank_call_0
ENDM