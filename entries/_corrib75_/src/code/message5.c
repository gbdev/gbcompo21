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

#include "../borrowed/gbt_player.h"


#include "player.h"
#include "levels.h"
#include "messagebox.h"
#include "message5.h"
#include "misc.h"

#define MESSAGE5_BANK 5
#pragma bank 5

char char_encode5(char);

#define BREAKABLE_WAIT {btn=joypad(); if ((btn & J_A) || (btn & J_B) || (btn & J_START)) return FALSE; wait(1, MESSAGE5_BANK);}


/* boolean message5(const char *message)
 *
 * Display a message in a text box at the bottom of the screen w/ automatic scrolling.
 * 
 * Call this function once. Returns TRUE if the message has been scrolled through.
 * or FALSE if interupted by a key press (a, b start)
 */
boolean message5(const char *message)
{
	if(message == NULL) return TRUE;

	/* _ newline*/
	uint8_t i;
	uint8_t line_count = 1;
	uint8_t char_count = 1;
	char current_char = 0;
	uint8_t wait_counter;


	/* blank window */	
	for(line_count = 1; line_count < 4; ++line_count) {
		for(char_count = 1; char_count < 19; ++char_count) {
		//VBK_REG = 1;
		//set_win_tile_xy(char_count, line_count, 0);
			VBK_REG = 0;
			set_win_tile_xy(char_count, line_count, 200);
		}
	}

	/* Scroll window onto screen */
	disable_interrupts();
	CRITICAL {
		WX_REG = 7;
		WY_REG = 0;
		window_height = 48;
		LYC_REG = 0;
		set_interrupts(VBL_IFLAG | LCD_IFLAG);
	}
	enable_interrupts();
	SHOW_WIN;
	NR41_REG = 0x18;
	NR42_REG = 0x1a;
	NR43_REG = 0x41;
	NR44_REG = 0xc0;

	for (i = 0; i < 48; i+=4) {
		CRITICAL {
			WY_REG = 147 - i;
			LYC_REG = 0;
		}
		BREAKABLE_WAIT;
	}

	/* Iterate through the message, copying text tiles to window */
	line_count = 1;
	char_count = 1;
	i = 0;
	while (TRUE) {
		BREAKABLE_WAIT;
		current_char = message[i];
		++i;
		/* _ for newline: */
		if (current_char == '_') {
			line_count += 1;
			/* If the box is full... */
			if (line_count == 5) {
				for(wait_counter = 0; wait_counter < 180; ++wait_counter) BREAKABLE_WAIT;
				/* clear the text box */
				for(line_count = 1; line_count < 4; ++line_count) {
					for(char_count = 1; char_count < 19; ++char_count) {
						set_win_tile_xy(char_count, line_count, 200);
					}
				}
				/* Start again at the top*/
				line_count = 1;
			}
			char_count = 1;
			continue;

		} else if (current_char == '\0') break;
		set_win_tile_xy(char_count, line_count, char_encode5(current_char));
		++char_count;
	}
	for(wait_counter = 0; wait_counter < 180; ++wait_counter) BREAKABLE_WAIT;

//for(line_count = 1; line_count < 4; ++line_count) {
//	for(char_count = 1; char_count < 19; ++char_count) {
//		set_win_tile_xy(char_count, line_count, 200);
//	}
//}
	NR41_REG = 0x18;
	NR42_REG = 0x1a;
	NR43_REG = 0x41;
	NR44_REG = 0xc0;
	for (i = 0; i < 48; i+=4) {
		CRITICAL {
			WY_REG = 103 + i;
			LYC_REG = 103 + i;
		}
		wait(1, MESSAGE5_BANK);
		BREAKABLE_WAIT;
	}

	HIDE_WIN;
	SHOW_SPRITES;
	disable_interrupts();
	set_interrupts(VBL_IFLAG);
	enable_interrupts();
	return TRUE;
}



/* char char_encode5(char c)
 * map trom characters to tile map indices
 * c: ASCII character
 * returns: index to bg tilemap
 */
inline char char_encode5(char c)
{
	if (c >= 'A' && c <= 'Z') {
		return c + 136;
	} else if (c >= '1' && c <= '9'){
		return c + 191;
	} else if (c == '0') {
		return 0xf9;
	} else  {
		switch (c) {
			case ',': return 0xe3; break;
			case '(': return 0xe4; break;
			case '.': return 0xe5; break;
			case '/': return 0xe6; break;
			case '?': return 0xe7; break;
			case '!': return 0xe8; break;
			case ')': return 0xe9; break;
			case '\'':return 0xea; break;
			case ':': return 0xeb; break;
			default:
				  return 200;
		}

	}
}

