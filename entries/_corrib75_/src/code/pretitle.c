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

#include <gb/gb.h>
#include <gb/cgb.h>

#include "../assets/alpha.h"
#include "../assets/blank.h"
#include "../assets/blobs.h"
#include "../assets/blobs2.h"
#include "../assets/circles.h"
#include "../assets/pretext.h"
#include "../assets/pretext_map.h"

#include "misc.h"
#include "pretitle.h"
#include "palettes.h"
#include "player.h"
#include "messagebox.h"

#pragma bank 4
#define PRETITLEBANK 4


void draw_pretitle_bg(uint16_t frame);
void draw_pretitle_text(uint16_t frame);

static uint8_t n;

const uint16_t pal2[] = {
	0x7f56,
	0x4a70,
	0x02da,
	0x3422
};

const uint16_t pal3[] = {
	RGB(31, 31, 31),
	RGB(15, 15, 18),
	0x02da,
	RGB(0, 0, 4)
};

const uint16_t all_black[] = {
	RGB_BLACK,
	RGB_BLACK,
	RGB_BLACK,
	RGB_BLACK
};



/* load_pretitle()
 * call before draw_pretitle()
 */
void load_pretitle(void) 
{
	HIDE_BKG;
	HIDE_WIN;
	HIDE_SPRITES;
	set_bkg_palette(0, 1, pal2);
	set_bkg_palette(1, 1, pal3);
	set_bkg_data(64, 55, Pretext);
	set_bkg_data(0, 39, Circles);
	VBK_REG = 1;
	set_bkg_tiles(0, 0, 20, 18, BlankBlack);
	VBK_REG = 0;
	set_bkg_tiles(0, 0, 20, 18, Blobs);
	set_win_tiles(0, 0, 20, 3, PretextMap);
	if (_cpu == CGB_TYPE) {
		VBK_REG = 1;
		for (uint8_t j = 0; j < 3; ++j) {
			for (uint8_t i = 0; i < 20; ++i) {
				set_win_tile_xy(i, j, 1);
			}
		}
	}
	VBK_REG = 0;
	animation_t = 0;
	n = 0;
	SHOW_BKG;

	WY_REG = 200;
	WX_REG = 7;
	window_height = 9;
	CRITICAL {
		set_interrupts(VBL_IFLAG | LCD_IFLAG);
	}
	enable_interrupts();
}



/* draw_pretitle()
 * Draws the pre-title credits (scroll text over black & grey circles)
 * First call load_pretitle.
 * Then call this function once.
 * Returns when the pretitle screen is over.
 */
void draw_pretitle(void)
{
	uint16_t counter = 0;
	draw_pretitle_bg(counter);
	draw_pretitle_text(counter);
	SHOW_BKG;
	HIDE_WIN;
	wait(1, PRETITLEBANK);
	while (counter < 880) {
		++counter;
		draw_pretitle_bg(counter);
		draw_pretitle_text(counter);
		wait(1, PRETITLEBANK);
		btn = joypad();
		if((btn & J_A) || (btn & J_B) || (btn & J_START)) goto PRETITLE_EXIT;
	}
PRETITLE_EXIT:
	/* */
	disable_interrupts();
	set_interrupts(VBL_IFLAG);
	enable_interrupts();
	set_bkg_data(0, 4, Circles + 36);
	WY_REG = 0;
	for(int i = 0; i < 8; ++i) set_bkg_palette(i, 1, all_black);
	wait(2, PRETITLEBANK);
	set_bkg_tiles(0, 0, 20, 18, BlankBlack);
	return;
}



/* draw_pretitle_bg(uint16_t frame)
 * draw background for one frame of the pretitle animation
 *
 * frame: # of current animation frame
 *
 * Uses the background layer and swaps out tile and map data to animate a
 * field of pulsing circles.
 */
