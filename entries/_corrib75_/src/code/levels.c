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

#include <gb/cgb.h>
#include <gb/gb.h>
#include <gb/hardware.h>

#include "../assets/bg_tiles.h"
#include "../assets/beam2.h"
#include "../assets/blocked.h"
#include "../assets/chasm.h"
#include "../assets/decay.h"
#include "../assets/entry.h"
#include "../assets/easy.h"
#include "../assets/easy2.h"
#include "../assets/meeting.h"
#include "../assets/nowall.h"
#include "../assets/pillars.h"
#include "../assets/reentry.h"
#include "../assets/rooms.h"
#include "../assets/sprite_tiles.h"

#include "lasers.h"
#include "levels.h"
#include "npc.h"
#include "palettes.h"
#include "player.h"

#pragma bank 0

uint8_t current_level;
uint8_t nspaces;
uint8_t filled_spaces;

boolean fixed_camera;
const uint8_t *level_ptr;
const uint8_t *level_ptr1;

uint8_t camera_x=0, camera_y=0;

uint8_t ntele=0;
uint16_t teleporters[max_teleporters * 2];

static uint8_t i,j;
static const uint8_t *p;
static uint16_t index;
uint8_t exit_mx, exit_my;

uint8_t box_x;
uint8_t box_y;
int8_t box_x_offset;
int8_t box_y_offset;
uint8_t box_map_x;
uint8_t box_map_y;
enum box_types box_type;
static boolean end_push = FALSE;
static uint8_t tile;
static uint8_t tile2;

const struct level_data levels_list[NLEVELS] = {
	{.pln0 = EntryPLN0,
		.pln1 = EntryPLN1,
		.floor_pal_rgb= floor_pal_rgb0,
		.wall_pal_rgb= wall_pal_rgb0,
		.npc_message = "BEHOLD, KAREN,_MY CREATION!___ISN'T IT AMAZING?_LET'S GO GREET_THE OTHER GUESTS.___WAIT...____SOMETHING'S NOT_RIGHT...",
		.npc_portrait = sprites + 332 * 16,
		.npc_pal_rgb = profd_pal_rgb0,
		.bank = 1,
		.fixed_camera = FALSE
	},

	{.pln0 = EasyPLN0,
		.pln1 = EasyPLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = "WAS THERE A CRASH?_I WAS IN THE LOBBY,_BUT THEN I GOT__SHUNTED HERE...____I HOPE PROFD FINDS_THE PROBLEM. THIS_TECH IS AMAZING!",
		.npc_portrait = sprites + 316 * 16,
		.bank = 1,
		.npc_pal_rgb = npc_pal_rgb1,
		.fixed_camera = FALSE
	},
	
	{.pln0 = Easy2PLN0,
		.pln1 = Easy2PLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.bank = 1,
		.fixed_camera = TRUE
	},

	{
	.pln0 = PillarsPLN0,
		.pln1 = PillarsPLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = "ANOTHER HUMAN,_PHEW!_I'M TOTALLY LOST.__DID YOU KNOW THERE_WOULD BE LASERS?_FYI, THEY HURT.__AMAZING REALISM!____BUT UH, WHAT'S_GOING ON?",
		.npc_portrait = sprites + 296 * 16,
		.bank = 3,
		.fixed_camera = FALSE
	},


	{.pln0 = RoomsPLN0,
		.pln1 = RoomsPLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = "CAN YOU LOG OUT?_I CAN'T LOG OUT.___IS THERE A BUTTON?_THIS IS KIND OF_SCARY...",
		.npc_portrait = sprites + 348 * 16,
		.npc_pal_rgb = npc_pal_rgb2,
		.bank = 3,
		.fixed_camera = FALSE
	},
	
	{.pln0 = ChasmPLN0,
		.pln1 = ChasmPLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.npc_pal_rgb = NULL,
		.bank = 1,
		.fixed_camera = FALSE
	},
	
	{.pln0 = Beam2PLN0,
		.pln1 = Beam2PLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.npc_pal_rgb = NULL,
		.bank = 1,
		.fixed_camera = FALSE
	},

	{.pln0 = BlockedPLN0,
		.pln1 = BlockedPLN1,
		.floor_pal_rgb= floor_pal_rgb0,
		.wall_pal_rgb= wall_pal_rgb0,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.bank = 3,
		.fixed_camera = FALSE
	},

	{.pln0 = DecayPLN0,
		.pln1 = DecayPLN1,
		.floor_pal_rgb= floor_pal_rgb2,
		.wall_pal_rgb= wall_pal_rgb2,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.bank = 3,
		.fixed_camera = FALSE
	},

	{.pln0 = NoWallPLN0,
		.pln1 = NoWallPLN1,
		.floor_pal_rgb= floor_pal_rgb2,
		.wall_pal_rgb= wall_pal_rgb2,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.bank = 3,
		.fixed_camera = TRUE
	},

	{.pln0 = ReentryPLN0,
		.pln1 = ReentryPLN1,
		.floor_pal_rgb= floor_pal_rgb2,
		.wall_pal_rgb= wall_pal_rgb2,
		.npc_message = NULL,
		.npc_portrait = NULL,
		.bank = 3,
		.fixed_camera = TRUE
	},
	
	{.pln0 = MeetingPLN0,
		.pln1 = MeetingPLN1,
		.floor_pal_rgb= floor_pal_rgb1,
		.wall_pal_rgb= wall_pal_rgb1,
		.npc_message = NULL,
		.npc_portrait = sprites + 312 * 16,
		.npc_pal_rgb = ai_pal_rgb,
		.bank = 3,
		.fixed_camera = FALSE
	}
};



