#include "Banks/SetBank2.h"

#include "../res/src/tiles.h"

#include "../res/src/ending.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "../res/src/font.h"


#include "Print.h"
#include "Sprite.h"
#include "Sound.h"
#include "gbt_player.h"



extern UINT16 Score;

extern UINT8* end_mod_Data[];

void Start_StateEnd() {
	UINT8 i;

	SPRITES_8x16;
	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	
	InitScroll(&ending, 0, 0);
	
	INIT_FONT(font, PRINT_BKG);
	
	
	SHOW_BKG;
	HIDE_WIN;
	
	PRINT_POS(2, 0);
	Printf("CONGRATULATIONS");
	PRINT_POS(2, 2);
	Printf("YOUR SCORE %d", Score);
	
	PRINT_POS(6, 3);
	Printf("THANK YOU");
	
	PRINT_POS(1, 5);
	Printf("CREDITS");
	
	PRINT_POS(1, 7);
	Printf("ART BY");
	
	PRINT_POS(1, 8);
	Printf("DRAGONDEPALATINO");
	
	PRINT_POS(1, 9);
	Printf("MUSIC BY");
	
	PRINT_POS(1, 10);
	Printf("GB COMMUNITY");
	
	PRINT_POS(1, 11);
	Printf("ZALO FOR ZGB");
	
	PRINT_POS(1, 13);
	Printf("GB COMP 21");
	
	PRINT_POS(1, 15);
	Printf("MADE BY AMADEUS B.");
	
	PlayMusic(end_mod_Data, 4, 1);
	
}

void Update_StateEnd() {
	
	if(KEY_PRESSED(J_START))
	{

	SetState(StateIntro);
	}
}
