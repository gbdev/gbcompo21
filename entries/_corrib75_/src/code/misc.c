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

#pragma bank 0

#include <gb/gb.h>

#include "../borrowed/gbt_player.h"
#include "../assets/logo_tiles.h"

#include "levels.h"
#include "misc.h"
#include "player.h"

/* some shared data: */
static uint8_t i;
uint8_t mask1;
uint8_t mask2;
uint8_t sprite_buffer[128];
uint8_t *obj_ptr1;
uint8_t *obj_ptr2;
uint8_t *obj_ptr3;
uint8_t *obj_ptr4;
uint8_t start_j;
	
uint16_t * const save_status = (uint16_t *) 0xA000;
uint8_t  * const save_data =   (uint8_t *)  0xA002;



/* wait(uint8_t frames, uint8_t returns_bank)
 * wait a specified number of frames.
 *
 * Calls wait_vbl done a number of times, but keeps music working with
 * gbt_update()
 *
 * switches to ROM bank return_bank before returning, so it can be called from
 * code in any bank.
 */
void wait(uint8_t frames, uint8_t return_bank)
{
	uint8_t i=0;
	while(i < frames) {
		gbt_update(); // This will change to ROM bank 1.
		wait_vbl_done();
		++i;
	}
	SWITCH_ROM_MBC5(return_bank);
}

void load_logo7(void)
{
	SWITCH_ROM_MBC5(4);
	set_sprite_data(0, 18, LogoTiles);
	SWITCH_ROM_MBC5(7);
}