/* Apply a mask to the box metasprite as it's moving
 * Only mask lower 8 lines of sprites. Mask 8x8 blocks.
 * sl source location, (lower) left tile 
 * sr source location, (lower) right tile 
 * dl destination location, (lower) left tile 
 * dr destination location, (lower) right tile 
 */
boolean mask_box_dl; 
boolean mask_box_dr;
boolean mask_box_sl; 
boolean mask_box_sr;

uint8_t level_bank = 3;


/* load_level(uint8_t level_number) 
 * Fades to black, loads all data for the current level, initialises
 * the player and other data, and fades in from black.
 */
void load_level(uint8_t level_number)
{
	fade_to_black();

	// SWITCH LEVELS HERE
	// ---
	if (level_number < NLEVELS) {
		level_bank = levels_list[level_number].bank;
		SWITCH_ROM_MBC5(level_bank);
		level_ptr  = levels_list[level_number].pln0;
		level_ptr1 = levels_list[level_number].pln1;
		fixed_camera = levels_list[level_number].fixed_camera;
		VBK_REG=1;
		set_bkg_tiles(0, 0, map_w, map_h, level_ptr1);
		wait(1, level_bank);
		VBK_REG=0;
		set_bkg_tiles(0, 0, map_w, map_h, level_ptr);
	}	// ----
	disable_interrupts();
	set_interrupts(VBL_IFLAG);
	enable_interrupts();
	SPRITES_8x16;
	p = level_ptr;
	wait(1, level_bank);
	// box sprites:
	box_x = 0;
	box_y = 0;
	box_map_x = 0;
	box_map_y = 0;
	box_x_offset = 0;
	box_y_offset = 0;
	set_sprite_tile(16, 136);
	set_sprite_tile(17, 138);
	set_sprite_prop(16, 7);
	set_sprite_prop(17, 7);
	/* Detect features on the map
	 * like player spawn point, teleporters, etc.
	 */
	player_win = FALSE;
	ntele = 0;
	laser_on = FALSE;
	nspaces = 0;
	wait(1, 2);
	set_sprite_data(136, 4, sprites + 248 * 16);
	wait(1, level_bank);
	filled_spaces = 0;
	npc_x = 0;
	npc_y = 0;
	npc_mx = 0;
	npc_my = 0;
	for(j = 0; j < map_h; ++j) {
		for (i = 0; i < map_w; ++i) {
			if ((*p) >= 98 && (*p) <= 103) { /* Destination spaces for boxes */
				++nspaces;
				wait(1, level_bank);
			} else if ((*p) == 115 || (*p) == 91) { /* lower right tile of exit*/
				exit_mx = i - 1;
				exit_my = j - 1;
				wait(1, level_bank);
			} else if ((*p) == 254U) { /* NPC */
				npc_x = (i << 3) + 8; /* Store NPC coords with the sprite offsets */
				npc_y = (j << 3) + 16;
				npc_mx = i;
				npc_my = j;
				tile = *(p + 1) - 200;
				set_sprite_prop(38, 17);
				set_sprite_prop(39, 17);
				SWITCH_ROM_MBC5(2);
				uncover_square(i, j, tile);
				init_npcs();
				wait(1, level_bank);
			} else if ((*p) == 255U) { /* Player */
				can_teleport = TRUE;
				player_x = i << 3 ;
				player_y = j << 3;
				player_face = south;
				player_state = standing;
				push_count = 0;
				set_sprite_prop(4, 0);
				set_sprite_prop(5, 0);
				set_sprite_prop(6, 17);
				set_sprite_prop(7, 17);
				set_sprite_tile(4, 0);
				set_sprite_tile(5, 2);
				set_sprite_tile(6, 4);
				set_sprite_tile(7, 6);
				tile = *(p + 1) - 200;
				set_vram_byte((uint8_t *)0xff49, 0x90); // DMG OBJECT PALETTE 1
				SWITCH_ROM_MBC5(2);
				uncover_square(i, j, tile);
				wait(1, level_bank);
			} else if ((*p) == 0x14){ /* Laser */
				laser_on = TRUE;
				laser_x = i * 8 + 4;
				laser_y = j * 8 + 10;
				init_lasers(); // switches to bank 2
				wait(1, level_bank);
			} else if ((*p) >= 122U && (*p) <= 127U && ntele < max_teleporters) { /*Teleporter*/
				teleporters[2 * ntele] = i * 8UL;
				teleporters[2 * ntele + 1] = j * 8UL;
				++ntele;
				wait(1, level_bank);
			} else if ((*p) == 128U) { /* Arrow Box */
				tile = *(p + 1) - 200;
				if (tile < 22) {
					set_bkg_tile_xy(i + 1, j, 130);
					if (tile < 6) {
						set_bkg_tile_xy(i,     j + 1, 129);
						set_bkg_tile_xy(i + 1, j + 1, 131);
					}
				}
				wait(1, level_bank);
			}
			++p;
		}
		wait(1, level_bank);
	}
	VBK_REG=0;
	draw_boxes();
	HIDE_BKG;
	HIDE_SPRITES;


	wait(1, 2);
	UPDATE_HITBOX;
	UPDATE_MAP_COLLISIONS;
	draw_player();

	SWITCH_ROM_MBC5(PALETTE_BANK);
	load_pal_buffer();
	dim_buffer(2);
	set_pal_from_buffer();

	if (_cpu == CGB_TYPE) {
		SHOW_BKG;
		SHOW_SPRITES;
	}
	wait(2, PALETTE_BANK);
	load_pal_buffer();
	dim_buffer(1);
	set_pal_from_buffer();

	wait(2, PALETTE_BANK);

	load_pal_buffer();
	set_pal_from_buffer();

	wait(1, 2);

	if (_cpu == DMG_TYPE) {
		SHOW_BKG;
		SHOW_SPRITES;
	}
	SWITCH_ROM_MBC5(2);
	HIDE_WIN;
	return;
}



