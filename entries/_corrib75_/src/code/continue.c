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

#pragma bank 7

#include <gb/cgb.h>
#include <gb/hardware.h>
#include <gb/gb.h>

#include "../assets/triangles.h"
#include "../assets/continue_tiles.h"
#include "../assets/ai_blank.h"

#include "levels.h"
#include "misc.h"
#include "ending.h"
#include "messagebox.h"
#include "postscript.h"

uint16_t bkg_pal[] = {0, 0, 0x1ce7, 0x7e0e};
void run_continue_screen()
{

	HIDE_WIN;
	WX_REG = 180;
	uint8_t i;
	for (i = 0; i < 40; ++i)hide_sprite(i);
	wait(8, 7); 
	enum option {new_game, continue_game} selection = continue_game;

	set_sprite_data(128, 24, ContinueTiles);
	set_sprite_palette(0, 1, logo_pal);
	set_bkg_palette(0, 1, bkg_pal);
	SPRITES_8x8;
	for (i = 0; i < 5; ++i) {
		set_sprite_tile(i, 128 + i);
		move_sprite(i, 40 + 8 * i, 80);
		set_sprite_prop(i, 16);
	}
	for (i = 5; i < 11; ++i) {
		set_sprite_tile(i, 128 + i);
		move_sprite(i, 60 + 8 * i, 110);
		set_sprite_prop(i, 16);
	}
	set_sprite_tile(11, 151);
	set_sprite_prop(11, 16);

	for (i = 12; i < 24; ++i) {
		set_sprite_tile(i, 127 + i);
		set_sprite_prop(i, 16);
	}
	for (i = 0; i < 6; ++i) {
		move_sprite(12 + i, 10 + i * 8, 20);
		move_sprite(18 + i, 10 + i * 8, 30);
	}
	SHOW_BKG;
	SHOW_SPRITES;

	while (TRUE) {
		if (selection == new_game) {
			move_sprite(11, 90, 110); 
		} else {
			move_sprite(11, 30, 80); 
		}
		btn = joypad();
		if ((btn & J_UP || btn & J_LEFT) && selection == new_game) {
			selection = continue_game;
		} else if ((btn & J_DOWN || btn & J_RIGHT) && selection == continue_game) {
			selection = new_game;
		} else if ((btn & J_START) || (btn & J_A)) {
			if (selection == new_game) {
				current_level = 0;
				break;
			} else {
				break;
			}
		}



		wait(1, 7);
	}
	//  clean up display
}
