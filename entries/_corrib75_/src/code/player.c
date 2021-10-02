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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <types.h>

#include <gb/gb.h>
#include <gb/cgb.h>

#include "../assets/sprite_tiles.h"

#include "lasers.h"
#include "levels.h"
#include "misc.h"
#include "npc.h"
#include "palettes.h"
#include "player.h"

#pragma bank 0

uint8_t shift1, shift2;
/* project global data */
uint8_t player_x;
uint8_t player_y;
uint8_t screen_x;
uint8_t screen_y;
boolean can_teleport;
enum direction player_face;
enum char_state player_state;
uint8_t push_count;
uint8_t animation_t = 0;
boolean player_win = FALSE;

/* calculate common values once per frame */
uint8_t player_hit_x1;
uint8_t player_hit_x2;
uint8_t player_hit_y1;
uint8_t player_hit_y2;

uint8_t player_hit_mx1;
uint8_t player_hit_mx2;
uint8_t player_hit_mxmid;
uint8_t player_hit_my1;
uint8_t player_hit_my2;

uint8_t player_hit_tile_ul;
uint8_t player_hit_tile_ur;
uint8_t player_hit_tile_ll;
uint8_t player_hit_tile_lr;
uint8_t player_hit_tile_mid;

uint8_t player_mask_mx1;
uint8_t player_mask_mx2;
uint8_t player_mask_mx3;
uint8_t player_mask_mx4;


/* file global data */
static uint8_t map_x, map_y;
static uint8_t tile1;
static size_t index;
static uint8_t i, j;
static int8_t n;
static uint8_t tilel;
static uint8_t tile;
static uint8_t tile2;
static uint8_t blocking_index;
static boolean npc_collide;

/* project global functions */
void update_player(void);
void draw_player(void);
boolean player_collide_npc(void);

/* file local functions */
void movl(void);
void movr(void);
void movu(void);
void movd(void);
void teleport(uint8_t destination_teleporter);



/* player_collide_map()
 * Check if the player is colliding with impassable parts of the map.
 *
 * Call the macros UPDATE_HITBOX and UPDATE_COLLISIONS first to get current data into player_hit_tile_XX. etc.
 */
boolean player_collide_map(void)
{
	/* */
	if (player_state == charging) {
		blocking_index = wall_tiles;
	} else {
		blocking_index = blocking_tiles;
	}
	if (player_hit_tile_ul < blocking_index)  return TRUE;
	else if ((player_state == standing || player_state == running) && player_hit_tile_ul == 112 && nspaces == filled_spaces) player_win = TRUE;
	if (player_hit_tile_ur < blocking_index)  return TRUE;
	if (player_hit_tile_ll < blocking_index)  return TRUE;
	if (player_hit_tile_lr < blocking_index)  return TRUE;
	if (player_hit_tile_mid < blocking_index) return TRUE;
	return FALSE;
}



/* player_collide_box()
 * Check if the player is colliding with a moveable box.
 *
 * May cause boxes to move as a side effect.
 *
 * Call the macros UPDATE_HITBOX and UPDATE_COLLISIONS first to get current data into player_hit_tile_XX. etc.
 */
