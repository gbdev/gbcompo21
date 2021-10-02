;Used with `w_player_direction` and `w_player_vspp`
player_going_up equ 0

;Used with `w_player_direction` and `w_player_hspp`
player_going_left equ 1

;Used with `w_player_direction`. Dictates sprite X-flip.
player_facing_left equ 2

;Used with `w_player_direction`. Whip state (0 = backwhip, 1 = forward whip)
player_whip_state equ 3

;Used with `w_player_direction`. Something to do with ladder-grabbing
player_laddertouch equ 4



;Player width in subblocks
player_width equ $80

;Player height in subblocks
player_height equ $B0

;Player physics stuff
player_accel equ $10
player_maxspeed equ $20
player_friction equ player_accel >> 1
player_gravity equ $03
player_jump_force equ $17
player_maxspeed_fall equ $60
player_jumptimer equ $0B
player_ledgehang_offset equ $40
player_ladder_speed equ $10
player_stun_time equ $16
player_stun_friction equ $0A
player_stun_pushX equ $50
player_stun_pushY equ $1A
player_invincible_time equ $64

;Whip timer
player_whip_timer equ 21

;Player state
RSRESET
pstate_default rb 1
pstate_whip rb 1
pstate_ledge rb 1
pstate_ladder rb 1
pstate_stun rb 1

;Player sprite ID's
RSRESET
panim_idle rb 1
panim_walk rb 8
panim_whip rb 5
panim_throw equ panim_whip
panim_jump rb 2
panim_jump_begin equ panim_jump
panim_jump_end equ panim_jump + 1
panim_hang rb 1
panim_climb_idle rb 1
panim_climb rb 6
panim_invalid rb 1

;Relative player sprite addresses
panima_idle equ panim_idle * $40
panima_walk equ panim_walk * $40
panima_whip equ panim_whip * $40
panima_throw equ panim_throw * $40
panima_jump equ panim_jump * $40
panima_jump_begin equ panim_jump_begin * $40
panima_jump_end equ panim_jump_end * $40
panima_hanging equ panim_hang * $40
panima_climb_idle equ panim_climb_idle * $40
panima_climb equ panim_climb * $40

;Player palettes (obj palettes)
player_palette_main equ 0
player_palette_whip equ 1