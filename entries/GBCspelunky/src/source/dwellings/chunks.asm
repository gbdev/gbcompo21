INCLUDE "banks.inc"

SECTION "DWELLINGS CHUNKS", ROMX, BANK[bank_dwellings_main]

;Chunk structs lookup
dwellings_chunk_struct_lookup::
    dw dwellings_chunk_struct
    dw dwellings_chunk_struct_entry
    dw dwellings_chunk_struct_exit
    dw dwellings_chunk_struct_segment
;



;Main chunk structure
dwellings_chunk_struct::
    ;Nopath
    db 14, 80, 10, 8
    dw dwellings_chunk_nopath

    ;Normal
    db 7, 80, 10, 8
    dw dwellings_chunk_normal

    ;Dropdown
    db 3, 80, 10, 8
    dw dwellings_chunk_dropdown

    ;Dropin
    db 3, 80, 10, 8
    dw dwellings_chunk_dropin

    ;Vertical
    db 7, 80, 10, 8
    dw dwellings_chunk_vertical
;

;Entry chunk structure
dwellings_chunk_struct_entry::
    ;Nopath
    db 0, 0, 0, 0
    dw $0038

    ;Normal
    db 3, 80, 10, 8
    dw dwellings_chunk_entry_normal

    ;Dropdown
    db 2, 80, 10, 8
    dw dwellings_chunk_entry_dropdown

    ;Dropin
    db 0, 0, 0, 0
    dw $0038
;

;Exit chunk structure
dwellings_chunk_struct_exit::
    ;Nopath
    db 0, 0, 0, 0
    dw $0038

    ;Normal
    db 1, 80, 10, 8
    dw dwellings_chunk_exit_normal

    ;Dropdown
    db 0, 0, 0, 0
    dw $0038

    ;Dropin
    db 1, 80, 10, 8
    dw dwellings_chunk_exit_dropin
;

;Segment chunk structure
dwellings_chunk_struct_segment::
    ;Ground
    db 6, 15, 5, 3
    dw dwellings_segment_ground

    ;Air
    db 9, 15, 5, 3
    dw dwellings_segment_air

    ;Door
    db 3, 18, 6, 3
    dw dwellings_segment_door
;



;Regular chunks

;Dwellings chunk data pointer
dwellings_chunk_nopath:
    INCBIN "dwellings/chunks/nopath_1.chu"
    INCBIN "dwellings/chunks/nopath_2.chu"
    INCBIN "dwellings/chunks/nopath_3.chu"
    INCBIN "dwellings/chunks/nopath_4.chu"
    INCBIN "dwellings/chunks/nopath_5.chu"
    INCBIN "dwellings/chunks/nopath_6.chu"
    INCBIN "dwellings/chunks/nopath_7.chu"
    INCBIN "dwellings/chunks/nopath_8.chu"
    INCBIN "dwellings/chunks/nopath_9.chu"
    INCBIN "dwellings/chunks/nopath_10.chu"
    INCBIN "dwellings/chunks/nopath_11.chu"
    INCBIN "dwellings/chunks/nopath_12.chu"
    INCBIN "dwellings/chunks/nopath_13.chu"
    INCBIN "dwellings/chunks/nopath_14.chu"

;Dwellings chunk data pointer
dwellings_chunk_normal:
    INCBIN "dwellings/chunks/normal_1.chu"
    INCBIN "dwellings/chunks/normal_2.chu"
    INCBIN "dwellings/chunks/normal_3.chu"
    INCBIN "dwellings/chunks/normal_4.chu"
    INCBIN "dwellings/chunks/normal_5.chu"
    INCBIN "dwellings/chunks/normal_6.chu"
    INCBIN "dwellings/chunks/normal_7.chu"

;Dwellings chunk data pointer
dwellings_chunk_dropdown:
    INCBIN "dwellings/chunks/dropdown_1.chu"
    INCBIN "dwellings/chunks/dropdown_2.chu"
    INCBIN "dwellings/chunks/dropdown_3.chu"

