;Caveman states
RSRESET
caveman_state_idle rb 1
caveman_state_walk rb 1
caveman_state_angry rb 1
caveman_state_thrown rb 1
caveman_state_getup rb 1
;

;Caveman constants
caveman_walktime equ $44
caveman_idletime equ $50
caveman_walkspeed equ $0A
caveman_runspeed equ $18
caveman_width equ $A0
caveman_height equ $B0
caveman_health equ $03