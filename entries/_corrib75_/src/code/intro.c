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
#include <gb/hardware.h>
#include <gb/gb.h>

#include "../assets/alpha.h"
#include "../assets/boot.h"
#include "../assets/intro_map.h"
#include "../assets/karen1.h"
#include "../assets/profd.h"
#include "../assets/frame_tiles.h"
#include "../assets/frame_map.h"

#include "intro.h"
#include "misc.h"
#include "levels.h"
#include "message5.h"

#pragma bank 5
#define INTROBANK 5

#define BLIP {NR10_REG = 0x25; NR11_REG = 0x81; NR12_REG = 0x64; NR13_REG = 0xD3; NR14_REG = 0x86;}

void load_intro(void);
void draw_intro(void);
boolean draw_boot(void);
boolean draw_irc(void);
boolean draw_convo(void);

/* load_intro()
 * call before draw_intro()
 */
void load_intro(void) 
{
	set_bkg_data(0, 52, Alpha);
	WY_REG = 0;
	SHOW_WIN;
	set_bkg_tiles(0, 0, 20, 27, IntroMap);
	set_bkg_palette_entry(0, 0, RGB_WHITE);
	NR52_REG = 0x80;
	NR50_REG = 0x77;
	NR51_REG = 0xff;
}



void draw_intro()
{
	if (draw_irc() && draw_convo() && draw_boot()) return;
	//if (draw_convo() && draw_boot()) return;

	return;
}