;Dwellings chunk data pointer
dwellings_chunk_vertical:
    INCBIN "dwellings/chunks/vertical_1.chu"
    INCBIN "dwellings/chunks/vertical_2.chu"
    INCBIN "dwellings/chunks/vertical_3.chu"
    INCBIN "dwellings/chunks/vertical_4.chu"
    INCBIN "dwellings/chunks/vertical_5.chu"
    INCBIN "dwellings/chunks/vertical_6.chu"
    INCBIN "dwellings/chunks/vertical_7.chu"

;Dwellings chunk data pointer
dwellings_chunk_dropin:
    INCBIN "dwellings/chunks/dropin_1.chu"
    INCBIN "dwellings/chunks/dropin_2.chu"
    INCBIN "dwellings/chunks/dropin_3.chu"
;



;Entrance and exit chunks

;Dwellings chunk data pointer
dwellings_chunk_entry_normal:
    INCBIN "dwellings/chunks/entry_normal_1.chu"
    INCBIN "dwellings/chunks/entry_normal_2.chu"
    INCBIN "dwellings/chunks/entry_normal_3.chu"

;Dwellings chunk data pointer
dwellings_chunk_entry_dropdown:
    INCBIN "dwellings/chunks/entry_dropdown_1.chu"
    INCBIN "dwellings/chunks/entry_dropdown_2.chu"

;Dwellings chunk data pointer
dwellings_chunk_exit_normal:
    INCBIN "dwellings/chunks/exit_normal_1.chu"
    
;Dwellings chunk data pointer
dwellings_chunk_exit_dropin:
    INCBIN "dwellings/chunks/exit_dropin_1.chu"
;



;Tall and wide chunks

;Dwellings chunk data pointer
dwellings_chunk_wide_path:
    INCBIN "dwellings/chunks/wide_path_1.chu"
    INCBIN "dwellings/chunks/wide_path_2.chu"

;Dwellings chunk data pointer
dwellings_chunk_wide_nopath:
    INCBIN "dwellings/chunks/wide_nopath_1.chu"

;Dwellings chunk data pointer
dwellings_chunk_tall_path:
    INCBIN "dwellings/chunks/tall_path_1.chu"

;Dwellings chunk data pointer
dwellings_chunk_tall_nopath:
    INCBIN "dwellings/chunks/tall_nopath_1.chu"
;



;Segments

;Dwellings chunk data pointer
dwellings_segment_ground:
    INCBIN "dwellings/chunks/seg_ground_1.sec"
    INCBIN "dwellings/chunks/seg_ground_2.sec"
    INCBIN "dwellings/chunks/seg_ground_3.sec"
    INCBIN "dwellings/chunks/seg_ground_4.sec"
    INCBIN "dwellings/chunks/seg_ground_5.sec"
    INCBIN "dwellings/chunks/seg_ground_6.sec"

;Dwellings chunk data pointer
dwellings_segment_air:
    INCBIN "dwellings/chunks/seg_air_1.sec"
    INCBIN "dwellings/chunks/seg_air_2.sec"
    INCBIN "dwellings/chunks/seg_air_3.sec"
    INCBIN "dwellings/chunks/seg_air_4.sec"
    INCBIN "dwellings/chunks/seg_air_5.sec"
    INCBIN "dwellings/chunks/seg_air_6.sec"
    INCBIN "dwellings/chunks/seg_air_7.sec"
    INCBIN "dwellings/chunks/seg_air_8.sec"
    INCBIN "dwellings/chunks/seg_air_9.sec"

;Dwellings chunk data pointer
dwellings_segment_door:
    INCBIN "dwellings/chunks/seg_door_1.sec"
    INCBIN "dwellings/chunks/seg_door_2.sec"
    INCBIN "dwellings/chunks/seg_door_3.sec"
;