/* draw_boxes()
 *
 * Handles the metasprite for moving boxes.
 * Draws the sprite, updates the coordinates, and puts the box back
 * on the bg tilemap when the movement is complete.
 */

uint8_t box_tiles0[4] = {128, 130, 129, 131};
uint8_t box_tiles1[4] = {7, 7, 7, 7};

uint8_t exit_tiles0[4] = {112, 114, 113, 115};
uint8_t qbox_tiles0[4] = {88, 90, 89, 91};
uint8_t exit_tiles1[4] = {6, 6, 6, 6};
void draw_boxes(void)
{
	/* Finished moving - put the box back on the bkg tile map: */
	if ( end_push ) {
		end_push = FALSE;
		box_y = 0;

		/* arrow type box */
		if (get_bkg_tile_xy(box_map_x, box_map_y + 1) >= 64) {  // bottom half uncovered
			VBK_REG = 1;
			set_bkg_tiles(box_map_x, box_map_y, 2, 2, box_tiles1);
			VBK_REG = 0;
			set_bkg_tiles(box_map_x, box_map_y, 2, 2, box_tiles0);

		} else { // bottom half covered
			VBK_REG = 1;
			set_bkg_tiles(box_map_x, box_map_y, 2, 1, box_tiles1);
			VBK_REG = 0;
			set_bkg_tiles(box_map_x, box_map_y, 2, 1, box_tiles0);
		}

		box_x_offset = 0;
		box_y_offset = 0;
		if (box_map_x != exit_mx || box_map_y != exit_my) {
			if (nspaces == filled_spaces ) { 
				// convert the q mark to an exit
				VBK_REG = 1;
				set_bkg_tiles(exit_mx, exit_my, 2, 2, exit_tiles1);
				VBK_REG = 0;
				set_bkg_tiles(exit_mx, exit_my, 2, 2, exit_tiles0);

			} else {
				// convert the exit to a q mark, if necessary
				VBK_REG = 1;
				set_bkg_tiles(exit_mx, exit_my, 2, 2, exit_tiles1);
				VBK_REG = 0;
				set_bkg_tiles(exit_mx, exit_my, 2, 2, qbox_tiles0);
			}
		}
		lasers_recalc();
	} else {
		/* Continue movement animation to destination grid square */
		if ( box_x_offset == 1 || box_x_offset == -1 || box_y_offset == 1 || box_y_offset == -1 ) {
			end_push = TRUE;
		}
		lasers_recalc_box();
		if      (box_x_offset < 0) ++box_x_offset;
		else if (box_x_offset > 0) --box_x_offset;
		if      (box_y_offset < 0) ++box_y_offset;
		else if (box_y_offset > 0) --box_y_offset;
	}
	/* Update the box sprite on the screen */
	if (box_y > 0) {
		if (box_type == arrow){
			memcpy(sprite_buffer, sprites + 248 * 16, 64);
		} else {
			memcpy(sprite_buffer, sprites + 252 * 16, 64);
		}
		//if (_cpu == DMG_TYPE) {
		{
			mask1 = 0xFF;
			mask2 = 0xFF;
			start_j = 0;
			if (!(mask_box_sl || mask_box_dl)) {
				// completely unobscured, do nothing
			} else if (mask_box_sl && mask_box_dl) {
				// east west movement, fully masked throughout
				mask1 = 0;
				mask2 = 0;
			} else if (mask_box_sl) {
				// east-west movement, obscured start position, unobscured destination
				if (box_x_offset == 16) {
					// moving west, fully obscured
					mask1 = 0;
					mask2 = 0;
				} else if (box_x_offset > 7) {
					// moving west, fully obscured right, partially obscured left
					mask1 = 0xFF << (box_x_offset - 7);
					mask2 = 0;
				} else if (box_x_offset > 0) {
					// moving west, partially obscured right, unobscured left
					mask2 = 0xFF << (box_x_offset + 1);
				} else if (box_x_offset == -16) {
					//moving east, fully obscured
					mask1 = 0;
					mask2 = 0;
				} else if (box_x_offset < -7) {
					//moving east, left fully obscured, right partially obscured
					mask1 = 0;
					mask2 = 0xFF >> ( -box_x_offset - 7);
				} else if (box_x_offset < 0) {
					//moving east, left partially obscured, right unobscured 
					mask1 = 0xFF >>  -box_x_offset + 1;
				}
			} else if (mask_box_dl) {
				if (box_y_offset > -7 && box_y_offset < 0) {
					//moving south
					mask1 = 0;
					mask2 = 0;
					start_j = - ((box_y_offset - 1) * 2);
					if (start_j == 16) start_j = 0;
				} else if (box_y_offset <= -7) {
					//moving south
					mask1 = 0xff;
					mask2 = 0xff;
					start_j = 15;
				} else if (box_x_offset == -16) {
					// moving east, unobscured
					mask1 = 0;
					mask2 = 0;
				} else if (box_x_offset <= -8) {
					//moving east, left unobscured, right partially obscured
					mask2 = 0xFF << (15 + box_x_offset); 
				} else if (box_x_offset < 0) {
					//moving east, left partially obscured, right fully obscured
					mask1 = 0xFF << (7 + box_x_offset);
					mask2 = 0;
				} else if (box_x_offset == 16) {
					// moving west, unobscured
				} else if (box_x_offset >= 8) {
					//moving west, left partially obscured, right unobscured
					mask1 = 0xFF >> (15 - box_x_offset);
				} else if (box_x_offset > 0) {
					//moving west, left fully obscured, right partially unobscured
					mask1 = 0;
					mask2 = 0xFF >> 7 - box_x_offset;
				} else if (box_x_offset == 0) {
					mask1 = 0;
					mask2 = 0;
				}
			}
			obj_ptr1 = sprite_buffer + (16)  + start_j;
			obj_ptr2 = sprite_buffer + (48)  + start_j;

			for (j = start_j; j < 16; ++j) {
				*(obj_ptr1++) &= mask1;
				*(obj_ptr2++) &= mask2;
			}
		}
		set_sprite_data(136, 4, sprite_buffer);
		move_sprite(16, box_x + box_x_offset + 8   - camera_x, box_y + box_y_offset + 16 - camera_y);
		move_sprite(17, box_x + box_x_offset + 16  - camera_x, box_y + box_y_offset + 16 - camera_y);
	} else {
		hide_sprite(16);
		hide_sprite(17);
	}
}



/* reset_level()
 * reset the current level.
 */
void reset_level(void)
{
	load_level(current_level);
}

void fade_to_black(void)
{
	if (_cpu == CGB_TYPE) {
		SWITCH_ROM_MBC5(PALETTE_BANK);
		// DIM
		dim_buffer(1);
		set_pal_from_buffer();
		HIDE_WIN;
	} else {
		HIDE_BKG;
		for (j = 0; j < 40; ++j)hide_sprite(j);
		HIDE_WIN;
	}

	// DARK
	wait(2, PALETTE_BANK);
	set_pal_from_buffer();
	dim_buffer(1);

	// BLACK
	wait(2, PALETTE_BANK);
	set_bkg_palette(0, 8, black_pal);
	set_sprite_palette(0, 8, black_pal);

	wait(2, 2);
	if (_cpu == CGB_TYPE) {
		HIDE_BKG;
		HIDE_SPRITES;
	}

	for (i=0; i<40; ++i) {
		hide_sprite(i);
	}
}