boolean player_collide_box(void)
{
	/* Collide with moving box sprite: */
	if (box_y > 0) {
		if (player_hit_x1  >= box_x &&
				player_hit_x1  <  box_x + 16 &&
				player_hit_y1  >= box_y &&
				player_hit_y2  <  box_y + 16) return TRUE;
		if (player_hit_x2 >= box_x &&
				player_hit_x2 <  box_x + 16 &&
				player_hit_y1  >= box_y &&
				player_hit_y1  <  box_y + 16) return TRUE;
		if (player_hit_x1  >= box_x &&
				player_hit_x1  <  box_x + 16 &&
				player_hit_y2 >= box_y &&
				player_hit_y2 <  box_y + 16) return TRUE;
		if (player_hit_x2 >= box_x &&
				player_hit_x2 <  box_x + 16 &&
				player_hit_y2 >= box_y &&
				player_hit_y2 <  box_y + 16) return TRUE;
	}
	
	if (player_hit_tile_ul > box_tiles && player_hit_tile_ul < 136) {
		tile = player_hit_tile_ul;
		map_x = player_hit_mx1;
		map_y = player_hit_my1;
	} else if (player_hit_tile_ur > box_tiles && player_hit_tile_ur < 136) {
		tile = player_hit_tile_ur;
		map_x = player_hit_mx2;
		map_y = player_hit_my1;
	} else if (player_hit_tile_ll > box_tiles && player_hit_tile_ll < 136) {
		tile = player_hit_tile_ll;
		map_x = player_hit_mx1;
		map_y = player_hit_my2;
	} else if (player_hit_tile_lr > box_tiles && player_hit_tile_lr < 136) {
		tile = player_hit_tile_lr;
		map_x = player_hit_mx2;
		map_y = player_hit_my2;
	} else {
		/* Didn't collide with a box */
		push_count = 0;
		return FALSE;
	}

	/* Did collide. */
	++push_count;

	/* Push box after a little resistance: */
	if (push_count == 32) { 
		/* Check if the player is blocked from moving by a second box: */
		switch (player_face) {
			case west:
				if (player_hit_tile_ll >= 48 && player_hit_tile_ll < 64) break;
				if ((player_hit_tile_ll - 1) != player_hit_tile_ul) return TRUE;
				break;
			case east:
				if (player_hit_tile_lr >= 48 && player_hit_tile_lr < 64) break;
				if ((player_hit_tile_lr - 1) != player_hit_tile_ur) return TRUE;
				break;
			case north:
				if (player_hit_tile_ur != player_hit_tile_ul + 2) return TRUE;
				break;
			case south:
				if (player_hit_tile_lr != player_hit_tile_ll + 2) return TRUE;
				break;
		}
		/* Determine box type */
		SWITCH_ROM_MBC5(PALETTE_BANK);
		if (tile >= 132) {
			box_type = question;
			set_sprite_palette(7, 1, box2_pal);
		} else {
			box_type = arrow;
			set_sprite_palette(7, 1, box1_pal);
		}

		/* Find the top left tile of the box on the map */
		tile = tile % 4;
		if (tile == 1) {
			--map_y;
		} else if (tile == 2) {
			--map_x;
		} else if (tile == 3) { 
			--map_x;
			--map_y;
		}
		/* Check for an empty space the box can move to: */
		switch (player_face) {
			case west:
				tile = get_bkg_tile_xy(map_x - 2, map_y);
				tile1 = get_bkg_tile_xy(map_x - 2, map_y + 1);
				tile2 = get_bkg_tile_xy(map_x - 1, map_y + 1);
				npc_collide = (npc_mx == map_x - 2 && npc_my == map_y);
				break;
			case east:
				tile = get_bkg_tile_xy(map_x + 2, map_y);
				tile1 = get_bkg_tile_xy(map_x + 2, map_y + 1);
				tile2 = get_bkg_tile_xy(map_x + 3, map_y + 1);
				npc_collide = (npc_mx == map_x + 2 && npc_my == map_y);
				break;
			case north:
				tile = get_bkg_tile_xy(map_x, map_y - 2);
				tile1 = get_bkg_tile_xy(map_x, map_y - 1);
				tile2 = get_bkg_tile_xy(map_x + 1, map_y - 1);
				npc_collide = (npc_mx == map_x && npc_my == map_y - 2);
				break;
			case south:
				tile = get_bkg_tile_xy(map_x, map_y + 2);
				tile1 = get_bkg_tile_xy(map_x, map_y + 3);
				tile2 = get_bkg_tile_xy(map_x + 1, map_y + 3);
				npc_collide = (npc_mx == map_x && npc_my == map_y + 2);
				break;
		}
		if (tile > blocking_tiles && tile < 0x7f && !npc_collide) { /* There is space */
			if (tile >= 98 && tile <= 103) ++filled_spaces;
			mask_box_dl = tile1 >= 48 && tile1 < 64;
			mask_box_dr = tile2 >= 48 && tile2 < 64;
			push_count = 0;
			/* Move sprite in direction player facing */
			box_map_x = map_x;
			box_map_y = map_y;
			switch (player_face) {
				case north:
					box_map_y -= 2;
					box_y_offset = 16;
					break;
				case south:
					box_map_y += 2;
					box_y_offset = -16;
					break;
				case east:
					box_map_x += 2;
					box_x_offset = -16;
					break;
				case west:
					box_map_x -= 2;
					box_x_offset = 16;
			}
			/* displace box from map */
			NR41_REG = 0x13;
			NR42_REG = 0xf1;
			NR43_REG = 0x73;
			NR44_REG = 0xc0;
			box_x = box_map_x << 3;
			box_y = box_map_y << 3;
			move_sprite(16, box_x + box_x_offset + 8   - camera_x, box_y + box_y_offset + 16 - camera_y);
			move_sprite(17, box_x + box_x_offset + 16  - camera_x, box_y + box_y_offset + 16 - camera_y);
			wait(1, level_bank);
			SWITCH_ROM_MBC5(level_bank);
			tile = level_ptr[map_x + map_w * map_y]; 
			if (tile > 0x7F) {  /* space originally held a box or special tile: */
				tile1 = level_ptr[1 + map_x + (map_w * map_y)] - 200; 
				mask_box_sl = tile1 > 5;
				mask_box_sr = mask_box_sl;
				SWITCH_ROM_MBC5(2);
				uncover_square(map_x, map_y, tile1);
			} else { /* space held a non box to start: */
				if (tile >= 98 && tile <= 103) --filled_spaces;
				if (_cpu == CGB_TYPE) {
					VBK_REG = 1;
					set_bkg_submap(map_x, map_y, 2, 2, level_ptr1, map_w);
				}
				mask_box_sl =  level_ptr1[map_x + map_w * (map_y+1)] & 0x80;
				mask_box_sr =  level_ptr1[map_x + map_w * (map_y+1) + 1] & 0x80;
				VBK_REG = 0;
				set_bkg_submap(map_x, map_y, 2, 2, level_ptr, map_w);
			}
			SWITCH_ROM_MBC5(2);
			if(mask_box_sl) {
				set_sprite_data(136, 4, sprites + 252 * 16);
			} else {
				set_sprite_data(136, 4, sprites + 248 * 16);
			}
			wait(1, 2);
		}
	}
	SWITCH_ROM_MBC5(2);
	return TRUE;
}



