;BLOCK ID'S
RSRESET
b_bg rb 1
b_dirt rb 1
b_ladder rb 1
b_spikes rb 1
b_ladderplatform rb 1

b_platform rb 1
b_wood rb 1
b_wood_bg rb 1
b_pushblock rb 1
b_arrowtrap_left rb 1
b_altar rb 1
b_boneblock rb 1
b_platform_support rb 1
b_arrowtrap_right rb 1

b_border rb 1
b_backdoor rb 1
b_campfire rb 1 ;Do not use

b_door_left rb 1
b_door_right rb 1
b_door_middle_entrance rb 1
b_door_middle_exit rb 1

b_spikes_support rb 1
b_spikes_wood rb 1
b_door_left_support rb 1
b_door_right_support rb 1
b_door_left_wood rb 1
b_door_right_wood rb 1



;Macroblocks
RSSET $80
b_chunk_air rb 1
b_chunk_ground rb 1
b_ground_chance rb 1
b_door_macro rb 1
b_door_macro2 rb 1
b_chunk_door rb 1
b_caveman_sleeping rb 1 ;Do not use
b_barrier_chance rb 1

;Do not use these
b_idol rb 1
b_subdoor rb 1
b_enemy_25 rb 1
b_enemy_75 rb 1
b_money_25 rb 1
b_money_50 rb 1
b_money_75 rb 1
b_money_100 rb 1
b_createsub_0 rb 1
b_createsub_1 rb 1
b_createsub_2 rb 1
b_createsub_3 rb 1
b_createsub_4 rb 1
b_createsub_5 rb 1
b_createsub_6 rb 1
b_createsub_7 rb 1



;TILE ID'S
t_dirt equ $00
t_dirt_top equ $00
t_dirt_bottom equ $08

t_bg equ $10
t_border equ $20

t_pushblock equ $30
t_arrowtrap equ $34
t_altar equ $38
t_boneblock equ $3C

t_wood equ $40
t_wood_top equ t_wood
t_wood_topleft equ t_wood_top
t_wood_topright equ t_wood_top + $08
t_wood_bottom equ t_wood + $10
t_wood_bottomleft equ t_wood_bottom
t_wood_bottomright equ t_wood_bottom + $08

t_wood_bg equ $45
t_ladderplatform equ $4D
t_ladder equ $4F
t_platform equ $55
t_platform_support equ $57
t_spikes equ $5D
t_unknown equ $5F

t_door equ $60
t_door_side_t equ $60
t_door_side_b equ $62
t_door_empty equ $64
t_door_top equ $65
t_door_blockade equ $66



;PALETTE ID'S
p_dirt equ 0
p_dirt_top equ 0
p_dirt_bottom equ 1
p_bg equ 2
p_platform equ 1
p_ladder equ 1
p_pushblock equ 5
p_arrowtrap equ 5
p_wood equ 0
p_altar equ 6
p_wood_bg equ 4
p_boneblock equ 3
p_spikes equ 3
p_border equ 7
p_door equ 0
p_door_blockade equ 6
p_door_empty equ 0

p_unknown equ 6


;Sprites
RSRESET
s_player rb $04
s_whip rb $08
s_button rb $04
s_entities rb $00
s_caveman rb $1C
s_caveman_idle equ s_caveman
s_caveman_walk equ s_caveman + $04
s_caveman_dead equ s_caveman + $14
s_caveman_tumble equ s_caveman + $18
s_snake rb $10
s_treasure rb $14
s_treasure_goldpile equ s_treasure
s_treasure_goldbar equ s_treasure + $04
s_treasure_emerald equ s_treasure + $08

;Sprite palettes
p_player equ 0
p_whip equ 1
p_caveman equ 2
p_snake equ 3
p_gold equ 4
p_emerald equ 3