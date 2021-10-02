INCLUDE "hardware.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "tile macros/all.inc"
INCLUDE "banks.inc"

SECTION "DWELLINGS TILETABLE", ROMX, BANK[bank_dwellings_main], ALIGN[8]
dwellings_tiletable::
    jp dwt_bg
    jp dwt_dirt
    jp dwt_ladder
    jp dwt_spikes
    jp dwt_ladderplatform
    jp dwt_platform
    jp dwt_wood
    jp dwt_wood_bg

    jp dwt_pushblock
    jp dwt_arrowtrap_left
    jp dwt_altar
    jp dwt_boneblock
    jp dwt_platform_support
    jp dwt_arrowtrap_right
    jp dwt_border
    jp dwt_unknown

    jp dwt_unknown
    jp dwt_door_left
    jp dwt_door_right
    jp dwt_door_entrance
    jp dwt_door_exit

    jp dwt_spikes_support
    jp dwt_spikes_wood
    jp dwt_door_left_support
    jp dwt_door_right_support
    jp dwt_door_left_wood
    jp dwt_door_right_wood
.edata
;Fill remaining space with 0's to prevent invalid data
REPT (768 - (.edata - dwellings_tiletable)) / 3
    jp dwt_unknown
ENDR


SECTION "DWELLINGS TILE HANDLERS", ROMX, BANK[bank_dwellings_main]
dwt_bg:
    tm_bg4x4 t_bg, p_bg
    ret
;
dwt_dirt:
    tm_ground t_dirt, p_dirt, b_dirt, p_dirt_bottom
    ret 
;
dwt_ladder:
    tm_mirror t_ladder, p_ladder
    ret 
;
dwt_spikes:
    tm_bg4x4_h t_bg, p_bg
    set 5, l
    dec l
    tm_simple_h t_spikes, p_spikes
    res 5, l
    ret
;
dwt_ladderplatform:
    tm_simple_h t_ladderplatform, p_ladder
    set 5, l
    dec l
    tm_mirror_h t_ladder, p_ladder
    res 5, l
    ret 
;
dwt_platform:
    tm_simple_h t_platform, p_platform
    set 5, l
    dec l
    tm_duplicate_h t_platform_support, p_platform
    res 5, l
    ret 
;
dwt_wood:
    tm_floor_experimental t_wood, p_wood, b_wood
    ret
;
dwt_wood_bg:
    tm_bg1strip t_wood_bg, p_wood_bg, b_wood_bg
    ret 
;
dwt_pushblock:
    tm_simple t_pushblock, p_pushblock
    ret 
;
dwt_arrowtrap_left:
    tm_simple t_arrowtrap, p_arrowtrap
    ret 
;
dwt_altar:
    tm_altar t_altar, p_altar
    ret 
;
dwt_boneblock:
    tm_simple t_boneblock, p_boneblock
    ret 
;
dwt_platform_support:
    tm_duplicate t_platform_support, p_platform
    ret 
;
dwt_arrowtrap_right:
    tm_flip t_arrowtrap, p_arrowtrap
    ret 
;
dwt_border:
    tm_bg4x4 t_border, p_border
    ret 
;
dwt_door_left:
    tm_door_left t_door, p_door, b_door_left, t_bg, p_bg
    ret 
;
dwt_door_right:
    tm_door_right t_door, p_door, b_door_right, t_bg, p_bg
    ret 
;
dwt_door_entrance:
    tm_door_blockade t_door_top, p_door, b_door_middle_entrance, t_door_blockade, p_door_blockade
    ret 
;
dwt_door_exit:
    tm_door_open t_door_top, p_door, b_door_middle_exit, t_door_empty, p_door_empty
    ret    
;
dwt_spikes_support:
    tm_duplicate_h t_platform_support, p_platform
    set 5, l
    dec l
    tm_simple_h t_spikes, p_spikes
    res 5, l
    ret
;
dwt_spikes_wood:
    tm_duplicate_h t_wood_bg, p_wood_bg
    set 5, l
    dec l
    tm_simple_h t_spikes, p_spikes
    res 5, l
    ret 
;
dwt_door_left_wood:
    tm_door_left_1strip t_door, p_door, b_door_left_wood, t_wood_bg, p_wood_bg
    ret 
;
dwt_door_right_wood:
    tm_door_right_1strip t_door, p_door, b_door_right_wood, t_wood_bg, p_wood_bg
    ret 
;
dwt_door_left_support:
    tm_door_left_duplicate t_door, p_door, b_door_left_support, t_platform_support, p_platform
    ret 
;
dwt_door_right_support:
    tm_door_right_duplicate t_door, p_door, b_door_right_support, t_platform_support, p_platform
    ret 
;


;dwt_platform:           tile_macro_simple           t_platform,          p_platform
;dwt_platform_support:   tile_macro_duplicate        t_platform_support,  p_platform
;dwt_wood_top:           tile_macro_floor_top        t_wood,              p_wood,       b_wood,     p_wood
;dwt_wood_bottom:        tile_macro_floor_bottom     t_wood_bottom,       p_wood,       b_wood,     p_wood
;dwt_wood_bg:            tile_macro_bg1strip         t_wood_bg,           p_wood_bg,    b_wood_bg
;dwt_pushblock_top:      tile_macro_simple           t_pushblock,         p_pushblock
;dwt_pushblock_bottom:   tile_macro_simple           t_pushblock+2,       p_pushblock
;dwt_arrowtrap_right_t:  tile_macro_simple           t_arrowtrap,         p_arrowtrap
;dwt_arrowtrap_right_b:  tile_macro_simple           t_arrowtrap+2,       p_arrowtrap
;dwt_arrowtrap_left_t:   tile_macro_flip             t_arrowtrap,         p_arrowtrap
;dwt_arrowtrap_left_b:   tile_macro_flip             t_arrowtrap+2,       p_arrowtrap
;dwt_altar_top:          tile_macro_altar            t_altar,             p_altar
;dwt_altar_bottom:       tile_macro_altar            t_altar+2,           p_altar
;dwt_boneblock_top:      tile_macro_simple           t_boneblock,         p_boneblock
;dwt_boneblock_bottom:   tile_macro_simple           t_boneblock+2,       p_boneblock
;dwt_border:             tm_bg4x4            t_border,            p_border


dwt_unknown:
    
    ;Write unknown block tiles to upper tiles
    ld a, t_unknown
    ld [hl+], a
    ld [hl], a
    set 5, l

    ;Write the block's ID in hex to bottom tiles
    ld a, [de]
    and a, $0F
    ld [hl-], a
    ld a, [de]
    swap a
    and a, $0F
    ld [hl+], a

    ;Write palettes
    ld a, 1
    ldh [rVBK], a
    ld a, p_boneblock + OAMF_BANK1
    ld [hl-], a
    ld [hl+], a
    res 5, l
    ld a, p_unknown
    ld [hl-], a
    ld [hl+], a
    xor a
    ldh [rVBK], a

    ;Return
    ret
;