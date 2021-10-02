;Regular chunk types
ct_nopath equ $00
ct_normal equ $01
ct_dropdown equ $02
ct_dropin equ $03
ct_vertical equ $04

ct_tall_t equ $05
ct_tall_b equ $06
ct_wide_i equ $07
ct_wide_o equ $08

ct_wide_nopath_i equ $09
ct_wide_nopath_o equ $0A
ct_tall_nopath_t equ $0B
ct_tall_nopath_b equ $0C

ct_invalid equ $FF



;Property bits
ctb_entrance equ $7
ctb_exit equ $6



;Chunk properties
cpb_mirror equ $0
cpb_wide equ $1
cpb_tall equ $2

;0-1(bit 1r) = right
;2-3(bit 1s) = left
;4 = down