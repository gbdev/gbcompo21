; w_screen_update_enable:
; Only the upper 5 bits of this register are used.
; Used by camera update routines to figure out if the screen needs to be updated,
; and in what orientation.
camb_vertical equ 7
camb_moved_up equ 6
camb_moved_left equ 5
camb_update equ 4
camb_update_list equ 3

; w_cam_update_x / w_cam_update_y:
; Used by the screen update routines to figure out which row/column to update.

; w_camera_x / w_camera_y:
; Stores the camera's current X/Y position.
; The high byte is what block the camera is on, the low byte is precision.

; w_camera_x_last / w_camera_y_last:
; Used by the screen update routine to test if the camera has moved into a new block.

; w_camera_limit_x / w_camera_limit_y:
; Used by camera handling routines to limit the camera's movement.
; The highest block number the camera can reach.

; w_camera_ulimit_x / w_camera_ulimit_y:
; Used by screen update routines.
; Used to limit what blocks are read when trying to update the screen.