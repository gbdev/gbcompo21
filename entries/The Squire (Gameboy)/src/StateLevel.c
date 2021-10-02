#include "Banks/SetBank2.h"

#include "../res/src/tiles.h"
#include "../res/src/level.h"
#include "../res/src/window.h"
#include "../res/src/font.h"


#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Print.h"

extern UINT16 Score;
extern INT8 LIFE;


UINT8 Level_countdown;

void HUD();

void Start_StateLevel() {
	UINT8 i;
	Level_countdown = 0;
	LIFE--;
	
	

	SPRITES_8x8;
	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}
	SpriteManagerAdd(SpriteNumber, 96, 32);
	SHOW_SPRITES;

	
	InitScroll(&level, 0, 0);
	SHOW_BKG;
	
	INIT_FONT(font, PRINT_WIN);
	WX_REG = 7;
	WY_REG = 136;
	
	InitWindow(0, 0, &window);

	SHOW_BKG;
	SHOW_WIN;

	PRINT_POS(0, 0);
	Printf("SCORE 0000", Score);
	PRINT_POS(18, 0);
	Printf("0%d", LIFE);
	
	HUD();
	
	//HIDE_WIN;
}

void Update_StateLevel() {
	
	Level_countdown++;
	
	if(Level_countdown == 120)
	{
		SetState(StateGame);
	}
	if(KEY_PRESSED(J_START))
	{
	//reset();
	SetState(StateGame);
	}
	
}
