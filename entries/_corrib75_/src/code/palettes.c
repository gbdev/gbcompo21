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
#include <string.h>

#include <gb/cgb.h>
#include <gb/gb.h>

#include "levels.h"

#pragma bank 2

static uint8_t j;

uint8_t bg_pal_buffer[96];
uint8_t fg_pal_buffer[96];

const uint16_t black_pal[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


const uint16_t box2_pal[] = {
        0,
        0x7fff,
        0x03ff,
        RGB_BLACK
};

const uint16_t exclam_pal[] = {
        0,
        RGB_YELLOW,
        RGB_DARKYELLOW,
        RGB_BLACK
};


const uint8_t player_pal_rgb0[12] = {
	0,0,0,
	0x1e, 0x16, 0x0d,
	0x1c, 0x09, 0x00,
	0, 0, 0

};	

const uint8_t player_pal_rgb1[12] = {
	0, 0, 0,
	0x1e, 0x1e, 0x1e,
	0x1e, 0x1e, 0x0,
	0x0a, 0x00, 0x0b
};	

const uint8_t laser_pal_rgb0[12] = {
	0x1e, 0x1e, 0x00,
	0x1e, 0x0f, 0x00,
	0x1a, 0x07, 0x00,
	0x0c, 0x00, 0x00
};	

const uint8_t tele_pal_rgb0[12] = {
	0, 0, 0,
	0x05, 0x0a, 0x1f,
	0x0e, 0x10, 0x1f,
	0x02, 0x03, 0x08,
};	

const uint8_t npc_pal_rgb0[12] = {
	0, 0, 0,
	0x1a, 0x16, 0x0c,
	0x06, 0x10, 0x15,
	0, 0, 0
};	
const uint8_t npc_pal_rgb1[12] = {
	0, 0, 0,
	0x1f, 0x1a, 0x0f,
	0x10, 0x04, 0x0,
	0, 0, 0
};	
const uint8_t npc_pal_rgb2[12] = {
	0, 0, 0,
	0x08, 0x05, 0x0,
	0x10, 0x10, 0,
	0, 0, 0
};	

const uint8_t profd_pal_rgb0[12] = {
	0, 0, 0,
	0x1a, 0x16, 0x0c,
	0x15, 0x15, 0x17,
	0, 0, 0
};	

const uint8_t ai_pal_rgb[12] = {
	0, 0, 0,
	0x0a, 0x0d, 0x15,
	0x12, 0x06, 0x05,
	0, 0, 4
};	

const uint8_t wall_pal_rgb1[12] = {
	0x0a, 0x15, 0x1b,
	0x02, 0x0b, 0x11,
	0x00, 0x05, 0x0a,
	0,    0,    0
};
const uint8_t wall_pal_rgb0[12] = {
	 0x18, 0x1b,0x1f,
	 0x0d, 0x0e,0x0f,
	 0x07, 0x08,0x09,
	 0,    0,    0,   
};
const uint8_t wall_pal_rgb2[12] = {
	0x1c,  0x0f,0x07,
	0x13,  0x3,0x0,
	0x09,  0x00,0x0,
	0,     0,    0,   
};

const uint8_t floor_pal_rgb2[12] = {
	0x012, 0x10, 0x0e,
	0x0c,  0x09, 0x06,
	0x07,  0x04, 0,
	0,     0,    0
};
const uint8_t floor_pal_rgb0[12] = {
	0x10, 0x0e,0x012, 
	0x09, 0x06,0x0c,  
	0x04, 0,   0x07,  
	0,    0,    0,     
};
const uint8_t floor_pal_rgb1[12] = {
	0x012, 0x10, 0x0e,
	0x0c,  0x09, 0x06,
	0x07,  0x04, 0,
	0,     0,    0
};

const uint8_t exit_pal_rgb0[12] = {
	0x012, 0x10, 0x0e,
	0x0c,  0x09, 0x06,
	0x01,  0x09, 5,
	0x02, 0x0b, 0x11,
};

const uint8_t box_pal_rgb0[12] = {
	0x18, 0x1c, 0x0a,
	0x1f, 0x0d, 0x00,
	0x13, 0x00, 0x00,
	0x09, 0,    0,
};

const uint16_t box1_pal[] = {
        0x2b98,
        0x01bf,
        0x0013,
	0x0009
};


void dim_buffer(uint8_t n)
{
	for (uint8_t i = 0; i < 96; ++i) {
		bg_pal_buffer[i] >>= n;
		fg_pal_buffer[i] >>= n;
	}
}

void load_pal_buffer(void)
{
	memcpy(bg_pal_buffer, levels_list[current_level].wall_pal_rgb, 12);
	memcpy(bg_pal_buffer + 12, levels_list[current_level].floor_pal_rgb, 12);
	memcpy(bg_pal_buffer + 72, exit_pal_rgb0, 12);
	memcpy(bg_pal_buffer + 84, box_pal_rgb0, 12);

	memcpy(fg_pal_buffer, player_pal_rgb0, 12);
	memcpy(fg_pal_buffer + 12, player_pal_rgb1, 12);
	memcpy(fg_pal_buffer + 72, laser_pal_rgb0, 12);
	if (levels_list[current_level].npc_pal_rgb == NULL) {
		memcpy(fg_pal_buffer + 60, npc_pal_rgb0, 12);
	} else {
		memcpy(fg_pal_buffer + 60, levels_list[current_level].npc_pal_rgb, 12);
	}
	memcpy(fg_pal_buffer + 48, tele_pal_rgb0, 12);
}

void set_pal_from_buffer(void)
{
	uint8_t palette = 0;
	uint8_t entry = 0;
	uint8_t *p = bg_pal_buffer;
	for (uint8_t i = 0; i < 32; ++i) {
		uint8_t r,g,b;
		r = *p++;
		g = *p++;
		b = *p++;
		set_bkg_palette_entry(palette, entry, RGB(r, g, b));
		++entry;
		if (entry == 4) {
			entry = 0;
			++palette;
		}
	}
	palette = 0;
	entry = 0;
	p = fg_pal_buffer;
	for (uint8_t i = 0; i < 32; ++i) {
		uint8_t r,g,b;
		r = *p++;
		g = *p++;
		b = *p++;
		set_sprite_palette_entry(palette, entry, RGB(r, g, b));
		++entry;
		if (entry == 4) {
			entry = 0;
			++palette;
		}
	}
}
