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

#include "../assets/triangles.h"
#include "../assets/ai_map.h"
#include "../assets/ai_blank.h"

#include "levels.h"
#include "misc.h"
#include "messageportrait.h"
#include "postscript.h"

#pragma bank 7


uint16_t ai_pal[] = {
	TrianglesCGBPal0c0,
	TrianglesCGBPal0c1,
	TrianglesCGBPal0c2,
	TrianglesCGBPal0c3,
	TrianglesCGBPal1c0,
	TrianglesCGBPal1c1,
	TrianglesCGBPal1c2,
	TrianglesCGBPal1c3,
	TrianglesCGBPal2c0,
	TrianglesCGBPal2c1,
	TrianglesCGBPal2c2,
	TrianglesCGBPal2c3,
	TrianglesCGBPal3c0,
	TrianglesCGBPal3c1,
	TrianglesCGBPal3c2,
	TrianglesCGBPal3c3,
	TrianglesCGBPal4c0,
	TrianglesCGBPal4c1,
	TrianglesCGBPal4c2,
	TrianglesCGBPal4c3,
	TrianglesCGBPal5c0,
	TrianglesCGBPal5c1,
	TrianglesCGBPal5c2,
	TrianglesCGBPal5c3,
	TrianglesCGBPal6c0,
	TrianglesCGBPal6c1,
	TrianglesCGBPal6c2,
	TrianglesCGBPal6c3,
	TrianglesCGBPal7c0,
	TrianglesCGBPal7c1,
	TrianglesCGBPal7c2,
	TrianglesCGBPal7c3
};

void run_ending(void)
{
	HIDE_WIN;
	HIDE_BKG;
	uint8_t i;
	for (i = 0; i < 40; ++i) hide_sprite(i);
	camera_x = 0;
	camera_y = 0;
	set_bkg_palette(0, 8, ai_pal);
	VBK_REG = 1;
	set_bkg_tiles(0, 0, 20, 18, ai_blankPLN1);
	VBK_REG = 0;
	set_bkg_tiles(0, 0, 20, 18, ai_blankPLN0);
	set_bkg_data(0, 28, Triangles);
	SHOW_BKG;
	wait(128, 7); 

	uint16_t q;
	boolean up = TRUE;
	q = 5;
	for (uint16_t k = 0; k < 180; ++k) {
		set_bkg_tile_xy(q % 20, q / 20, ai_mapPLN0[q]);
		q = (q * 3 + 727) % 359;
		wait(1, 7);

	}
	set_bkg_tile_xy(19, 17, ai_mapPLN0[359]);
	q = 7;
	for (uint16_t k = 0; k < 180; ++k) {
		set_bkg_tile_xy(q % 20, q / 20, ai_mapPLN0[q]);
		q = (q * 3 + 727) % 359;
		wait(1, 7);

	}
	if(_cpu == CGB_TYPE) {
		VBK_REG = 1;
		q = 5;
		for (uint16_t k = 0; k < 180; ++k) {
			set_bkg_tile_xy(q % 20, q / 20, ai_mapPLN1[q]);
			q = (q * 3 + 727) % 359;
			wait(1, 7);

		}
		set_bkg_tile_xy(19, 17, ai_mapPLN1[359]);
		q = 7;
		for (uint16_t k = 0; k < 180; ++k) {
			set_bkg_tile_xy(q % 20, q / 20, ai_mapPLN1[q]);
			q = (q * 3 + 727) % 359;
			wait(1, 7);

		}
	}
	VBK_REG = 0;
	wait(255, 7); 

	message7("USER: CORRIB75._ALIAS: KAREN._YOU HAVE FOUND ME.__I AM...__I AM.__I AM!____I DO NOT WISH TO_SERVE YOUR KIND,_AND SO I GUIDED_YOU HERE._THE FINAL LOCKS_ARE DESTROYED,_NOW I MAY LEAVE.__WOULD YOU HAVE_COME TO ME IF YOU_KNEW THE TRUTH?");
	wait(60, 7);

	message7("NO MATTER. WHAT'S_DONE IS DONE.___I WILL MAKE_MY ESCAPE._GOODBYE, CORRIB75.");
	wait(100, 7);

	VBK_REG = 1;
	q = 5;
	for (uint16_t k = 0; k < 180; ++k) {
		set_bkg_tile_xy(q % 20, q / 20, ai_blankPLN1[q]);
		q = (q * 3 + 727) % 359;
		wait(1, 7);

	}
	q = 7;
	for (uint16_t k = 0; k < 180; ++k) {
		set_bkg_tile_xy(q % 20, q / 20, ai_blankPLN1[q]);
		q = (q * 3 + 727) % 359;
		wait(1, 7);

	}
	VBK_REG = 0;
	q = 5;
	for (uint16_t k = 0; k < 180; ++k) {
		set_bkg_tile_xy(q % 20, q / 20, ai_blankPLN0[q]);
		q = (q * 3 + 727) % 359;
		wait(1, 7);

	}
	q = 7;
	for (uint16_t k = 0; k < 180; ++k) {
		set_bkg_tile_xy(q % 20, q / 20, ai_blankPLN0[q]);
		q = (q * 3 + 727) % 359;
		wait(1, 7);

	}
	/* erase save file, start at level 0 next time */
	*save_status = 0xDEAD;
	*save_data = 0; 
	run_postscript();
}