/* movu(), movd(), movl(), movr(): 
 * Move the player one pixel up, down, left, or right.
 *
 * Updates the coordinates, checks for collisions,
 * then resets the coordinations if the move is invalid.
 *
 * Collision checks may have side effects, like pushing boxes.
 */
void movu(void)
{
	--player_y;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		++player_y;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
	}
	return;
}



void movd(void)
{
	++player_y;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		--player_y;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
	}
	return;
}



void movl(void)
{
	--player_x;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		++player_x;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
	}
	return;
}



void movr(void)
{
	++player_x;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		--player_x;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
	}
	return;
}


boolean player_collide_npc()
{
	if (player_hit_mx2 == npc_mx) {
		/* row 1 */
		if (    (player_hit_my2 == npc_my) ||
				(player_hit_my1 == npc_my) ||
				(player_hit_my1 == npc_my + 1)
		   ) return TRUE;
	} else if (player_hit_mx1 == npc_mx) {
		/* row 2 */
		if (    (player_hit_my2 == npc_my) ||
				(player_hit_my1 == npc_my + 1)
		   ) return TRUE;
	} else if (player_hit_mx1 == npc_mx + 1){
		/* row 3 */
		if (    (player_hit_my2 == npc_my) ||
				(player_hit_my1 == npc_my) ||
				(player_hit_my1 == npc_my + 1)
		   ) return TRUE;
	} else if (player_hit_mxmid == npc_mx) {
		if (    (player_hit_my2 == npc_my) ||
				(player_hit_my1 == npc_my) ||
				(player_hit_my1 == npc_my + 1)
		   ) return TRUE;
	}
	return FALSE;
}



