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

#ifndef __player_h_INCLUDE
#define __player_h_INCLUDE
#include "misc.h"
/* functions */
extern void update_player(void);
extern void draw_player(void);
extern void charge(void);
extern boolean zap(void);
boolean player_collide_map();
boolean player_collide_box();
void pick_player_animation(void);
void move_player_sprite(void);
void update_screen_xy(void);
void update_camera(void);
extern boolean player_collide_npc();
extern void advance_level(void);
extern void enter_level(void);

/* types */
enum direction {north, south, east, west};
enum char_state {standing, running, pushing, teleporting, charging, zapped};
//enum camera_type {fixed_camera, moving_camera, scrolling_camera};

/* data */
extern enum direction player_face;
extern enum char_state player_state;
extern uint8_t player_x;
extern uint8_t player_y;
extern uint8_t screen_x;
extern uint8_t screen_y;
extern uint8_t push_count;
extern boolean can_teleport;
extern uint8_t animation_t;
extern boolean player_win;

/* calculate common values once per frame */
extern uint8_t player_hit_x1;
extern uint8_t player_hit_x2;
extern uint8_t player_hit_y1;
extern uint8_t player_hit_y2;
extern uint8_t player_hit_mx1;
extern uint8_t player_hit_mx2;
extern uint8_t player_hit_mxmid;
extern uint8_t player_hit_my1;
extern uint8_t player_hit_my2;
extern uint8_t player_hit_tile_ul;
extern uint8_t player_hit_tile_ur;
extern uint8_t player_hit_tile_ll;
extern uint8_t player_hit_tile_lr;
extern uint8_t player_hit_tile_mid;

#define UPDATE_HITBOX {\
	player_hit_x1 = player_x + 2;\
	player_hit_x2 = player_x + 13;\
	player_hit_y1 = player_y + 7;\
	player_hit_y2 = player_y + 15;\
	player_hit_mx1 = player_hit_x1 >> 3;\
	player_hit_mx2 = player_hit_x2 >> 3;\
	player_hit_my1 = player_hit_y1 >> 3;\
	player_hit_my2 = player_hit_y2 >> 3;\
	player_hit_mxmid = (player_hit_x1 + 6) >> 3;\
}

#define UPDATE_MAP_COLLISIONS {\
	player_hit_tile_ul = get_bkg_tile_xy(player_hit_mx1, 	player_hit_my1);\
	player_hit_tile_ur = get_bkg_tile_xy(player_hit_mx2, 	player_hit_my1);\
	player_hit_tile_ll = get_bkg_tile_xy(player_hit_mx1, 	player_hit_my2);\
	player_hit_tile_mid = get_bkg_tile_xy(player_hit_mxmid, player_hit_my2);\
	player_hit_tile_lr = get_bkg_tile_xy(player_hit_mx2, 	player_hit_my2);\
}

#endif
