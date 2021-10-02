#include "global.h"

const unsigned char start_sound[] =
{
	SET,0x80,0xF2,0x50,0x87,
	SET,0x80,0xF2,0x80,0x87,
	SET,0x80,0xF2,0xB0,0x87,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

const unsigned char speed_sound[] =
{
	SET,0x40,0x70,0xFF,0xFF,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

/*
const unsigned char boost_sound[] =
{
	SET,0x80,0xF2,0x50,0x87,
	SET,0x80,0xF2,0x70,0x87,
	IDLE,
	SET,0x80,0xF2,0x50,0x87,
	SET,0x80,0xF2,0x90,0x87,
	IDLE,
	SET,0x80,0xF2,0x50,0x87,
	SET,0x80,0xF2,0xB0,0x87,
	IDLE,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

const unsigned char boost_sound[] =
{
	SET,0x80,0xF1,0xA4,0x86,
	SET,0x80,0xF1,0x04,0x87,
	SET,0x80,0xE1,0xC4,0x86,
	SET,0x80,0xE1,0xE4,0x86,
	SET,0x80,0xD1,0x44,0x87,
	SET,0x80,0xD1,0x04,0x87,
	SET,0x80,0xC1,0x64,0x87,
	SET,0x80,0xC1,0x24,0x87,
	SET,0x00,0x00,0x00,0x00,
	STOP
};
*/

const unsigned char pause_sound[] =
{
	SET,0x80,0xF2,0x7C,0x86,
	SET,0x80,0xF2,0xCC,0x86,
	SET,0x80,0xF2,0x1C,0x87,
	SET,0x80,0xF2,0x7C,0x86,
	SET,0x80,0xF2,0xCC,0x86,
	SET,0x80,0xF2,0x1C,0x87,
	SET,0x80,0xF2,0x7C,0x86,
	SET,0x80,0xF2,0xCC,0x86,
	SET,0x80,0xF2,0x1C,0x87,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

const unsigned char beep1_sound[] =
{
	SET,0x80,0xF2,0x06,0x87,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

const unsigned char beep2_sound[] =
{
	SET,0x80,0xF2,0x83,0x87,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	IDLE,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

const unsigned char blib_sound[] =
{
	SET,0x80,0xF2,0x50,0x87,
	SET,0x80,0xF2,0x80,0x87,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

const unsigned char hit_sound[] =
{
	SET,0xFF,0xF2,0x37,0x80,		
	SET,0xFF,0xF2,0x3B,0x80,
	SET,0xFF,0xF2,0x45,0x80,
	SET,0xFF,0xF2,0x3C,0x80,
	SET,0xFF,0xF2,0x57,0x80,
	SET,0x00,0x00,0x00,0x00,
	STOP
};

UBYTE sound_prio_ch2;
const unsigned char *sound_ptr_ch2;

UBYTE sound_prio_ch4;
const unsigned char *sound_ptr_ch4;

void init_sound()
{
	sound_prio_ch2 = 0;
	sound_ptr_ch2 = NULL;
	
	sound_prio_ch4 = 0;
	sound_ptr_ch4 = NULL;
}

void set_sound( UBYTE sound )
{
	switch( sound )
	{
		case SND_START:
			if( (sound_ptr_ch2 == NULL) || (sound_prio_ch2 <= 2) )
			{
				gbt_mute_channel2(1);
				sound_prio_ch2 = 2;
				sound_ptr_ch2 = start_sound;
			}
			break;
		case SND_SPEED:
			if( (sound_ptr_ch2 == NULL) || (sound_prio_ch2 <= 1) )
			{
				gbt_mute_channel2(1);
				sound_prio_ch2 = 1;
				sound_ptr_ch2 = speed_sound;
			}
			break;
		case SND_PAUSE:
			if( (sound_ptr_ch2 == NULL) || (sound_prio_ch2 <= 2) )
			{
				gbt_mute_channel2(1);
				sound_prio_ch2 = 2;
				sound_ptr_ch2 = pause_sound;
			}
			break;
		case SND_BEEP1:
			if( (sound_ptr_ch2 == NULL) || (sound_prio_ch2 <= 2) )
			{
				gbt_mute_channel2(1);
				sound_prio_ch2 = 2;
				sound_ptr_ch2 = beep1_sound;
			}
			break;
		case SND_BEEP2:
			if( (sound_ptr_ch2 == NULL) || (sound_prio_ch2 <= 2) )
			{
				gbt_mute_channel2(1);
				sound_prio_ch2 = 2;
				sound_ptr_ch2 = beep2_sound;
			}
			break;
		case SND_BLIB:
			if( (sound_ptr_ch2 == NULL) || (sound_prio_ch2 <= 2) )
			{
				gbt_mute_channel2(1);
				sound_prio_ch2 = 2;
				sound_ptr_ch2 = blib_sound;
			}
			break;
		case SND_HIT:
			if( (sound_ptr_ch4 == NULL) || (sound_prio_ch4 <= 2) )
			{
				gbt_mute_channel4(1);
				sound_prio_ch4 = 2;
				sound_ptr_ch4 = hit_sound;
			}
			break;
	}
}

void update_sound()
{
	UWORD frequency;
	
	if( sound_ptr_ch2 != NULL )
	{
		switch( *sound_ptr_ch2 )
		{
			case SET:
				sound_ptr_ch2++;
				NR21_REG = *sound_ptr_ch2++;
				NR22_REG = *sound_ptr_ch2++;
				if( *sound_ptr_ch2 == 0xFF )
				{
					frequency = (player_speed) + 1155;
					NR23_REG = frequency & 0x00FF;
					NR24_REG = 0x80 | ((frequency >> 8) & 0x0007);
					
					sound_ptr_ch2 += 2;
				} else {
					NR23_REG = *sound_ptr_ch2++;
					NR24_REG = *sound_ptr_ch2++;
				}
				break;
			case IDLE:
				sound_ptr_ch2++;
				break;
			case STOP:
				gbt_mute_channel2(0);
				sound_ptr_ch2 = NULL;
				break;
		}
	}
	
	if( sound_ptr_ch4 != NULL )
	{
		switch( *sound_ptr_ch4 )
		{
			case SET:
				sound_ptr_ch4++;
				NR41_REG = *sound_ptr_ch4++;
				NR42_REG = *sound_ptr_ch4++;
				NR43_REG = *sound_ptr_ch4++;
				NR44_REG = *sound_ptr_ch4++;
				break;
			case IDLE:
				sound_ptr_ch4++;
				break;
			case STOP:
				gbt_mute_channel4(0);
				sound_ptr_ch4 = NULL;
				break;
			
		}
	}
}

void stop_sound()
{
	sound_ptr_ch2 = NULL;
	sound_ptr_ch4 = NULL;
	
	NR21_REG = 0;
	NR22_REG = 0;
	NR23_REG = 0;
	NR24_REG = 0;
	
	NR41_REG = 0;
	NR42_REG = 0;
	NR43_REG = 0;
	NR44_REG = 0;
}