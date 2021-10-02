#include "global.h"

const unsigned char start[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x18,0x1D,0x1B,0x10,0x00,0x1B,0x1C,0x09,0x1A,0x1C
};

UBYTE title_count;

void init_title()
{
	unsigned char buffer[10];
	UBYTE i;
	UWORD w;
	
	//load tiles
	UNAPACK( title_tiles, buf, DATA_BANK);
	set_bkg_data(0,117,buf);
	
	//set background
	UNAPACK( title_map, buf, DATA_BANK);
	w = 0;
	for( i = 0; i != 18; i++ )
	{
		set_bkg_tiles(0,i,20,1,&buf[w]);
		w += 20;
	}
	move_bkg(0,0);
	
	title_count = 0;
	
	race_number = RN_RACE1;
	
	if( score > high_score )
	{
		high_score = score;
	}	
	
	//high score
	bcd2text(&high_score, 0x23, buffer);	
	set_bkg_tiles(7, 15, 6, 1, &buffer[2]);
	
	score = MAKE_BCD(0);
}

void update_title()
{
	title_count++;
		
	if( (title_count & 31) == 0 )
	{
		set_bkg_tiles(5,11,10,1,&start[10]);
	}
	
	if( (title_count & 31) == 16 )
	{
		set_bkg_tiles(5,11,10,1,&start[0]);
	}
	
	if( KEY_TICKED( J_START ) )
	{
		game_state = GS_RACE;
		set_sound( SND_START );
	}
}

void run_title()
{
	UBYTE i;
	
	set_interrupts( VBL_IFLAG );
	
	init_title();
	init_sound();
		
	gbt_play(intro_music, MUSIC_BANK, 7);
		
	SHOW_BKG;
	DISPLAY_ON;
	enable_interrupts();
	
	fade_from_white();
	
	while( game_state == GS_TITLE )
	{
		UPDATE_KEYS();
		update_title();
		update_sound();
		gbt_update();
		wait_vbl_done();
	}
	
	for( i = 0; i != 10; i++ )
	{
		update_sound();
		gbt_update();
		wait_vbl_done();
	}
	
	gbt_stop();
	fade_to_white();
	
	disable_interrupts();
	DISPLAY_OFF;
}