/* update_player()
 *
 * Update player state, face, position based on input
 */
void update_player(void)
{
	if (push_count == 127) push_count = 5;
	player_state = standing;
	if (btn & J_LEFT) {
		/* Movement left w/ or w/o up/down movement: */
		if (btn & J_UP) movu();
		else if (btn & J_DOWN) movd(); 
		movl();
		if (player_face == west) {
			if (push_count > 5)  {
				player_state = pushing; 
			} else {
				player_state = running;
			}
		} else {
			player_face = west;
			animation_t = 0;
		}
	} else if (btn & J_RIGHT) {	
		/* Movement right w/ or w/o up/down movement: */
		if (btn & J_UP) movu();
		else if (btn & J_DOWN) movd(); 
		movr();
		if (player_face == east) {
			if (push_count > 5)  {
				player_state = pushing; 
			} else {
				player_state = running;
			}
		} else {
			player_face = east;
			animation_t = 0;
		}
	} else if (btn & J_UP) {

		/* Only moving up: */
		movu();
		if (player_face == north) {
			if (push_count > 5)  {
				player_state = pushing; 
			} else {
				player_state = running;
			}
		} else {
			player_face = north;
			animation_t = 0;
		}
	} else if (btn & J_DOWN) {	
		/* Only moving down: */
		movd();
		if (player_face == south) {
			if (push_count > 5)  {
				player_state = pushing; 
			} else {
				player_state = running;
			}
		} else {
			player_face = south;
			//push_count = 0;
			animation_t = 0;
		}
	}
	/* Check for teleporter collision and teleport: */
	for(i = 0; i < ntele; ++i) {
		if(	player_x >= teleporters[2 * i    ] - 1 &&
				player_x <  teleporters[2 * i    ] + 3 &&
				player_y >= teleporters[2 * i + 1] - 8 &&
				player_y <  teleporters[2 * i + 1] - 2 ) {
			if (can_teleport) {
				teleport(i);
			}
			can_teleport = FALSE;
			return;
		}
	}
	can_teleport = TRUE;
	return;
}



/* teleport()
 *
 * move the player through a teleporter w/ animation
 *
 */
