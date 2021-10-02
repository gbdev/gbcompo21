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
#include "misc.h"

#define MESSAGE_BANK 2
#pragma bank 2

char char_encode(char);
uint8_t window_height;



/* messagebox(const char *message)
 *
 * Display a message in a text box.
 *
 * Box appears at the top or bottom of the screen, depending on the player's
 * position on screen. Uses the window_handler ISR to scroll the window from
 * the top or bottom edge.
 * 
 * Call this function once. Returns when the message has been scrolled through.
 */
void messagebox(const char *message)
{
	if(message == NULL) return;

	/* _ newline*/
	uint8_t i;
	uint8_t line_count = 1;
	uint8_t char_count = 1;
	char current_char = 0;
	uint8_t local_t;
	boolean top = screen_y > 72;


	/* blank window */	
	for(line_count = 0; line_count < 6; ++line_count) {
		for(char_count = 0; char_count < 20; ++char_count) {
			VBK_REG = 1;
			set_win_tile_xy(char_count, line_count, 0);
			VBK_REG = 0;
			set_win_tile_xy(char_count, line_count, 218);
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
		if (top) {
			window_height = i;
			LYC_REG = 1;
		} else {
			CRITICAL {
				WY_REG = 145 - i;
				LYC_REG = 0;
			}
		}
		wait(1, MESSAGE_BANK);
	}

	/* Iterate through the message, copying text tiles to window */
	line_count = 1;
	char_count = 1;
	i = 0;
	while (TRUE) {
		wait(1, MESSAGE_BANK);
		current_char = message[i];
		++i;
		/* _ for newline: */
		if (current_char == '_') {
			line_count += 1;
			/* If the box is full... */
			if (line_count == 5) {
				/* wait for key */
				local_t = 0;
				while (TRUE) {
					++local_t;
					if (local_t % 32 == 0) {
						set_win_tile_xy(18, 4, 254);
					} else if (local_t % 32 == 16) {
						set_win_tile_xy(18, 4, 255);
					}
					btn = joypad();
					if (btn & J_A || btn & J_B) break;
					wait(1, MESSAGE_BANK);
				}
				hide_sprite(8);
				/* clear the text box */
				for(line_count = 0; line_count <= 5; ++line_count) {
					for(char_count = 0; char_count < 20; ++char_count) {
						set_win_tile_xy(char_count, line_count, 218);
					}
				}
				/* Start again at the top*/
				line_count = 1;
			}
			char_count = 1;
			continue;

		} else if (current_char == '\0') break;
		set_win_tile_xy(char_count, line_count, char_encode(current_char));
		++char_count;
	}
	/* wait for key */

	local_t = 0;
	while (TRUE) {
		++local_t;
		if (local_t % 32 == 0) {
			set_win_tile_xy(18, 4, 254);
		} else if (local_t % 32 == 16) {
			set_win_tile_xy(18, 4, 255);
		}
		btn = joypad();
		if (btn & J_A || btn & J_B) break;
		wait(1, MESSAGE_BANK);
	}

	for(line_count = 0; line_count <= 5; ++line_count) {
		for(char_count = 0; char_count <= 20; ++char_count) {
			set_win_tile_xy(char_count, line_count, 218);
		}
	}
	NR41_REG = 0x18;
	NR42_REG = 0x1a;
	NR43_REG = 0x41;
	NR44_REG = 0xc0;
	for (i = 0; i < 48; i+=4) {
		if (top) {
			window_height = 48 - i;
		} else {
			CRITICAL {
				WY_REG = 97 + i;
				LYC_REG = 0;
			}
		}
		wait(1, MESSAGE_BANK);
	}

	HIDE_WIN;
	SHOW_SPRITES;
	disable_interrupts();
	set_interrupts(VBL_IFLAG);
	enable_interrupts();
}



/* char char_encode(char c)
 * map trom characters to tile map indices
 * c: ASCII character
 * returns: index to bg tilemap
 */
inline char char_encode(char c)
{
	if (c >= 'A' && c <= 'Z') {
		return c + 154;
	} else  {
		switch (c) {
			case ',': return 0xf5; break;
			case '(': return 0xf6; break;
			case '.': return 0xf7; break;
			case '/': return 0xf8; break;
			case '?': return 0xf9; break;
			case '!': return 0xfa; break;
			case ')': return 0xfb; break;
			case '\'': return 0xfc; break;
			case ':': return 0xfd; break;
			case '7': return 0xd8; break;
			case '5': return 0xd9; break;
			default:
				  return 218;
		}

	}
}

