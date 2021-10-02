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
#include <gb/bgb_emu.h>

#include "../assets/sprite_tiles.h"

#include "levels.h"
#include "lasers.h"
#include "palettes.h"
#include "misc.h"
#include "player.h"
#include "messagebox.h"
#include "npc.h"

#pragma bank 2
#define PLAYER2BANK 2

boolean movnl(uint8_t n);
boolean movnr(uint8_t n);
boolean movnu(uint8_t n);
boolean movnd(uint8_t n);
void fall(void); 
boolean zap(void);

static uint8_t map_x, map_y, tile;



/* zap()
 *
 * Returns TRUE if the player was zapped by a laser, otherwise FALSE. 
 *
 * If the player is zapped, runs the zapping animation and reloads the current level. 
 */
boolean zap(void)
{
	if (laser_on &&
			player_x > laser_x - 8 &&
			player_x < laser_x     &&
			player_y > laser_y - 8 &&
			player_y < laser_end_y) {

		hide_sprite(1);
		hide_sprite(2);
		hide_sprite(3);
		set_sprite_data(126, 2, sprites + 256 * 16);
		set_sprite_palette(7, 1, exclam_pal);
		set_sprite_tile(0, 126);
		set_sprite_prop(0, 7);
		move_sprite(0, screen_x + 14, screen_y - 10);
		animation_t = 0;
		player_state = zapped;
		draw_player();
		NR41_REG = 0x3f;
		NR42_REG = 0xc3;
		NR43_REG = 0x2f;
		NR44_REG = 0x80;
		wait(6, PLAYER2BANK);
		NR41_REG = 0x3a;
		NR42_REG = 0xb3;
		NR43_REG = 0x2c;
		NR44_REG = 0x80;
		wait(42, PLAYER2BANK);
		load_level(current_level);
		return TRUE;
	}
	return FALSE;
}



/* charge()
 *
 * Shoot player forward until the hit an obstacle
 * Fly over pits, but collide with walls & boxes
 */
