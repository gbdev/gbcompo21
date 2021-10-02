#include <stdint.h>
#include <types.h>
#include <string.h>
#include <stdio.h>

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/bgb_emu.h>

#include "levels.h"

#pragma bank 2

uint8_t uncovered_floors[23][4] = {
	// no walls, just shadows
	{73, 66, 65, 67},
	{68, 70, 65, 67},
	{71, 66, 69, 67},
	{75, 70, 65, 67},
	{74, 66, 69, 67},
	{64, 66, 65, 67},
	// straight wall below
	{73, 66, 48, 50},
	{68, 70, 48, 50},
	{75, 70, 48, 50},
	{64, 66, 48, 50},
	// left corner wall below
	{73, 66, 49, 50},
	{68, 70, 49, 50},
	{75, 70, 49, 50},
	{64, 66, 49, 50},
	// right corner wall below
	{73, 66, 48, 51},
	{68, 70, 48, 51},
	{75, 70, 48, 51},
	{64, 66, 48, 51},
	// both corners, wall below
	{73, 66, 49, 51},
	{68, 70, 49, 51},
	{75, 70, 49, 51},
	{64, 66, 49, 51},
	// wall left and below - invalid box source, player can start here
	{71, 66, 48, 51}
};

char debug_str[32];
void uncover_square(uint8_t x, uint8_t y, uint8_t n) {
	if (n > 22) return; 
	VBK_REG = 1;
	set_bkg_tile_xy(x,     y, 1);
	set_bkg_tile_xy(x + 1, y, 1);
	if (n < 6) {
		set_bkg_tile_xy(x,     y + 1, 1);
		set_bkg_tile_xy(x + 1, y + 1, 1);
	} else {
		set_bkg_tile_xy(x,     y + 1, 0x80);
		set_bkg_tile_xy(x + 1, y + 1, 0x80);
	}
	VBK_REG = 0;
	set_bkg_tile_xy(x,     y,     uncovered_floors[n][0]);
	set_bkg_tile_xy(x + 1, y,     uncovered_floors[n][1]);
	set_bkg_tile_xy(x,     y + 1, uncovered_floors[n][2]);
	set_bkg_tile_xy(x + 1, y + 1, uncovered_floors[n][3]);
}
