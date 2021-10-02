/*
    Copyright 2021 Gearóid Fox

    This file is part of <corrib75>.

    <corrib75> is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    <corrib75> is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with <corrib75>.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __level_h_INCLUDE
#define __level_h_INCLUDE

#include "misc.h"

#define NLEVELS 12
#define blocking_tiles 48
#define wall_tiles 31
#define box_tiles 127
#define map_w 32
#define map_h 32
#define max_boxes 6
#define max_teleporters 4


extern uint8_t current_level;

extern uint8_t uncovered_floors[23][4];
void uncover_square(uint8_t n, uint8_t x, uint8_t y);

extern void load_level(uint8_t level_number);
extern void draw_boxes();
extern boolean check_win(void);
extern void reset_level(void);
void fade_to_black(void);

/* global pointer to the current level map */
extern const uint8_t *level_ptr;
extern const uint8_t *level_ptr1;

extern uint8_t level_bank;
extern uint8_t camera_x, camera_y;
extern uint8_t exit_mx, exit_my;
extern uint8_t box_x;
extern uint8_t box_y;
extern uint8_t box_map_x;
extern uint8_t box_map_y;
extern int8_t box_x_offset;
extern int8_t box_y_offset;
enum box_types {arrow, question};
extern enum box_types box_type;

extern uint8_t nspaces;
extern uint8_t filled_spaces;

volatile extern struct box boxes[max_boxes];
extern uint8_t nboxes;

extern uint8_t ntele;
extern uint16_t teleporters[];

extern boolean fixed_camera;

extern boolean mask_box_dl; 
extern boolean mask_box_dr;
extern boolean mask_box_sl; 
extern boolean mask_box_sr;

struct level_data {
	uint8_t *pln0;
	uint8_t *pln1;
	uint8_t *floor_pal_rgb;
	uint8_t *wall_pal_rgb;
	char *npc_message;
	uint8_t *npc_portrait;
	uint8_t bank;
	uint8_t *npc_pal_rgb;
	boolean fixed_camera;
};

extern const struct level_data levels_list[];

#endif