void charge(void)
{
	uint8_t j;

//set_sprite_tile(0, 124);
//set_sprite_tile(1, 126);
//set_sprite_tile(2, 120);
//set_sprite_tile(3, 122);

	/* Move player in 4 pixel increments:
	 */
	player_state = charging;
	NR41_REG = 0x05;
	NR42_REG = 0x29;
	NR43_REG = 0x41;
	NR44_REG = 0xc0;	
	switch (player_face) {
		case north:
		//set_sprite_data(124, 4, sprites + 228 * 16);
		//set_sprite_data(120, 4, sprites + 268 * 16);
			while (movnu(4)) {
				draw_player();
			//move_sprite(0, screen_x, screen_y + 8);
			//move_sprite(1, screen_x + 5, screen_y + 14);
			//move_sprite(2, screen_x, screen_y - 4);
			//move_sprite(3, screen_x + 8, screen_y - 4);
				draw_npcs();
				draw_boxes();
				if(zap()) return;
				draw_lasers();
				wait(1, PLAYER2BANK);
				btn = joypad();
				if (btn & J_SELECT) {
					reset_level();
					return;
				}
			}
			break;
		case south:
		//set_sprite_data(124, 4, sprites + 228 * 16);
		//set_sprite_data(120, 4, sprites + 272 * 16);
			while (movnd(4)) {
				draw_player();
			//move_sprite(0, screen_x, screen_y - 16);
			//move_sprite(1, screen_x + 5, screen_y - 24);
			//move_sprite(2, screen_x, screen_y + 4);
			//move_sprite(3, screen_x + 8, screen_y + 4);
				draw_npcs();
				draw_boxes();
				if(zap()) return;
				draw_lasers();
				wait(1, PLAYER2BANK);
				btn = joypad();
				if (btn & J_SELECT) {
					reset_level();
					return;
				}
			}
			break;
		case east:
		//set_sprite_data(124, 4, sprites + 232 * 16);
		//set_sprite_data(120, 4, sprites + 266 * 16);
			while (movnr(4)) {
				draw_player();
			//move_sprite(0, screen_x - 16, screen_y);
			//move_sprite(1, screen_x -  8, screen_y);
			//move_sprite(2, screen_x + 12, screen_y);
			//hide_sprite(3);
				draw_npcs();
				draw_boxes();
				if(zap()) return;
				draw_lasers();
				wait(1, PLAYER2BANK);
				btn = joypad();
				if (btn & J_SELECT) {
					reset_level();
					return;
				}
			}
			break;
		case west:
		//set_sprite_data(124, 4, sprites + 232 * 16);
		//set_sprite_data(120, 4, sprites + 264 * 16);
			while (movnl(4)) {
				draw_player();
			//move_sprite(0, screen_x + 16, screen_y);
			//move_sprite(1, screen_x + 24, screen_y);
			//move_sprite(2, screen_x - 4,  screen_y);
			//hide_sprite(3);
				draw_npcs();
				draw_boxes();
				if(zap()) return;
				draw_lasers();
				wait(1, PLAYER2BANK);
				btn = joypad();
				if (btn & J_SELECT) {
					reset_level();
					return;
				}
			}
			break;
	}
	draw_player();
	draw_npcs();
	wait(1, PLAYER2BANK);

	/* Move player the last 0-3 pixels until touching an object:
	 */
	for (j = 3; j > 0; --j) {
		switch (player_face) {
			case north:
				movnu(j);
				break;
			case south:
				movnd(j);
				break;
			case east:
				movnr(j);
				break;
			case west:
				movnl(j);
				break;
		}
	}
	hide_sprite(0);
	hide_sprite(1);
	hide_sprite(2);
	hide_sprite(3);
	draw_player();
	draw_npcs();
	if(zap()) return;
	wait(1, PLAYER2BANK);


	int8_t adjust_x = 0;
	int8_t adjust_y = 0;
	boolean ul, ur, ll, lr;
	boolean falling = FALSE;
	/* The 16x16 player sprite after colliding with a wall:
	   ........,,,,,,,,
	   ........,,,,,,,,
	   ........,,,,,,,,
	   ........,,,,,,,,
	   ........,,,,,,,,
	   ........,,,,,,,,
	   ........,,,,,,,,
	   ..W....AB,,,,X,,
	   ,,,,,,,CD.......
	   ,,,,,,,,........
	   ,,,,,,,,........
	   ,,,,,,,,........
	   ,,,,,,,,........
	   ,,,,,,,,........
	   ,,,,,,,,........
	   ,,Y,,,,,.....Z..

	   1) Check that pixels ABCD are all on solid ground,
	   otherwise fall
	   2) Check that pixels WXYZ are all on solid ground,
	   otherwise shunt the player to solid ground
	 */
	map_x = (player_x + 7) >> 3;
	map_y = (player_y + 7) >> 3; 
	tile = get_bkg_tile_xy(map_x, map_y);
	ul = (tile >= blocking_tiles);

	map_x = (player_x + 8) >> 3;
	tile  = get_bkg_tile_xy(map_x, map_y);
	ur = (tile >= blocking_tiles);

	map_y = (player_y + 8) >> 3; 
	tile  = get_bkg_tile_xy(map_x, map_y);
	lr = (tile >= blocking_tiles);

	map_x = (player_x + 7) >> 3;
	ll = (tile >= blocking_tiles);

	if (ul && ur && ll && lr) {
		//
		map_x = (player_x + 2) >> 3;
		map_y = (player_y + 7) >> 3; 
		tile = get_bkg_tile_xy(map_x, map_y);
		ul = (tile >= wall_tiles && tile < blocking_tiles);

		map_x = (player_x + 13) >> 3;
		tile  = get_bkg_tile_xy(map_x, map_y);
		ur = (tile >= wall_tiles && tile < blocking_tiles);

		map_y = (player_y + 15) >> 3; 
		tile  = get_bkg_tile_xy(map_x, map_y);
		lr = (tile >= wall_tiles && tile < blocking_tiles);

		map_x = (player_x + 2) >> 3;
		ll = (tile >= wall_tiles && tile < blocking_tiles);

		switch (player_face) {
			case north:
			case south:
				if (ul || ll) adjust_x = 5;
				else if (ur || lr) adjust_x = -5;
				break;
			case east:
			case west:
				if (ul || ur) adjust_y = 5;
				else if (ll || lr) adjust_y = -5;
				break;
		}

	} else {
		falling = TRUE;
	}
	NR41_REG = 0x06;
	NR42_REG = 0xf3;
	NR43_REG = 0x57;
	NR44_REG = 0x80;	

	/* Screen shake animation */
	for (j = 24; j > 0; --j) {
		if (j % 2 == 0) {
			if (adjust_x > 0) {
				player_x -= 1;
				--adjust_x;
			} else if (adjust_x < 0) {
				player_x += 1;
				++adjust_x;
			}
			if (adjust_y > 0) {
				screen_y -= 1;
				--adjust_y;
			} else if (adjust_y < 0) {
				screen_y += 1;
				++adjust_y;
			}
		}
		update_screen_xy();
		switch( j % 4) {
			case 0:
				--camera_x;
				--screen_x;
				break;
			case 2:
				++camera_x;
				++screen_x;
				break;
		}
		pick_player_animation();
		move_player_sprite();
		draw_boxes();
		draw_lasers();
		wait(1, PLAYER2BANK);
	}
	if (falling) {
		fall();
	} else {
		player_state = standing;
	}
	return;
}