#define BREAKABLE_WAIT {btn=joypad(); if ((btn & J_A) || (btn & J_B) || (btn & J_START)) return FALSE; wait(1, INTROBANK);}
boolean draw_convo(void)
{
	if (_cpu == DMG_TYPE) HIDE_BKG;
	HIDE_WIN;
	const uint16_t black_palette[] = {0, 0, 0, 0}; 
	const uint16_t white_palette[] = {RGB_WHITE,RGB_WHITE,RGB_WHITE,RGB_WHITE}; 
	set_bkg_palette(0, 1, black_palette);
	camera_x = 80;
	camera_y = 0;
	WY_REG = 200;
	const uint16_t portrait_palette[] = {RGB(31,31,31), RGB(15, 15, 17), RGB(8, 8, 10), RGB(1, 1, 2)};
	const uint16_t portrait_palette_dark1[] = {RGB(24,24,24), RGB(8, 8, 10), RGB(2, 2, 4), RGB(0, 0, 0)};
	const uint16_t portrait_palette_dark2[] = {RGB(8,8,10), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0)};
	const uint16_t portrait_palette_light1[] = {RGB(31,31,31), RGB(24, 24, 25), RGB(15, 15, 7), RGB(8, 8, 10)};
	const uint16_t portrait_palette_light2[] = {RGB(31,31,31), RGB(31, 31, 31), RGB(29, 29, 31), RGB(15, 15, 17)};
	const uint16_t frame_palette[] = {RGB(31,31,31), RGB(15, 15, 17), RGB(10, 10, 15), RGB(4, 4, 10)};
	uint8_t x;
	uint8_t i;
	uint8_t line_count, char_count;
	VBK_REG = 1;
	for( line_count = 0; line_count < 5; ++line_count) {
		for( char_count = 0; char_count < 20; ++char_count) {
			set_win_tile_xy(char_count, line_count, 1);
		}
	}
	VBK_REG = 0;

	set_bkg_palette(0, 1, black_palette);
	set_bkg_data(192, 8, Frame);
	set_win_tiles(0, 0, 20, 5, FrameMap);

	set_bkg_palette(1, 1, frame_palette);
	set_bkg_data(200, 52, Alpha);
	set_bkg_data(0, 170, karen1_tiles);
	set_bkg_tiles(0, 0, 20, 18, karen1_map);
	set_bkg_palette(0, 1, portrait_palette_dark2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_dark1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	WY_REG = 200;
	camera_x = 40;
	SHOW_BKG;
	BREAKABLE_WAIT;
	for (x = 40; x > 0; x-=2) {
		camera_x = x;
		BREAKABLE_WAIT;
	}	

	for (i = 0; i < 60; ++i) BREAKABLE_WAIT;
	if (!message5("OK, PROFESSOR,_I'M HERE. WHAT'S_THE BIG NEWS?")) return FALSE;
	for (i = 0; i < 60; ++i) BREAKABLE_WAIT;

	if (_cpu == DMG_TYPE) HIDE_BKG;
	set_bkg_palette(0, 1, portrait_palette_light1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_light2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, white_palette);
	set_bkg_data(0, 190, profd_tiles);
	set_bkg_tiles(0, 0, 20, 18, profd_map);
	set_bkg_palette(0, 1, portrait_palette_light2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_light1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette);
	camera_x = 230;
	SHOW_BKG;	
	BREAKABLE_WAIT;
	for (x = 230; x < 254; x+=2) {
		camera_x = x;
		BREAKABLE_WAIT;
	}
	if(!message5("KAREN! JUST IN_TIME! WE'RE ABOUT_TO GO LIVE.__MY VIRTUAL WORLD_IS READY FOR_ITS FIRST USERS.__I HAVE SOME VIPS_WAITING TO DIAL_IN.__I HOPE YOU'LL_JOIN THEM!___YOUR RESEARCH WAS_INDISPENSIBLE TO_MY SUCCESS.")) return FALSE;
	if(_cpu == DMG_TYPE) HIDE_BKG;
	set_bkg_palette(0, 1, portrait_palette_dark1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_dark2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, black_palette);
	BREAKABLE_WAIT;

	set_bkg_data(200, 52, Alpha);
	set_bkg_data(0, 170, karen1_tiles);
	set_bkg_tiles(0, 0, 20, 18, karen1_map);
	WY_REG = 200;
	set_bkg_palette(0, 1, portrait_palette);
	camera_x = 30;
	SHOW_BKG;
	BREAKABLE_WAIT;
	for (x = 30; x > 0; x-=2) {
		camera_x = x;
		BREAKABLE_WAIT;
	}	
	for (i = 0; i < 60; ++i) BREAKABLE_WAIT;
	if(!message5("SOUNDS BETTER THAN_LATE NIGHT TV.___BUT WHAT WAS YOUR_BREAKTHROUGH?")) return FALSE;
	for (i = 0; i < 60; ++i) BREAKABLE_WAIT;

	HIDE_BKG;
	set_bkg_palette(0, 1, black_palette);
	BREAKABLE_WAIT;
	
	set_bkg_data(0, 190, profd_tiles);
	set_bkg_tiles(0, 0, 20, 18, profd_map);
	camera_x = 220;
	set_bkg_palette(0, 1, portrait_palette_light2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_light1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette);
	SHOW_BKG;
	for (x = 220; x < 254; x+=2) {
		camera_x = x;
		BREAKABLE_WAIT;
	}
	if(!message5("AHA! YOU SEE,_THIS NEW WORLD IS_BUILT AROUND__A CORE AI, WHICHs_MAKES ADJUSTMENTS_TO THE EXPERIENCE__TO ENSURE MAXIMUM_COHERENCE FOR_EACH USER.__THE AI'S A GENIUS.__I'M A GENIUS!__BUT ENOUGH CHAT._GRAB A HAPTIC SUIT_AND LET'S GO!")) return FALSE;
	HIDE_BKG;
	set_bkg_palette(0, 1, portrait_palette_dark1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_dark2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, black_palette);
	BREAKABLE_WAIT;

	set_bkg_data(200, 52, Alpha);
	set_bkg_data(0, 170, karen1_tiles);
	set_bkg_tiles(0, 0, 20, 18, karen1_map);
	WY_REG = 200;
	set_bkg_palette(0, 1, portrait_palette_dark2);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette_dark1);
	BREAKABLE_WAIT;
	BREAKABLE_WAIT;
	set_bkg_palette(0, 1, portrait_palette);
	camera_x = 30;
	SHOW_BKG;
	BREAKABLE_WAIT;
	for (x = 30; x > 0; x-=2) {
		camera_x = x;
		BREAKABLE_WAIT;
	}	
	for (i = 0; i < 60; ++i) BREAKABLE_WAIT;
	if(!message5("OK! LET'S DO THIS!")) return FALSE;
	for (i = 0; i < 60; ++i) BREAKABLE_WAIT;



	HIDE_BKG;
	set_bkg_palette(0, 1, black_palette);
	set_bkg_palette(1, 1, black_palette);
	BREAKABLE_WAIT;
	for( uint8_t line_count = 0; line_count < 5; ++line_count) {
		for( uint8_t char_count = 0; char_count < 20; ++char_count) {
			set_win_tile_xy(char_count, line_count, 200);
		}
	}
	return TRUE;
}


/* draw_irc()
 * call load_intro() first.
 * Then call this function to draw the intro screen (IRC conversation)
 *
 * The entire coversation is already on the bg tile map, but covered by the
 * window. Messages are revealed by scrolling the window down until the whole
 * background is visible, then scrolling the background up.
 *
 * Returns TRUE if the animation runs to completion
 *         or FALSE if it ends early because the player presses A, B, or START.
 */
