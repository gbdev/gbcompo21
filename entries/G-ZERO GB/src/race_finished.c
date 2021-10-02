#include "global.h"

const unsigned char game_over[] =
{
  0x00,0x00,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x00,0x00
};

UWORD points;

void init_race_finished()
{
	unsigned char buffer[10];
	UWORD bonus;
	BCD value;
	UBYTE i;
	UWORD w;
	
	//load tiles
	UNAPACK( race_finished_tiles, buf, DATA_BANK);
	set_bkg_data(0,73,buf);
	
	//set background
	UNAPACK( race_finished_map, buf, DATA_BANK);
	w = 0;
	for( i = 0; i != 18; i++ )
	{
		set_bkg_tiles(0,i,20,1,&buf[w]);
		w += 20;
	}
	if( (race_state == RS_TIMEUP) || (race_state == RS_POWEROUT) )
	{
		set_bkg_tiles(3,4,14,1,&game_over[0]);
	}
	move_bkg(0,0);
	
	//race score
	switch( race_number )
	{
		case RN_RACE1:
			race_score = ((race_score << 5) / 232) * 200;
			race_score += 800;
			break;
		case RN_RACE2:
			race_score = ((race_score << 5) / 288) * 200;
			race_score += 1600;
			break;
		case RN_RACE3:
			race_score = ((race_score << 5) / 224) * 200;
			race_score += 2400;
			break;
		case RN_RACE4:
			race_score = ((race_score << 5) / 280) * 200;
			race_score += 3200;
			break;
	}	
			
	uint2bcd(race_score, &value);
	bcd2text(&value, 0x27, buffer);	
	if( race_score < 10000 )
	{
		buffer[3] = 0;
	}
	if( race_score < 1000 )
	{
		buffer[4] = 0;
	}
	if( race_score < 100 )
	{
		buffer[5] = 0;
	}
	if( race_score < 10 )
	{
		buffer[6] = 0;
	}
	set_bkg_tiles(12, 7, 5, 1, &buffer[3]);
	
	//bonus
	if( race_state == RS_FINISH )
	{
		bonus = (seconds & 0x0F) * 200;
		bonus += (seconds >> 4) * 2000;
		if( hundredth >= MAKE_BCD(50) )
		{
			bonus += 100;
		}
		if( bonus > 5000 )
		{
			bonus = 5000;
		}
	} else {
		bonus = 0;
	}

	uint2bcd(bonus, &value);
	bcd2text(&value, 0x27, buffer);	
	if( bonus < 1000 )
	{
		buffer[4] = 0;
	}
	if( bonus < 100 )
	{
		buffer[5] = 0;
	}
	if( bonus < 10 )
	{
		buffer[6] = 0;
	}
	set_bkg_tiles(13, 8, 4, 1, &buffer[4]);
	
	//points
	points = race_score + bonus;
	uint2bcd(points, &value);
	bcd2text(&value, 0x27, buffer);	
	if( points < 10000 )
	{
		buffer[3] = 0;
	}
	if( points < 1000 )
	{
		buffer[4] = 0;
	}
	if( points < 100 )
	{
		buffer[5] = 0;
	}
	if( points < 10 )
	{
		buffer[6] = 0;
	}
	set_bkg_tiles(12, 10, 4, 1, &buffer[3]);
	
	//score
	bcd2text(&score, 0x27, buffer);	
	set_bkg_tiles(11, 13, 6, 1, &buffer[2]);
}

void update_race_finished()
{
	if( KEY_TICKED( J_START ) )
	{
		if( (race_state == RS_TIMEUP) || (race_state == RS_POWEROUT) )
		{
			game_state = GS_TITLE;
		} else {
			if( race_number == RN_RACE4 )
			{
				game_state = GS_END;
			} else {
				game_state = GS_RACE;
				race_number++;
			}
		}		
		set_sound( SND_START );
	}
}

void run_race_finished()
{
	unsigned char buffer[10];
	BCD value;
	UBYTE i;
	
	set_interrupts( VBL_IFLAG );
	
	init_race_finished();
	init_sound();
		
	gbt_play(end_music, MUSIC_BANK, 7);
		
	SHOW_BKG;
	HIDE_SPRITES;
	HIDE_WIN;
	DISPLAY_ON;
	enable_interrupts();
	
	fade_from_white();
	
	for( i = 0; i != 200; i++ )
	{
		update_sound();
		gbt_update();
		wait_vbl_done();
	}
	
	while( points != 0 )
	{
		//points
		points -= 100;
		uint2bcd(points, &value);
		bcd2text(&value, 0x27, buffer);
		
		if( points < 10000 )
		{
			buffer[3] = 0;
		}
		if( points < 1000 )
		{
			buffer[4] = 0;
		}
		if( points < 100 )
		{
			buffer[5] = 0;
		}
		if( points < 10 )
		{
			buffer[6] = 0;
		}
		set_bkg_tiles(12, 10, 4, 1, &buffer[3]);
	
		//score
		value = MAKE_BCD(100);
		bcd_add(&score, &value);
		
		bcd2text(&score, 0x27, buffer);
		set_bkg_tiles(11, 13, 6, 1, &buffer[2]);
		
		set_sound( SND_BLIB );
		
		gbt_update();
		update_sound();
		wait_vbl_done();
		
		gbt_update();
		update_sound();
		wait_vbl_done();
		
		gbt_update();
		update_sound();
		wait_vbl_done();
	}
	
	while( game_state == GS_RACE_FINISHED )
	{
		UPDATE_KEYS();
		update_race_finished();
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