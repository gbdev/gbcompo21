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

#include <gb/cgb.h>
#include <gb/gb.h>

#include "misc.h"
#include "../assets/ai_blank.h"
#include "../assets/thanks.h"
#include "levels.h"
#include "player.h"

static uint8_t drawing_this_column = 20;
static uint8_t drawing_this_row = 18;
static uint8_t reading_this_column = 0;
static uint8_t reading_this_row = 0;
static uint8_t j;
static size_t rrow14;
static size_t rrow21;
static size_t drow32;	
static size_t rcol14;	
static size_t rcol28;	
static uint8_t* base;
static const int8_t cosines[256] = {
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -6, -6, -6, -6, -6, -6, -6, -6, -6, -5, -5, -5, -5, -5, -5, -5, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
	
const uint16_t logo_pal[] = {RGB_BLACK, RGB(31, 31, 29), RGB(12, 12, 14), RGB(1, 1, 5)};

#pragma bank 7

void run_postscript()
{
	uint8_t i, j;
	if (_cpu == CGB_TYPE) {
		VBK_REG = 1;
		set_bkg_tiles(20, 0, 12, 18, ai_blankPLN1);
		set_bkg_tiles(0, 18, 20, 14, ai_blankPLN1);
		set_bkg_tiles(20, 18, 12, 14, ai_blankPLN1);
	}
	VBK_REG = 0;
	set_bkg_tiles(20, 0, 2, 18, ai_blankPLN1);
	set_bkg_tiles(0, 18, 20, 2, ai_blankPLN1);
	set_bkg_tiles(20, 18, 2, 2, ai_blankPLN1);

	set_vram_byte((uint8_t *)0xff49, 0xd0); // DMG OBJECT PALETTE 1
	load_logo7();
	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);
	set_sprite_tile(2, 4);
	set_sprite_tile(3, 6);
	set_sprite_tile(4, 6);
	set_sprite_tile(5, 8);
	set_sprite_tile(6, 10);
	set_sprite_tile(7, 12);
	set_sprite_tile(8, 14);
	set_sprite_tile(9, 16);

	set_sprite_data(18, 22, Thanks);
	for (i = 10, j = 18; i < 20; ++i, j+=2) {
		set_sprite_tile(i, j);
	}

	for (j = 0; j < 20; ++j) {
		set_sprite_prop(j, 1 << 4);
		hide_sprite(j);
	}
	SHOW_SPRITES;
	set_sprite_palette(0, 1, logo_pal);
	wait(1, 7);

	animation_t = 0;
	boolean show_text = FALSE;
	uint8_t counter = 0;
	while(TRUE)  {
		if (counter < 90) {
			++counter;
		} else {
			show_text = TRUE;
			for (i = 10; i < 20; ++i) move_sprite(i, 8 * i - 60, 40);
		}
		++camera_x;
		++camera_y;
		/* Every 8 pixels of scroll, we add a new row of tiles to the right,
		   and a new column of tiles at the bottom. This work is split up
		   into 8 segments, happening in 8 different frames, to keep CPU low.
		 */
		switch (animation_t % 8) {
			case 0:
				reading_this_row++;
				drawing_this_row++;
				if (reading_this_row == 21) reading_this_row = 0;
				if (drawing_this_row == 32) drawing_this_row = 0;
				reading_this_column+=1;
				drawing_this_column+=1;
				if (reading_this_column == 14) reading_this_column = 0;
				if (drawing_this_column == 32) drawing_this_column = 0;
				set_bkg_tile_xy(drawing_this_column, drawing_this_row, ai_blank[(14 * reading_this_row) + reading_this_column % 14]);
				drow32 = drawing_this_row + 32;
				rcol14 = reading_this_column % 14;
				rrow21 = reading_this_row + 21;
				base = (uint8_t *) 0x9800 + drawing_this_column;
				break;

			case 1:
				for (j = 1 ; j < 8; ++j) {	
					set_vram_byte(base + (((drow32 - j) % 32) << 5), ai_blank[14 * ((rrow21 - j) % 21) + rcol14]);
				}
				break;
			case 2:
				for (j = 8 ; j < 14; ++j) {	
					set_vram_byte(base + (((drow32 - j) % 32) << 5), ai_blank[14 * ((rrow21 - j) % 21) + rcol14]);
				}
				break;
			case 3:
				for (j = 14 ; j < 20; ++j) {	
					set_vram_byte(base + (((drow32 - j) % 32) << 5), ai_blank[14 * ((rrow21 - j) % 21) + rcol14]);
				}
				base = (uint8_t *) 0x9800 + (drawing_this_row << 5);
				rrow14 = reading_this_row * 14;
				rcol28 = reading_this_column + 28;
				break;
			case 4:
				for (j = 1 ; j < 6; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), ai_blank[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			case 5:
				for (j = 6 ; j < 11; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), ai_blank[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			case 6:
				for (j = 11 ; j < 16; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), ai_blank[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			case 7:
				for (j = 16 ; j < 20; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), ai_blank[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			default:
				break;

		}
		animation_t++;
		wait(1, 7);
		if (show_text) {
			for (j = 0; j < 10; ++j) {
				uint8_t index = 4 * animation_t + 16 * j;
				move_sprite(j, 50 + j * 8, 80 + cosines[index] );
			}
		}
		wait(1, 7);
	}
}


