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

#pragma bank 2

#include <gb/cgb.h>
#include <gb/gb.h>

#include "misc.h"

const uint8_t error[6] = {0xdf, 0xec, 0xec, 0xe9, 0xec, 0xfa};
const uint8_t help[18] = {0xdf, 0xec, 0xec, 0xe9, 0xec, 0xfa, 0xe2, 0xdf, 0xe6, 0xea, 0xe7, 0x0df, 0xec, 0xec, 0xe9, 0xec, 0xfa};

#define ERROR(X, Y) {\
	for (i = 0; i < 6; ++i)\
	set_bkg_tile_xy(X + i, Y, error[i]);\
}

void glitch(void)
{
	uint8_t i;
	wait(30, 2);
	ERROR(0xa, 0xf);
	wait(12, 2);
	ERROR(0x5, 0xa);
	wait(12, 2);
	ERROR(0x12, 0x12);
	wait(12, 2);

	for (i = 0; i < 18; ++i) set_bkg_tile_xy(2 + i, 20, error[i]); 

	wait(12, 2);
	ERROR(0x17, 0x09);
	ERROR(0x03, 0x09);
	wait(12, 2);

	wait (120, 2);
	return;
}

