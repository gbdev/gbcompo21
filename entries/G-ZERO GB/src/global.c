#include "global.h"

UBYTE game_state;

UBYTE input;
UBYTE old_input;

BCD score;
BCD high_score;

UBYTE buf[4096];

void show_message( UBYTE message )
{
	hide_message();
	
	switch( message )
	{
		case MSG_ONE:
			set_sprite_tile(SN_MESSAGE1,ST_ONE1);
			move_sprite(SN_MESSAGE1,85,40);
			break;
		case MSG_TWO:
			set_sprite_tile(SN_MESSAGE1,ST_TWO1);
			move_sprite(SN_MESSAGE1,85,40);
			break;
		case MSG_THREE:
			set_sprite_tile(SN_MESSAGE1,ST_THREE1);
			move_sprite(SN_MESSAGE1,85,40);
			break;
		case MSG_GO:
			set_sprite_tile(SN_MESSAGE1,ST_GO1);
			set_sprite_tile(SN_MESSAGE2,ST_GO2);
			set_sprite_tile(SN_MESSAGE3,ST_GO3);
			move_sprite(SN_MESSAGE1,78,40);
			move_sprite(SN_MESSAGE2,86,40);
			move_sprite(SN_MESSAGE3,94,40);
			break;
		case MSG_TIMEUP:
			set_sprite_tile(SN_MESSAGE1,ST_TIMEUP1);
			set_sprite_tile(SN_MESSAGE2,ST_TIMEUP2);
			set_sprite_tile(SN_MESSAGE3,ST_TIMEUP3);
			set_sprite_tile(SN_MESSAGE4,ST_TIMEUP4);
			set_sprite_tile(SN_MESSAGE5,ST_TIMEUP5);
			set_sprite_tile(SN_MESSAGE6,ST_TIMEUP6);
			set_sprite_tile(SN_MESSAGE7,ST_TIMEUP7);
			move_sprite(SN_MESSAGE1,58,40);
			move_sprite(SN_MESSAGE2,66,40);
			move_sprite(SN_MESSAGE3,74,40);
			move_sprite(SN_MESSAGE4,82,40);
			move_sprite(SN_MESSAGE5,98,40);
			move_sprite(SN_MESSAGE6,106,40);
			move_sprite(SN_MESSAGE7,114,40);
			break;
		case MSG_POWEROUT:
			set_sprite_tile(SN_MESSAGE1,ST_POWEROUT1);
			set_sprite_tile(SN_MESSAGE2,ST_POWEROUT2);
			set_sprite_tile(SN_MESSAGE3,ST_POWEROUT3);
			set_sprite_tile(SN_MESSAGE4,ST_POWEROUT4);
			set_sprite_tile(SN_MESSAGE5,ST_POWEROUT5);
			set_sprite_tile(SN_MESSAGE6,ST_POWEROUT6);
			set_sprite_tile(SN_MESSAGE7,ST_POWEROUT7);
			set_sprite_tile(SN_MESSAGE8,ST_POWEROUT8);
			set_sprite_tile(SN_MESSAGE9,ST_POWEROUT9);
			move_sprite(SN_MESSAGE1,50,36);
			move_sprite(SN_MESSAGE2,58,36);
			move_sprite(SN_MESSAGE3,66,36);
			move_sprite(SN_MESSAGE4,74,36);
			move_sprite(SN_MESSAGE5,82,36);
			move_sprite(SN_MESSAGE6,98,36);
			move_sprite(SN_MESSAGE7,106,36);
			move_sprite(SN_MESSAGE8,114,36);
			move_sprite(SN_MESSAGE9,122,36);
			break;	
		case MSG_FINISH:
			set_sprite_tile(SN_MESSAGE1,ST_FINISH1);
			set_sprite_tile(SN_MESSAGE2,ST_FINISH2);
			set_sprite_tile(SN_MESSAGE3,ST_FINISH3);
			set_sprite_tile(SN_MESSAGE4,ST_FINISH4);
			set_sprite_tile(SN_MESSAGE5,ST_FINISH5);
			set_sprite_tile(SN_MESSAGE6,ST_FINISH6);
			set_sprite_tile(SN_MESSAGE7,ST_FINISH7);
			move_sprite(SN_MESSAGE1,62,40);
			move_sprite(SN_MESSAGE2,70,40);
			move_sprite(SN_MESSAGE3,78,40);
			move_sprite(SN_MESSAGE4,86,40);
			move_sprite(SN_MESSAGE5,94,40);
			move_sprite(SN_MESSAGE6,102,40);
			move_sprite(SN_MESSAGE7,110,40);
			break;
		case MSG_RACENAME:
			set_sprite_tile(SN_MESSAGE1,ST_RACENAME1);
			set_sprite_tile(SN_MESSAGE2,ST_RACENAME2);
			set_sprite_tile(SN_MESSAGE3,ST_RACENAME3);
			set_sprite_tile(SN_MESSAGE4,ST_RACENAME4);
			set_sprite_tile(SN_MESSAGE5,ST_RACENAME5);
			set_sprite_tile(SN_MESSAGE6,ST_RACENAME6);
			set_sprite_tile(SN_MESSAGE7,ST_RACENAME7);
			set_sprite_tile(SN_MESSAGE8,ST_RACENAME8);
			set_sprite_tile(SN_MESSAGE9,ST_RACENAME9);
			move_sprite(SN_MESSAGE1,53,36);
			move_sprite(SN_MESSAGE2,61,36);
			move_sprite(SN_MESSAGE3,69,36);
			move_sprite(SN_MESSAGE4,77,36);
			move_sprite(SN_MESSAGE5,85,36);
			move_sprite(SN_MESSAGE6,93,36);
			move_sprite(SN_MESSAGE7,101,36);
			move_sprite(SN_MESSAGE8,109,36);
			move_sprite(SN_MESSAGE9,117,36);
			break;
	}
}

void hide_message()
{
	UBYTE i;
	
	for( i = 0; i != 10; i++ )
	{
		move_sprite(SN_MESSAGE1+i,0,0);
	}
}

void fade_from_white()
{
	UBYTE i;
	
	for(i = 0; i != 8; i++ )
	{
		wait_vbl_done();
	}
	for(i = 0; i != 16; i++ )
	{
		if( i == 0 )
		{
			BGP_REG = 0x00;
			OBP0_REG = 0x00;
		}
		if( i == 4 )
		{
			BGP_REG = 0x40;
			OBP0_REG = 0x40;
		}
		if( i == 8 )
		{
			BGP_REG = 0x90;
			OBP0_REG = 0x80;
		}
		if( i == 12 )
		{
			BGP_REG = 0xE4;
			OBP0_REG = 0xC4;
		}
		wait_vbl_done();	
	}
}

void fade_to_white()
{
	UBYTE i;
	
	for(i = 0; i != 16; i++ )
	{
		if( i == 0 )
		{
			BGP_REG = 0xE4;
			OBP0_REG = 0xC4;
		}
		if( i == 4 )
		{
			BGP_REG = 0x90;
			OBP0_REG = 0x80;
		}
		if( i == 8 )
		{
			BGP_REG = 0x40;
			OBP0_REG = 0x40;
		}
		if( i == 12 )
		{
			BGP_REG = 0x00;
			OBP0_REG = 0x00;
		}
		wait_vbl_done();
	}
	for(i = 0; i != 8; i++ )
	{
		wait_vbl_done();
	}
}