/* fall() 
 * 
 * Call when the player should fall into a hole.
 * Runs the falling animation and reloads the current level.
 */
void fall(void)
{
	/* Load exlamation mark */
	set_sprite_data(126, 2, sprites + 256 * 16);
	set_sprite_palette(7, 1, exclam_pal);
	set_sprite_tile(0, 126);
	set_sprite_prop(0, 7);
	move_sprite(0, screen_x + 14, screen_y - 10);
	wait(12, PLAYER2BANK);

	/* Animate player falling */
	set_sprite_data(0, 4, sprites + 164 * 16);
	wait(6, PLAYER2BANK);

	set_sprite_data(0, 4, sprites + 168 * 16);
	wait(6, PLAYER2BANK);

	hide_sprite(6);
	hide_sprite(7);
	set_sprite_data(0, 4, sprites + 172 * 16);
	wait(6, PLAYER2BANK);

	hide_sprite(0);
	hide_sprite(4);
	hide_sprite(5);
	wait(24, PLAYER2BANK);

	load_level(current_level);

	return;
}



/* movnu(n), movnd(n), movnl(n), movnr(n): 
 * Move the player n pixels up, down, left, or right.
 *
 * Updates the coordinates, checks for collisions,
 * then resets the coordinations if the move is invalid.
 *
 * The player either moves the full n pixels, or not at all.
 * 
 * Collision checks may have side effects, like pushing boxes.
 *
 * ARGUMENTS
 *  n: pixels to move
 *
 * Return TRUE on successful move, or FALSE otherwise.
 *
 */
boolean movnu(uint8_t n)
{
	player_y -= n;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		player_y += n;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
		return FALSE;
	}
	return TRUE;
}



boolean movnd(uint8_t n)
{
	player_y += n;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		player_y -= n;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
		return FALSE;
	}
	return TRUE;
}



boolean movnl(uint8_t n)
{
	player_x -= n;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		player_x += n;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
		return FALSE;
	}
	return TRUE;
}



boolean movnr(uint8_t n)
{
	player_x += n;
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	if (player_collide_map() || player_collide_box() || player_collide_npc()) {
		player_x -= n;
		UPDATE_HITBOX;
		UPDATE_MAP_COLLISIONS;
		return FALSE;
	}
	return TRUE;
}


/* advance_level()
 * play a teleport out animation 
 */
void advance_level(void)
{
	/* Run the teleporter animation 
	 * Sprites 0, 1 are used for the teleporter
	 */
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
	hide_sprite(4);
	hide_sprite(5);
	hide_sprite(6);
	hide_sprite(7);

	set_sprite_data(124, 4, sprites + 212 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 208 * 16);
	wait(8, 2);

	hide_sprite(0);
	hide_sprite(1);
}

void enter_level(void)
{
	draw_npcs();
	draw_lasers();
	hide_sprite(4);
	hide_sprite(5);
	hide_sprite(6);
	hide_sprite(7);
	set_sprite_tile(0, 124);
	set_sprite_tile(1, 126);
	set_sprite_prop(0, 4);
	set_sprite_prop(1, 4);
	move_sprite(0, screen_x, screen_y);
	move_sprite(1, screen_x + 7, screen_y);
	
	set_sprite_data(124, 4, sprites + 208 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 212 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 216 * 16);
	wait(8, 2);

	/* Show player sprite: */
	player_state = standing;
	draw_player();

	set_sprite_data(124, 4, sprites + 220 * 16);
	wait(8, 2);

	set_sprite_data(124, 4, sprites + 224 * 16);
	wait(8, 2);

	/* Hide teleporter animation: */
	hide_sprite(0);
	hide_sprite(1);

	switch(current_level) {
		case 1:
			messagebox("// USER: CORRIB75._PLEASE FIND ME._//END MESSAGE");
			break;
		case 2: messagebox("MUCH IS CORRUPTED_BUT YOU MUST FIND _A PATH.__DO NOT DELAY.");
			break;
		default: break;
	}
}