void teleport(uint8_t destination_teleporter)
{
	uint8_t destination_x;
	uint8_t destination_y;
	uint8_t source_x = player_x;
	uint8_t source_y = player_y;

	/* Find destination coordinates, and check if they are blocked 
	 * by a pushable box:  
	 */
	boolean abort_teleport = FALSE;
	if (destination_teleporter % 2 == 0) {
		destination_x = teleporters[2 * destination_teleporter + 2];
		destination_y = teleporters[2 * destination_teleporter + 3];
	} else {
		destination_x = teleporters[2 * destination_teleporter - 2] ;
		destination_y = teleporters[2 * destination_teleporter - 1];
	}
	if (get_bkg_tile_xy(destination_x >> 3, destination_y >> 3) >= 128) {
		abort_teleport = TRUE;
	}
	destination_y -= 4;

	/* Run the teleporter animation 
	 * Sprites 0, 1 are used for the teleporter
	 */
	//SWITCH_ROM_MBC5(PALETTE_BANK);
	SWITCH_ROM_MBC5(2);
	set_sprite_data(124, 4, sprites + 224 * 16);
	set_sprite_tile(0, 124);
	set_sprite_tile(1, 126);
	set_sprite_prop(0, 4);
	set_sprite_prop(1, 4);
	move_sprite(0, screen_x, screen_y);
	move_sprite(1, screen_x + 7, screen_y);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 220 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 216 * 16);
	wait(8, 2);

	/* Hide the player metasprite and use sprites 4 and 5
	 * for a transporting animation:
	 */
	hide_sprite(6);
	hide_sprite(7);
	set_sprite_data(0, 4, sprites + 192 * 16);
	set_sprite_prop(4, 4);
	set_sprite_prop(5, 4);
	SWITCH_ROM_MBC5(2);

	set_sprite_data(124, 4, sprites + 212 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 208 * 16);
	wait(8, 2);

	hide_sprite(0);
	hide_sprite(1);

	animation_t = 0;

	/* Move camera */
	do {
		while (player_x != destination_x || player_y != destination_y) {
			++animation_t;
			if (player_x < destination_x) {
				++player_x;
			} else if (player_x > destination_x) {
				--player_x;
			}
			if (player_y < destination_y) {
				++player_y;
			} else if (player_y > destination_y) {
				--player_y;
			}
			//SWITCH_ROM_MBC5(2);
			update_camera();
			update_screen_xy();
			draw_boxes();
			draw_npcs();
			SWITCH_ROM_MBC5(4);
			draw_lasers();
			SWITCH_ROM_MBC5(2);
			/* animate the spinning energy: */
			move_sprite(4, screen_x,     screen_y);
			move_sprite(5, screen_x + 8, screen_y);
			switch (animation_t % 32) {
				case 0:
					set_sprite_data(0, 4, sprites + 192 * 16);
					break;
				case 7:
					set_sprite_data(0, 4, sprites + 196 * 16);
					break;
				case 15:
					set_sprite_data(0, 4, sprites + 200 * 16);
					break;
				case 23:
					set_sprite_data(0, 4, sprites + 204 * 16);
					break;
			}
			wait(1, 2);
		}
		if (abort_teleport) {
			/* Play an animation to show the teleport failed: */
			set_sprite_data(124, 4, sprites + 112 * 16);
			move_sprite(0, screen_x, screen_y);
			move_sprite(1, screen_x + 7, screen_y);
			wait(8, 2);
			hide_sprite(0);
			hide_sprite(1);
			wait(8, 2);
			move_sprite(0, screen_x, screen_y);
			move_sprite(1, screen_x + 7, screen_y);
			wait(8, 2);
			hide_sprite(0);
			hide_sprite(1);
			wait(8, 2);

			/* rewind */
			abort_teleport = FALSE;
			destination_x = source_x;
			destination_y = source_y;
			wait(8, 2);
			continue;
		} else break;
	} while (TRUE);

	/* Teleporter animation: */
	set_sprite_data(124, 4, sprites + 208 * 16);
	move_sprite(0, screen_x, screen_y);
	move_sprite(1, screen_x + 7, screen_y);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 212 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 216 * 16);
	wait(8, 2);

	/* Show player sprite: */
	set_sprite_prop(4, 0);
	set_sprite_prop(5, 0);
	player_state = standing;
	//SWITCH_ROM_MBC5(PALETTE_BANK);
	//set_sprite_palette(0, 1, player_pal0);
	draw_player();

	SWITCH_ROM_MBC5(2);
	set_sprite_data(124, 4, sprites + 220 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 224 * 16);
	wait(8, 2);

	/* Hide teleporter animation: */
	hide_sprite(0);
	hide_sprite(1);

	return;
}



/* draw_player()
 *
 * Update player sprite and camera based on the player state and position
 * 
 */
void draw_player(void)
{
	update_camera();
	update_screen_xy();
	move_player_sprite();
	pick_player_animation();
}



/* 
 * Choose the current animation frame for the player metasprite and load
 * the right animation data.
 */
