#include "global.h"

void main() 
{
    disable_interrupts();
	DISPLAY_OFF;
	HIDE_SPRITES;
	HIDE_WIN;
	HIDE_BKG;
	SPRITES_8x16;
	//OBP0_REG = 0xC4;
	
	BGP_REG = 0x00;
	OBP0_REG = 0x00;
	
	input = 0;
	old_input = 0;
	
	score = MAKE_BCD(0);
	high_score = MAKE_BCD(0);
	
	game_state = GS_TITLE;
	
	while (1) 
	{
		switch( game_state )
		{
			case GS_TITLE:
				run_title();
				break;
			case GS_RACE:
				run_race();
				break;
			case GS_RACE_FINISHED:
				run_race_finished();
				break;	
			case GS_END:
				run_end();
				break;	
		}
    }
}