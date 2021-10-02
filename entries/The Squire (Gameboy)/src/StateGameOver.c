#include "Banks/SetBank2.h"

#include "../res/src/tiles.h"

#include "../res/src/gameover.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

extern UINT8* gameover_mod_Data[];

void Start_StateGameOver() {
	UINT8 i;

	SPRITES_8x16;
	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	
	InitScroll(&gameover, 0, 0);
	SHOW_BKG;
	HIDE_WIN;
	
	PlayMusic(gameover_mod_Data, 4, 1);
}

void Update_StateGameOver() {
	
	if(KEY_PRESSED(J_START))
	{
	//reset();
	SetState(StateIntro);
	}
}
