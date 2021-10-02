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

#include "../assets/blank.h"
#include "../assets/hextile.h"
#include "../assets/logo_tiles.h"
#include "../assets/plane.h"
#include "../assets/start_tiles.h"

#include "misc.h"
#include "title.h"
#include "pretitle.h"
#include "player.h"
#include "levels.h"
#include "messagebox.h"

#pragma bank 4
#define TITLEBANK 4

static uint8_t drawing_this_column = 21;
static uint8_t drawing_this_row = 20;
static uint8_t reading_this_column = 0;
static uint8_t reading_this_row = 20;
static uint8_t j;
static size_t rrow14;
static size_t rrow21;
static size_t drow32;	
static size_t rcol14;	
static size_t rcol28;	
static uint8_t* base;
static const int8_t cosines[256] = {
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -6, -6, -6, -6, -6, -6, -6, -6, -6, -5, -5, -5, -5, -5, -5, -5, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
	static const uint16_t logo_pal[] = {RGB_BLACK, RGB(31, 31, 29), RGB(12, 12, 14), RGB(1, 1, 5)};



void load_title(void)
{
	//SHOW_WIN;
	//WY_REG = 0;
	HIDE_WIN;
	VBK_REG = 1;
		for( uint8_t char_count = 0; char_count < 20; ++char_count) {
			set_win_tile_xy(char_count, 0, 0);
		}
	VBK_REG = 0;
	SPRITES_8x16;
	set_vram_byte((uint8_t *)0xff49, 0xd0); // DMG OBJECT PALETTE 1
	set_bkg_data(0, 15, HexTiles);
	set_bkg_data(15, 8, StartTiles);
	set_sprite_data(0, 18, LogoTiles);

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

	for (j = 0; j < 10; ++j) {
		set_sprite_prop(j, 1 << 4);
		hide_sprite(j);
	}
	VBK_REG = 1;
	set_bkg_tiles(0,   0,  20, 18, BlankBlack);
	VBK_REG = 0;
	set_bkg_tiles(0,   0,  14, 21, Plane);
	set_bkg_tiles(14,  0,  14, 21, Plane);
	animation_t = 0;
	camera_x = 0;
	camera_y = 0;
	wait(1, TITLEBANK);
	disable_interrupts();
	CRITICAL {
		WX_REG = 200;
		WY_REG = 116;
		window_height = 8;
		LYC_REG = 100;	
		set_interrupts(VBL_IFLAG | LCD_IFLAG);
	}
	enable_interrupts();
	for (j = 1; j < 20; j++) {
		set_win_tile_xy(j - 1, 0, 15 + j);
	}
	set_sprite_palette(0, 1, logo_pal);
	set_bkg_palette_entry(0, 0, 0x77ff);
	set_bkg_palette_entry(0, 2, 0);
	set_bkg_palette_entry(0, 3, 0);
	SHOW_SPRITES;
	SHOW_BKG;
	wait(1, TITLEBANK);
}



/* draw_title()
 *
 * Draws the title screen with bouncing logo and infinite tiled background
 *
 * Call load_title first.
 * Then call this function once. Returns when the title screen is over.
 */

void draw_title(void)
{
	/* smoothly change background colours: */
	uint8_t bg_index = 1;
	uint8_t bg_red_list[6]   = {0x01, 0x14, 0x13, 0x1c, 0x00, 0x00};
	uint8_t bg_green_list[6] = {0x01, 0x08, 0x02, 0x1c, 0x10, 0x07};
	uint8_t bg_blue_list[6]  = {0x12, 0x00, 0x00, 0x08, 0x10, 0x09};
	uint8_t bg_red = 0;
	uint8_t bg_green = 0;
	uint8_t bg_blue = 0;
	uint8_t bg_target_red   = bg_red_list[1];		
	uint8_t bg_target_green = bg_green_list[1];		
	uint8_t bg_target_blue  = bg_blue_list[1];		
	/* smoothly change foreground colours: */
	uint8_t fg_index = 1;
	uint8_t fg_red_list[6] =   {0,    0x1c,    0x08,    0x18, 0x01, 0x0c};
	uint8_t fg_green_list[6] = {0x10,    0x1c, 0x00, 0x06, 0x01, 0x04};
	uint8_t fg_blue_list[6] =  {0x10, 0x00, 0,    0,    0x09, 0x0};
	uint8_t fg_red =   0;
	uint8_t fg_green = 0;
	uint8_t fg_blue =  0;
	uint8_t fg_target_red = fg_red_list[1];		
	uint8_t fg_target_green = fg_green_list[1];		
	uint8_t fg_target_blue = fg_blue_list[1];		

	uint8_t end_timer = 60; /* Countdown after pressing start button */
	boolean show_text = FALSE;

	wait(1, TITLEBANK);
	btn = joypad();
	while (end_timer > 0) {
		/* Scroll bg: */
		++camera_x;
		++camera_y;
		if (animation_t == 30) show_text = TRUE;
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
				set_bkg_tile_xy(drawing_this_column, drawing_this_row, Plane[(14 * reading_this_row) + reading_this_column % 14]);
				drow32 = drawing_this_row + 32;
				rcol14 = reading_this_column % 14;
				rrow21 = reading_this_row + 21;
				base = (uint8_t *) 0x9800 + drawing_this_column;
				break;

			case 1:
				for (j = 1 ; j < 8; ++j) {	
					set_vram_byte(base + (((drow32 - j) % 32) << 5), Plane[14 * ((rrow21 - j) % 21) + rcol14]);
				}
				break;
			case 2:
				for (j = 8 ; j < 14; ++j) {	
					set_vram_byte(base + (((drow32 - j) % 32) << 5), Plane[14 * ((rrow21 - j) % 21) + rcol14]);
				}
				break;
			case 3:
				for (j = 14 ; j < 20; ++j) {	
					set_vram_byte(base + (((drow32 - j) % 32) << 5), Plane[14 * ((rrow21 - j) % 21) + rcol14]);
				}
				base = (uint8_t *) 0x9800 + (drawing_this_row << 5);
				rrow14 = reading_this_row * 14;
				rcol28 = reading_this_column + 28;
				break;
			case 4:
				for (j = 1 ; j < 6; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), Plane[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			case 5:
				for (j = 6 ; j < 11; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), Plane[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			case 6:
				for (j = 11 ; j < 16; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), Plane[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			case 7:
				for (j = 16 ; j < 20; ++j) {	
					set_vram_byte( base + ((drawing_this_column + 32 - j) % 32), Plane[rrow14 + ((rcol28 - j) % 14)]);
				}
				break;
			default:
				break;

		}
		wait(1, TITLEBANK);
		/* Update palette entry with current colours: */
		set_bkg_palette_entry(0, 3, RGB(fg_red, fg_green, fg_blue));
		set_bkg_palette_entry(0, 2, RGB(bg_red, bg_green, bg_blue));
		/* Move colours towards target colours: */
		if (animation_t % 3 == 0) {
			if (fg_red < fg_target_red) ++fg_red;
			else if (fg_red > fg_target_red) --fg_red;
			if (fg_green < fg_target_green) ++fg_green;
			else if (fg_green > fg_target_green) --fg_green;
			if (fg_blue < fg_target_blue) ++fg_blue;
			else if (fg_blue > fg_target_blue) --fg_blue;
			if (bg_red < bg_target_red) ++bg_red;
			else if (bg_red > bg_target_red) --bg_red;
			if (bg_green < bg_target_green) ++bg_green;
			else if (bg_green > bg_target_green) --bg_green;
			if (bg_blue < bg_target_blue) ++bg_blue;
			else if (bg_blue > bg_target_blue) --bg_blue;
		}
		/* Hit the target colour, the update target: */
		if ( animation_t == 255 && end_timer >= 60 && fg_target_blue == fg_blue && fg_target_green==fg_green && fg_target_red == fg_red) { 
			++fg_index;
			if (fg_index == 6) fg_index = 0;
			fg_target_red = fg_red_list[fg_index];
			fg_target_blue = fg_blue_list[fg_index];
			fg_target_green = fg_green_list[fg_index];
		}
		if (animation_t == 128 && end_timer >= 60 && bg_target_blue == bg_blue && bg_target_green==bg_green && bg_target_red == bg_red) { 
			++bg_index;
			if (bg_index == 6) bg_index = 0;
			bg_target_red = bg_red_list[bg_index];
			bg_target_blue = bg_blue_list[bg_index];
			bg_target_green = bg_green_list[bg_index];
		}
		/* Move window onto screen from the right ("push start")*/
		if (animation_t > 128 && WX_REG > 116) {
			WX_REG-=4;
		}
		wait(1, TITLEBANK);
		/* Wavy logo: */
		if (show_text) {
			for (j = 0; j < 10; ++j) {
				uint8_t index = 4 * animation_t + 16 * j;
				move_sprite(j, 50 + j * 8, 80 + cosines[index] );
			}
		}
		/* Fade title screen out when we press start: */
		if (end_timer < 60 && end_timer > 0) { 
			--end_timer;
			if (_cpu == DMG_TYPE) {
				for (int i = 0; i < 40; ++i) hide_sprite(i);
				HIDE_SPRITES;
				HIDE_WIN;
				HIDE_BKG;
				break;
			}
		} else if (end_timer == 0) { 
			for (int i = 0; i < 40; ++i) hide_sprite(i);
		}

		if (end_timer == 20) {
			HIDE_SPRITES;
		}
		btn = joypad();
		if ((btn & J_START) && end_timer == 60) {
			--end_timer;
			bg_target_red   = RGB_BLACK;
			bg_target_blue  = RGB_BLACK;
			bg_target_green = RGB_BLACK;
			fg_target_red   = RGB_BLACK;
			fg_target_blue  = RGB_BLACK;
			fg_target_green = RGB_BLACK;
		}
		++animation_t;
	}
	wait(1, TITLEBANK);

	return;
}


