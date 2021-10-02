#include "Banks/SetBank2.h"

#include "../res/src/tiles.h"

#include "../res/src/intro.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Sound.h"
#include "gbt_player.h"

#define SWORD 	0

INT8 LIFE;
UINT8 LEVEL; 
INT16 BONUS_LIFE[10];
UINT16 Score;
UINT8 WEAPON;


void Start_StateIntro() 
{
	
	LIFE = 5;
	LEVEL = 1;
	UINT8 i;
	Score = 0;
	
	BONUS_LIFE[0] = 0;
	BONUS_LIFE[1] = 0;
	BONUS_LIFE[2] = 0;
	BONUS_LIFE[3] = 0;
	BONUS_LIFE[4] = 0;
	BONUS_LIFE[5] = 0;
	BONUS_LIFE[6] = 0;
	BONUS_LIFE[7] = 0;
	BONUS_LIFE[8] = 0;
	BONUS_LIFE[9] = 0;
	
	
	WEAPON = SWORD;
	

	SPRITES_8x16;
	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	
	InitScroll(&intro, 0, 0);
	SHOW_BKG;
	HIDE_WIN;
	
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
}

void Update_StateIntro() {
	
	if(KEY_PRESSED(J_START))
	{
	
	SetState(StateLevel);
	//SetState(StateEnd);
	}
}
