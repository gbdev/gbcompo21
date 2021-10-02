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

#include<stdio.h>

#include "../assets/sprite_tiles.h"

#include "levels.h"
#include "messagebox.h"
#include "player.h"
#include "glitch.h"

#pragma bank 2
#define NPCBANK 2

static uint8_t i;

uint8_t npc_x = 0;
uint8_t npc_y = 0;

uint8_t npc_mx = 0;
uint8_t npc_my = 0;

void init_npcs(void)
{
	if (levels_list[current_level].npc_portrait != NULL) {
		set_sprite_data(0x4c, 4, levels_list[current_level].npc_portrait);
	}
	set_sprite_data(0x50, 4, sprites + 244 * 16);
	set_sprite_tile(36, 0x4c);
	set_sprite_tile(37, 0x4e);
	set_sprite_tile(38, 0x50);
	set_sprite_tile(39, 0x52);
	set_sprite_prop(36, 5);
	set_sprite_prop(37, 5);
	set_sprite_prop(38, 17);
	set_sprite_prop(39, 17);
}

void draw_npcs(void)
{
	if(npc_y > 0) { 
		move_sprite(36, npc_x - camera_x, 	npc_y - camera_y);
		move_sprite(37, npc_x + 8 - camera_x, 	npc_y - camera_y);
		move_sprite(38, npc_x - camera_x, 	npc_y - camera_y);
		move_sprite(39, npc_x + 8 - camera_x, 	npc_y - camera_y);
	}
}

void npc_talk(void)
{
	switch(player_face) {
		case north:
			player_hit_x1 = player_x + 2;
			player_hit_x2 = player_x + 13;
			player_hit_y1 = player_y - 13;
			player_hit_y2 = player_y;
			break;
		case south:
			player_hit_x1 = player_x + 2;
			player_hit_x2 = player_x + 13;
			player_hit_y1 = player_y + 16;
			player_hit_y2 = player_y + 29;
			break;
		case east:
			player_hit_x1 = player_x + 16;
			player_hit_x2 = player_x + 29;
			player_hit_y1 = player_y + 5;
			player_hit_y2 = player_y + 15;
			break;
		case west:
			player_hit_x1 = player_x - 13;
			player_hit_x2 = player_x;
			player_hit_y1 = player_y + 5;
			player_hit_y2 = player_y + 15;
			break;
	}
	if (npc_x > player_hit_x1 && npc_x < player_hit_x2 && npc_y - 8  > player_hit_y1 && npc_y - 8 < player_hit_y2) {
		player_state = standing;
		wait(1, NPCBANK);
		draw_player();
		if(current_level == NLEVELS - 1) {
			end();
		}	
		switch(player_face) {
			case north:
				set_sprite_data(0x4c, 4, levels_list[current_level].npc_portrait + 128);
				break;
			case south:
				set_sprite_data(0x4c, 4, levels_list[current_level].npc_portrait + 192);
				break;
			case east:
				set_sprite_data(0x4c, 4, levels_list[current_level].npc_portrait);
				break;
			case west:
				set_sprite_data(0x4c, 4, levels_list[current_level].npc_portrait + 64);
				break;
		}
		draw_npcs();
		messagebox(levels_list[current_level].npc_message);
		if(current_level == 0) {
			glitch();
			player_win = TRUE;
		} 

	}
}