void pick_player_animation(void)
{
	switch (player_face) {
		case west:
			tilel = 44;
			break;
		case east:
			tilel = 4;
			break;
		case north:
			tilel = 124;
			break;
		case south:
			tilel = 84;
			break;
	}
	if (player_state == standing) {
		// nothing
	} else if (player_state == running) {
		if( animation_t < 12) {
			tilel += 4;
		} else if (animation_t < 24) {
			tilel += 8;
		} else if (animation_t < 36) {
			tilel += 12;
		} else {
			tilel += 8;
		}
	} else if (player_state == pushing) {
		if( animation_t < 16) {
			tilel += 20;
		} else {
			tilel += 16;
		}
	} else if (player_state == charging) {
		tilel += 32;
	} else if (player_state == zapped) {
		if (animation_t < 30) { 
			tilel += 24; 
		} else  {
			tilel += 28;
		}
	}
	memcpy(sprite_buffer, sprites + tilel * 16, 64);
	if (player_state == pushing) {
		if (player_face == west) {
			memcpy(sprite_buffer + 64, sprites + 184 * 16, 64);
		}
		else if (player_face == east) {
			memcpy(sprite_buffer + 64, sprites + 188 * 16, 64);
		}
	} else {
		memcpy(sprite_buffer + 64, sprites + 180 * 16, 64);
	}

	/* Mask sprites*/
	player_mask_mx1 = (player_x) >> 3;
	player_mask_mx2 = (player_x + 7) >> 3;
	player_mask_mx3 = (player_x + 8) >> 3;
	player_mask_mx4 = (player_x + 15) >> 3;
	player_hit_my2 = (player_y + 15) >> 3;
	mask1 = 0xFF;
	mask2 = 0xFF;
	shift1 = 0xFF << (player_x % 8); 
	shift2 = ~shift1;

	tile = get_bkg_tile_xy(player_mask_mx1, player_hit_my2);
	if (tile >= 48 && tile < 64) {
		mask1 &= shift2;
	}
	tile = get_bkg_tile_xy(player_mask_mx2, player_hit_my2);
	if (tile >= 48 && tile < 64) {
		mask1 &= shift1;
	}
	tile = get_bkg_tile_xy(player_mask_mx3, player_hit_my2);
	if (tile >= 48 && tile < 64) {
		mask2 &= shift2;
	}
	tile = get_bkg_tile_xy(player_mask_mx4, player_hit_my2);
	if (tile >= 48 && tile < 64) {
		mask2 &= shift1;
	}

	start_j = (8 - (player_y % 8)) * 2;
	if (start_j == 16) start_j = 0;
	obj_ptr1 = sprite_buffer + 16  + start_j;
	obj_ptr3 = obj_ptr1 + 32;
	obj_ptr2 = obj_ptr3 + 32;
	obj_ptr4 = obj_ptr2 + 32;

	for (j = start_j; j < 16; ++j) {
		*obj_ptr1 &= mask1;
		*obj_ptr2 &= mask1;
		*obj_ptr3 &= mask2;
		*obj_ptr4 &= mask2;
		++obj_ptr1;
		++obj_ptr2;
		++obj_ptr3;
		++obj_ptr4;
	}

	set_sprite_data(0, 8, sprite_buffer);

	if (animation_t > 47) animation_t = 0;
	else ++animation_t;
}



/* 
 *  Move the components of the player metasprite into place at (screen_x, screen_y)
 */
void move_player_sprite()
{
	move_sprite(4, screen_x,     screen_y);
	move_sprite(5, screen_x + 8, screen_y);
	if (player_state == pushing && player_face == south) {
		move_sprite(6, screen_x,     screen_y + 1);
		move_sprite(7, screen_x + 8, screen_y + 1);
	} else if (player_state == zapped) {
		hide_sprite(6);
		hide_sprite(7);
	} else {
		move_sprite(6, screen_x,     screen_y);
		move_sprite(7, screen_x + 8, screen_y);
	}
}



/* update_camera()
 *
 * recalculate camera coordinates
 *
 *  reads globals: player_x, player_y
 *  sets globals:  camera_x, camera_y
 */
void update_camera(void)
{
	if (fixed_camera) {
		camera_x = 0;
		camera_y = 0;
	} else {
		if (player_x < 72) {
			camera_x = 0;
		} else if (player_x > 167) {
			camera_x = 95;
		} else {
			camera_x = player_x - 72;
		}
		if (player_y < 56) {
			camera_y = 0;
		} else if (player_y > 167) {
			camera_y = 111;
		} else {
			camera_y = player_y - 56;
		}
	}
}



/*
 * Calculate where the player sprite should be drawn on the screen
 * based on the player's map coordinates.
 */
void update_screen_xy(void)
{
	screen_x = player_x + 8 - camera_x;
	screen_y = player_y + 16 - camera_y;
}

