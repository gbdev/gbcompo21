#include "global.h"

void init_end()
{
	unsigned char buffer[10];
	UBYTE i;
	UWORD w;
	
	//load tiles
	UNAPACK( end_tiles, buf, DATA_BANK);
	set_bkg_data(0,49,buf);
	
	//set background
	UNAPACK( end_map, buf, DATA_BANK);
	w = 0;
	for( i = 0; i != 18; i++ )
	{
		set_bkg_tiles(0,i,20,1,&buf[w]);
		w += 20;
	}
	
	//score
	bcd2text(&score, 0x27, buffer);	
	set_bkg_tiles(7, 13, 6, 1, &buffer[2]);
}

void update_end()
{
	if( KEY_TICKED( J_START ) )
	{
		game_state = GS_TITLE;
		set_sound( SND_START );
	}
}

void run_end()
{
	UBYTE i;
	
	set_interrupts( VBL_IFLAG );
	
	init_end();
	init_sound();
		
	gbt_play(end_music, MUSIC_BANK, 7);
		
	SHOW_BKG;
	HIDE_SPRITES;
	HIDE_WIN;
	DISPLAY_ON;
	enable_interrupts();
	
	fade_from_white();
	
	while( game_state == GS_END )
	{
		UPDATE_KEYS();
		update_end();
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