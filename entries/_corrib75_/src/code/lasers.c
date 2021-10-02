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

#include <stdio.h>

#include <gb/gb.h>
#include <gb/cgb.h>

#include "../assets/sprite_tiles.h"

#include "levels.h"
#include "player.h"

#pragma bank 0

uint8_t laser_x;
uint8_t laser_y;
uint8_t end_tile = 72;
static uint8_t j, i;
uint8_t laser_end_y;
boolean laser_on = FALSE;

static uint8_t sprite_counter;
static uint8_t frame_counter = 64;
static uint8_t sprite_update = 33;
static enum end_types {long_end, short_end} end_type;

uint8_t laser_n;
uint8_t laser_screen_x;
uint8_t laser_screen_y0;
uint8_t laser_screen_y1;

void lasers_recalc(void);
void lasers_recalc_box(void);

uint16_t laser_pal[4] = {0x03de, 0x01fe, 0x00fa, 0x000c};



/* init_lasers()
 * Call on level start to initialise laser data & sprites.
 */
void init_lasers(void)
{
	if (laser_on) {
		SWITCH_ROM_MBC5(2);
		set_sprite_data(64, 12, sprites + 284 * 16);
		for(j = 24; j < 34; ++j) {
			set_sprite_tile(j, 64 + j % 4);
			if (_cpu == DMG_TYPE) {
				set_sprite_prop(j, 24);
			} else {
				set_sprite_prop(j, 6);
			}
		}
		lasers_recalc();
	} else {
		for(j = 24; j < 34; ++j) {
			hide_sprite(j);
		}
		lasers_recalc();
	}
	return;
}



/* lasers_recalc()
 * Updates the end point of the laser beam.
 * Call after moving items on the map.
 */
void lasers_recalc(void)
{
	if (laser_on) {
		j = (laser_y + 10) >> 3;
		i = (laser_x - 4) >> 3;	
		sprite_counter = get_bkg_tile_xy(i, j);
		while(sprite_counter >= wall_tiles && sprite_counter < 128  && j < 32) {
			j += 1;
			sprite_counter = get_bkg_tile_xy(i, j);
		}
		laser_end_y = j << 3;
		if (sprite_counter < 128) {
			laser_end_y -= 8;
			end_type = long_end;
		} else {
			end_type = short_end;
			end_tile = 74;
		}
		return;
	}
}



/* lasers_recalc_box()
 * Recalculates the end point of the laser, taking into account moving
 * boxes, which are temporatily removed from the map.
 */
void lasers_recalc_box(void)
{
	if (laser_on &&
	 	box_y_offset != 0 &&
		laser_x == box_x + 4 &&
		laser_end_y == box_y + 16
	   ) {
		laser_end_y = box_y;
	}
}



/* draw_lasers()
 * update the sprites for the laser beam.
 * Moves the sprites to the correct location wrt the camera,
 * and picks animation frames for each section of the beam,
 * and updates the palette on CGB.
 */
void draw_lasers(void) 
{
	if (_cpu == CGB_TYPE) {
		set_sprite_palette(6, 1, laser_pal);
	}
	if (laser_on) {
		set_sprite_tile(sprite_update, frame_counter);
		frame_counter += 1; 
		if (frame_counter == 72) frame_counter = 64;
		--sprite_update;
		if (sprite_update == 23) {
			sprite_update = 33;
			if (end_tile == 74) {
				end_tile = 72;
			} else {
				end_tile = 74;
			}
		}

		laser_screen_x = laser_x + 8 - camera_x;
		laser_screen_y0 = laser_y > camera_y ? laser_y - camera_y + 16: 16;
		laser_screen_y1 = laser_end_y > camera_y ? laser_end_y - camera_y: 0; 
		if (laser_screen_y1 > 144) {
			laser_screen_y1 = 144;
		}

		sprite_counter = 27;
		if (laser_screen_y0 > laser_screen_y1) {
			laser_screen_y0 = 0;
		} else {
			j = laser_screen_y0 + 16;
			while(j <= laser_screen_y1) {
				move_sprite(sprite_counter, laser_screen_x, j);
				++sprite_counter;
				j += 16;
			}
		}
		if (sprite_counter > 27) {
			move_sprite(sprite_counter, laser_x + 8 - camera_x, laser_screen_y1 - 4);
			++sprite_counter;
		}
		while (sprite_counter < 34) {
			hide_sprite(sprite_counter);
			++sprite_counter;
		}
		if (end_type == short_end) {
			set_sprite_tile(24, end_tile);
		}

		move_sprite(25, laser_x + 8 - camera_x, laser_screen_y0);
		move_sprite(24, laser_x + 8 - camera_x, laser_screen_y1);

		if(laser_screen_y1 > laser_y - camera_y + 16) move_sprite(26, laser_x + 8 - camera_x, laser_screen_y1);
		else hide_sprite(26);

		return;
	}
}

