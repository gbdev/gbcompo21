;Entity system is enitialized in `variables_init`.

;Entity size in bytes
entity_size equ $40
entity_count equ $3F

;Constant values I need
_jr equ $18
_next equ entity_size - 2
_ldan8 equ $3E

;Relative entity pointers
RSRESET
entity_pointer rb 0
entity_execute rb 15
entity_allocate rb 6
entity_collision rb 12
entity_variables rb 28
entity_destroy rb 3
entity_end rb 0

;Entity variables
RSSET entity_variables
entity_state rb 1
entity_x rb 2
entity_y rb 2
entity_direction rb 1
entity_hspp rb 1
entity_vspp rb 1
entity_health rb 1
entity_width rb 2
entity_height rb 2
entity_visible rb 1
entity_sprite rb 1
entity_grounded rb 1
entity_timer rb 1
entity_variable_end rb 0
entity_variable_slack equ entity_size - entity_variable_end
ASSERT entity_end >= entity_variable_end

;Entity collision mask bits
RSRESET

;Visibility flag, is culled by the culling system.
entsys_col_visibleB rb 1
entsys_col_visibleF equ 1 << entsys_col_visibleB

;Treasure flag, entity will be destroyed by the player when touched.
entsys_col_treasureB rb 1
entsys_col_treasureF equ 1 << entsys_col_treasureB

;Enemy flag, player will take damage
entsys_col_enemyB rb 1
entsys_col_enemyF equ 1 << entsys_col_enemyB

;Bounce flag, entity can be jumped on
entsys_col_bounceB rb 1
entsys_col_bounceF equ 1 << entsys_col_bounceB

;Damagable flag, takes damage from whip and jumping
entsys_col_damagableB rb 1
entsys_col_damagableF equ 1 << entsys_col_damagableB

;Whipable flag, used for whipping
entsys_col_whipB rb 1
entsys_col_whipF equ 1 << entsys_col_whipB



;`entity_visible` bits
entsys_visible_newB equ 7
entsys_visible_newF equ 1 << entsys_visible_newB
entsys_visible_currentB equ 6
entsys_visible_currentF equ 1 << entsys_visible_currentB