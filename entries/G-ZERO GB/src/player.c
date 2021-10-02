#include "global.h"

UWORD player_x;
UWORD player_speed;
UBYTE player_count;
UBYTE player_range;
UBYTE player_animation;
UBYTE player_power;

void init_player()
{
	player_x = 77*8;
	player_speed = 0;
	player_count = 0;
	player_range = 0;
	player_animation = 0;
	player_power = 14;
		
	set_sprite_tile(SN_PLAYER1,ST_PLAYER1);
	set_sprite_tile(SN_PLAYER2,ST_PLAYER2);
	set_sprite_tile(SN_PLAYER3,ST_PLAYER3);
	
	move_sprite(SN_PLAYER1,(player_x >> 3),112);
	move_sprite(SN_PLAYER2,(player_x >> 3)+8,112);
	move_sprite(SN_PLAYER3,(player_x >> 3)+16,112);

	set_sprite_tile(SN_MINI_PLAYER1,ST_MINI_PLAYER1);
	move_sprite(SN_MINI_PLAYER1,135,130);
}

void update_player()
{	
	//handle inputs
	player_animation = 0;
	
	if( (KEY_PRESSED( J_A )) && (race_state == RS_RACE) )
	{
		if( player_speed < 363 )
		{
			player_speed++;
		}
		player_animation = 1;
	} else {
		
		if( (KEY_PRESSED( J_B )) && (race_state == RS_RACE) )
		{
			if( player_speed != 0 )
			{
				player_speed--;
			}	
		}
		if( player_speed != 0 )
		{
			player_speed--;
		}	
	
	}
		
	if( race_state == RS_RACE )
	{
		if( KEY_PRESSED( J_LEFT ) )
		{
			input_acceleration = -13;	//-13
		}
		if( KEY_PRESSED( J_RIGHT ) )
		{
			input_acceleration = 14;	//14
		} 
	}
	
	set_sound( SND_SPEED );
}