boolean draw_irc(void)
{
	uint16_t counter = 0;
	while (counter < 2400) {
		switch (counter) {
			case 30: // joined
				WY_REG = 32;
				BLIP;
				break;
			case 250: // you're awake
				WY_REG = 56;
				BLIP;
				break;
			case 400: // come to the lab
				WY_REG = 80;
				BLIP;
				break;
			case 700: // ...
				WY_REG = 96;
				BLIP;
				break;
			case 950: // now?
				WY_REG = 112;
				BLIP;
				break;
			case 1200: // yes now!
				WY_REG = 128;
				BLIP;
				break;
			case 1450: // i solved it
				WY_REG = 152;
				BLIP;
				break;
			case 1700: // you have to see this
				/* SCY_REG is set automatically from camera_y in an ISR */
				BLIP;
				camera_y = 24;
				break;
			case 1950: // fine
				camera_y = 40;
				BLIP;
				break;
			case 2200: // omw
				camera_y = 56;
				BLIP;
				break;
			case 2310: // quit
				camera_y = 72;
			default:
				break;
		}
		++counter;
		wait(1, INTROBANK);
		btn = joypad();
		if ((btn & J_A) || (btn & J_B) || (btn & J_START)) return FALSE;
	}
	WY_REG = 0;
	return TRUE;
}


boolean draw_boot()
{
	set_bkg_data(0, 1, Alpha);
	SHOW_WIN;
	set_bkg_data(0, 52, Alpha);
	camera_x = 0;
	camera_y = 0;
	WY_REG = 0;
	set_bkg_tiles(0, 0, 20, 18, Boot);
	uint8_t ok[] = {0x0f, 0x0b};
	uint8_t ok_pal[] = {1, 1};
	uint8_t os[] = {0x26, 0x26, 0x26, 0x1d, 0x1d, 0x1d};
	uint8_t os_pal[] = {2, 2, 2, 2, 2, 2};
	set_bkg_palette_entry(0, 0, RGB_WHITE);
	set_bkg_palette_entry(1, 0, RGB_GREEN);
	set_bkg_palette_entry(2, 0, RGB_RED);
	uint16_t counter = 0;
	SHOW_BKG;
	while (counter < 350) {
		switch (counter) {
			case 30: // booting
				WY_REG = 16; break;
			case 45: 
				VBK_REG = 1;
				set_bkg_tiles(0xa, 0x1, 2, 1, ok_pal);
				VBK_REG = 0;
				set_bkg_tiles(0xa, 0x1, 2, 1, ok);
				break;
			case 60: // calibrating input
				WY_REG = 32; break;
			case 75: 
				VBK_REG = 1;
				set_bkg_tiles(0, 4, 2, 1, ok_pal);
				VBK_REG = 0;
				set_bkg_tiles(0, 4, 2, 1, ok);
				break;
			case 90: // connecting
				WY_REG = 48; break;
			case 105: 
				VBK_REG = 1;
				set_bkg_tiles(0x0d, 5, 2, 1, ok_pal);
				VBK_REG = 0;
				set_bkg_tiles(0x0d, 5, 2, 1, ok);
				break;
			case 120: // loading world:
				WY_REG = 64; break;
			case 135: 
				VBK_REG = 1;
				set_bkg_tiles(0x10, 7, 2, 1, ok_pal);
				VBK_REG = 0;
				set_bkg_tiles(0x10, 7, 2, 1, ok);
				break;
			case 150: // starting ai
				WY_REG = 80; break;
			case 165: 
				VBK_REG = 1;
				set_bkg_tiles(0x0e, 9, 6, 1, os_pal);
				VBK_REG = 0;
				set_bkg_tiles(0x0e, 9, 6, 1, os);
				break;
			case 172: 
				set_bkg_palette_entry(2, 0, RGB_WHITE);
				break;
			case 180: // reticulating splines
				WY_REG = 96; break;
			case 195: 
				VBK_REG = 1;
				set_bkg_tiles(0x03, 0x0c, 2, 1, ok_pal);
				VBK_REG = 0;
				set_bkg_tiles(0x03, 0x0c, 2, 1, ok);
				break;
			case 210: // welcome to the future
				WY_REG = 200; break;
		}
		++counter;
		wait(1, INTROBANK);
		btn = joypad();
		if ((btn & J_A) || (btn & J_B) || (btn & J_START)) return FALSE;
	}
	WY_REG = 0;
	return TRUE;
}

