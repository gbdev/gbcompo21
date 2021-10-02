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

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/bgb_emu.h>
#include <gb/sgb.h>

#include "../borrowed/sgb_border.h"
#include "../borrowed/gbt_player.h"

#include "../assets/alpha.h"
#include "../assets/blank.h"
#include "../assets/bg_tiles.h"
#include "../assets/border_data.h"
#include "../assets/music1.h"
#include "../assets/sprite_tiles.h"

#include "ending.h"
#include "continue.h"
#include "intro.h"
#include "pretitle.h"
#include "title.h"
#include "levels.h"
#include "misc.h"
#include "palettes.h"
#include "player.h"
#include "lasers.h"
#include "npc.h"
#include "messagebox.h"
#include "messageportrait.h"

#pragma bank 0


/*
 * TODO


 */

void reset_level(void);


uint8_t btn = 0;
static uint8_t i;
static uint8_t t;



void vbl_handler(void)
{
	SCY_REG = camera_y;
	SCX_REG = camera_x;
}

void window_handler(void)
{
	if (LYC_REG == WY_REG) {
		SHOW_WIN;
		HIDE_SPRITES;
		LYC_REG = LYC_REG + window_height;
	} else {
		HIDE_WIN;
		SHOW_SPRITES;
		LYC_REG = WY_REG;
	}
}



void main(void)
{


	HIDE_BKG;
	HIDE_SPRITES;
	HIDE_WIN;
	/* Add isr to draw window on correct scanlines: */
	CRITICAL {
		STAT_REG |= 0b01000000;
		add_VBL(vbl_handler);
		add_LCD(window_handler);
		set_interrupts(VBL_IFLAG);
	}
	enable_interrupts();

	/* Start gbt-player */	
	disable_interrupts();
	gbt_stop();
	gbt_enable_channels(0x0);
	set_interrupts(VBL_IFLAG);
	enable_interrupts();

	fixed_camera = FALSE;

	SWITCH_ROM_MBC1(5);
	set_sgb_border(border_chr, border_chr_size, border_map, border_map_size, border_pal, border_pal_size);

	SWITCH_ROM_MBC5(5);
	set_bkg_data(218, 38, Alpha);

	SWITCH_ROM_MBC5(4);
	load_pretitle();
	draw_pretitle();

	SWITCH_ROM_MBC5(4);
	set_win_tiles(0, 0, 20, 18, BlankBlack);

	SWITCH_ROM_MBC5(5);
	load_intro();
	draw_intro();

	/* Music */

	disable_interrupts();
	gbt_play(Music1_Data, 6, 6);
	gbt_enable_channels( GBT_CHAN_1 | GBT_CHAN_2 | GBT_CHAN_3);
	gbt_loop(1);
	set_interrupts(VBL_IFLAG);
	enable_interrupts();

	SWITCH_ROM_MBC5(4);
	load_title();
	draw_title();



	/* DETECT SAVE GAME */	
	current_level = 0;
	ENABLE_RAM_MBC5;
	SWITCH_RAM_MBC5(0);
	if (*save_status == 0x1337) {
		current_level = *save_data;
	}

	if (current_level > 0) {
		SWITCH_ROM_MBC5(7);
	       	run_continue_screen();
	}
	
	/* Get ready for levels */	
	SWITCH_ROM_MBC5(PALETTE_BANK);
	for(uint8_t i = 0; i < 96; ++i) {
		bg_pal_buffer[i] = 0;
		fg_pal_buffer[i] = 0;
	}
	set_pal_from_buffer();



	/* Load background */
	SWITCH_ROM_MBC5(1);
	set_bkg_data(0, 128, BGTiles);

	/* Load box tiles: */
	SWITCH_ROM_MBC5(2);
	set_sprite_data(128, 8, sprites + 248 * 16);
//init_npcs();

	SWITCH_ROM_MBC5(5);
	set_bkg_data(218, 38, Alpha);
	set_bkg_data(216, 1, Alpha + 46 * 16);
	set_bkg_data(217, 1, Alpha + 44 * 16);

	load_level(current_level);
	enter_level();


	while(1) {
		gbt_update(); // This will change to ROM bank 1.

		if (player_win) {
			++current_level;
			if (current_level < NLEVELS) { 
				*save_status = 0x1337;
				*save_data = current_level; 
			} else {
				*save_status = 0xDEAD;
				*save_data = 0; 
			}

			SWITCH_ROM_MBC5(2);
			advance_level();

			load_level(current_level);

			enter_level();
		}

		btn = joypad();
		/*if (btn & J_START) {
			++current_level;
			load_level(current_level);
		} else*/ if (btn & J_SELECT) {
			reset_level();
		} else if (btn & J_B) {
			SWITCH_ROM_MBC5(2);
			charge();
		} else if (btn & J_A) {
			SWITCH_ROM_MBC5(2);
			npc_talk();
		}

		update_player();
		SWITCH_ROM_MBC5(2);
		zap();
		draw_boxes();
		draw_player();
		draw_npcs();
		SWITCH_ROM_MBC5(4);
		draw_lasers();

		wait_vbl_done();
	}
}

void end(void) {
	fade_to_black();
	SWITCH_ROM_MBC5(5);
	set_bkg_data(200, 51, Alpha);
	SWITCH_ROM_MBC5(7);
	run_ending();

}