void draw_pretitle_bg(uint16_t frame)
{
	size_t cosine_index[32] = {512, 512, 512, 448, 448, 384, 320, 320, 256, 192, 128, 128, 64, 64, 0, 0, 0, 0, 64, 64, 128, 128, 192, 256, 320, 320, 384, 448, 448, 512, 512, 512};
	static size_t indices[9] = {0, 3, 7, 11, 15, 19, 23, 27, 31};
	static uint8_t fg_red = 6;
	static uint8_t fg_green = 6;
	static uint8_t fg_blue = 6;
	static uint8_t bg_red = 2;
	static uint8_t bg_green = 2;
	static uint8_t bg_blue = 2;
	static uint8_t fg_target_red = 0;
	static uint8_t fg_target_green = 0;
	static uint8_t fg_target_blue = 0;
	static uint8_t bg_target_red = 0;
	static uint8_t bg_target_green = 0;
	static uint8_t bg_target_blue = 0;
	if (frame == 0) {
		//set_bkg_palette_entry(0, 2, RGB(fg_red, fg_green, fg_blue));
		//set_bkg_palette_entry(0, 3, RGB(bg_red, bg_green, bg_blue));
	}
	if (frame < 520) {
		++n;
		if (n >= 9) n = 0;
		set_bkg_data(4 * n , 4, Circles + cosine_index[indices[n]]);
		indices[n] += 2;
		if (indices[n] >= 32) indices[n] -= 32;
		/* fade out */
	} else if (frame < 670) {
		++n;
		if (n >= 9) n = 0;
		set_bkg_data(4 * n , 4, Circles + cosine_index[indices[n]]);
		if (indices[n] < 14 || indices[n] > 17) indices[n] += 2;
		if (indices[n] >= 32) indices[n] -= 32;
		/* switch patterns */
	} else if (frame == 670) {
		set_bkg_tiles(0, 0, 20, 18, Blobs2);
		indices[0] = 14;
		indices[1] = 15;
		indices[2] = 16;
		indices[3] = 17;
		indices[4] = 18;
		indices[5] = 19;
		indices[6] = 20;
		indices[7] = 21;
		indices[8] = 22;
		n = 8;
	} else if (frame < 775) {
		++n;
		if (n >= 9) n = 0;
		set_bkg_data(4 * n , 4, Circles + cosine_index[indices[n]]);
		indices[n] += 2;
		if (indices[n] >= 28) indices[n] -= 25;
	} else if (frame == 775) {
		indices[0] = 14;
		indices[1] = 14;
		indices[2] = 14;
		indices[3] = 14;
		indices[4] = 14;
		indices[5] = 14;
		indices[6] = 14;
		indices[7] = 14;
		indices[8] = 14;
	} else if ( frame < 865) {
		++n;
		if (n >= 9) n = 0;
		set_bkg_data(4 * n , 4, Circles + cosine_index[indices[n]]);
		if (indices[n] <= 28) indices[n] += 2;

		++animation_t;
		if (animation_t % 3 == 0) {
			if (fg_red > fg_target_red) --fg_red;
			if (fg_green > fg_target_green) --fg_green;
			if (fg_blue > fg_target_blue) --fg_blue;
			if (bg_red > bg_target_red) --bg_red;
			if (bg_green > bg_target_green) --bg_green;
			if (bg_blue > bg_target_blue) --bg_blue;
		}
		set_bkg_palette_entry(0, 2, RGB(fg_red, fg_green, fg_blue));
		set_bkg_palette_entry(0, 3, RGB(bg_red, bg_green, bg_blue));
	} else {
		++n;
		if (n >= 9) n = 0;
		set_bkg_data(4 * n , 4, Circles + 576);
	}
	return;
}



/* draw_pretitle_text(uint16_t frame)
 * draw text for one frame of the pretitle animation
 *
 * frame: # of current animation frame
 *
 * Uses the window to scroll lines of text from the bottom of the screen.
 */
void draw_pretitle_text(uint16_t frame)
{
	if (frame < 210) {
		if (200 - frame > 68 ) {
			WY_REG = 200 - frame;
			LYC_REG = WY_REG;
		}
	} else if (frame == 210) {
		set_win_tiles(0, 0, 20, 1, PretextMap + 20);
	} else if (frame < 420) {
		if (200 - (frame - 210) > 68 ) {
			WY_REG = 200 - (frame - 210);
			LYC_REG = WY_REG;
		}
	} else if (frame == 420) {
		set_win_tiles(0, 0, 20, 1, PretextMap + 40);
	} else if (frame < 630) {
		if (200 - (frame - 420) > 68 ) {
			WY_REG = 200 - (frame - 420);
			LYC_REG = WY_REG;
		}
	} else {
		HIDE_WIN;
		WY_REG = 200;
	}
	return;
}

