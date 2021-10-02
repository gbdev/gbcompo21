#include "Banks/SetBank2.h"

#include "../res/src/tiles.h"
#include "../res/src/map.h"
#include "../res/src/map2.h"
#include "../res/src/map3.h"
#include "../res/src/map4.h"
#include "../res/src/map5.h"
#include "../res/src/ending.h"
#include "../res/src/window.h"
#include "../res/src/font.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Print.h"
#include "Sprite.h"
#include "Sound.h"
#include "gbt_player.h"



#define SLIME	0
#define WHEEL	1



UINT8 CONTROLS_OFF;

UINT8 collision_tiles[] = {4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 20, 21, 22, 23, 
							72, 73, 74, 75, 76, 77, 78, 79, 80, 82, 84, 85, 
							86, 87, 68, 69, 70, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
							110, 111, 112, 117, 118, 119, 114, 115, 116, 120,0};

UINT8 collision_dungeon[] = {4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19, 20, 21, 22, 23, 
							72, 73, 74, 75, 76, 77, 78, 79, 80, 82, 84, 85, 
							86, 87, 68, 69, 70, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
							110, 111, 112, 117, 118, 119, 114, 115, 116, 120,
							127, 128,129, 130, 131, 132, 134, 135, 137, 139, 140, 141, 142,143,0};


UINT16 map_w;
UINT16 map_h; 


extern UINT8* win_mod_Data[];
extern UINT8* end_mod_Data[];
extern UINT8* gameover_mod_Data[];
extern UINT8* under_mod_Data[];
extern UINT8* overworld_mod_Data[];



//extern UINT8* joy_mod_Data[];

extern UINT16 Score;
INT16 movement;

extern INT16 BONUS_LIFE[];

extern INT8 LIFE;
extern UINT8 LEVEL; 
extern UINT16 CAM_POS;

//UINT8 ControlsOff = 0;

void Create_Enemy(UINT16 x, UINT16 y, UINT8 TYPE);
void Create_Weapon(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE);

extern UINT8* win_mod_Data[];
extern UINT8* death_mod_Data[];


void HUD()
{
	//BONUS LIFE
	
	if(Score >= 1000 && BONUS_LIFE[0] == 0)
	{
		BONUS_LIFE[0] = 1;
		LIFE++;
	}
	if(Score >= 2000 && BONUS_LIFE[1] == 0)
	{
		BONUS_LIFE[1] = 1;
		LIFE++;
	}
	if(Score >= 3000 && BONUS_LIFE[2] == 0)
	{
		BONUS_LIFE[2] = 1;
		LIFE++;
	}
	if(Score >= 4000 && BONUS_LIFE[3] == 0)
	{
		BONUS_LIFE[3] = 1;
		LIFE++;
	}
	if(Score >= 5000 && BONUS_LIFE[4] == 0)
	{
		BONUS_LIFE[4] = 1;
		LIFE++;
	}
	if(Score >= 6000 && BONUS_LIFE[5] == 0)
	{
		BONUS_LIFE[5] = 1;
		LIFE++;
	}
	if(Score >= 7000 && BONUS_LIFE[6] == 0)
	{
		BONUS_LIFE[6] = 1;
		LIFE++;
	}
	if(Score >= 8000 && BONUS_LIFE[7] == 0)
	{
		BONUS_LIFE[7] = 1;
		LIFE++;
	}
	if(Score >= 9000 && BONUS_LIFE[8] == 0)
	{
		BONUS_LIFE[8] = 1;
		LIFE++;
	}
	if(Score >= 10000 && BONUS_LIFE[9] == 0)
	{
		BONUS_LIFE[9] = 1;
		LIFE++;
	}
	
	if(Score < 10)
	{
	PRINT_POS(0, 0);
	Printf("SCORE 0000%d", Score);
	}	
	
	if(Score >= 10 && Score < 100 )
	{
	PRINT_POS(0, 0);
	Printf("SCORE 000%d", Score);
	}	
	
	if(Score >= 100 && Score < 1000 )
	{
		
	PRINT_POS(0, 0);
	Printf("SCORE 00%d", Score);
	}	
	
	if(Score >= 1000 && Score < 10000)
	{
	PRINT_POS(0, 0);
	Printf("SCORE 0%d", Score);
	}
	
	if(Score >= 10000)
	{
	PRINT_POS(0, 0);
	Printf("SCORE %d", Score);
	}
	
	if(LIFE < 10)
	{
	PRINT_POS(18, 0);
	Printf("0%d", LIFE);
	}
	
	if(LIFE >= 10)
	{
	PRINT_POS(18, 0);
	Printf("%d", LIFE);
	}
	
}

void Start_StateGame() 
{
 CONTROLS_OFF = 0;


const struct MapInfo* levels[] = {
	&map,
	&map2,
	&map3,
	&map4,
	&map5,
	&ending
	};

	UINT8 i;
	
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	

	
	SPRITES_8x16;
	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	
		//SpriteManagerAdd(SpriteOrb, 50, 1200);
		//SpriteManagerAdd(SpriteChest, 50, 1240);
	//SpriteManagerAdd(SpriteSlime, 80, 80);
	
	//SpriteManagerAdd(SpriteWeapon, 80, 80);
	//Create_Weapon(32, 96, 2);
	//Create_Weapon(32, 96, 0);
	
	//CAM START LEVEL
	switch(LEVEL)
	{
		case 1: scroll_target = SpriteManagerAdd(SpriteCam, -16, 1280); break;
		case 2: scroll_target = SpriteManagerAdd(SpriteCam, -16, 960); break;
		case 3: scroll_target = SpriteManagerAdd(SpriteCam, -16, 960); break;
		case 4: scroll_target = SpriteManagerAdd(SpriteCam, -16, 960); break;
		case 5: scroll_target = SpriteManagerAdd(SpriteCam, -16, 1280); break;
		//case 6: scroll_target = SpriteManagerAdd(SpriteCam, -16, 1280); break;
		
	}
	
	
		
	
	
//InitScroll(&map, collision_tiles, 0);

switch(LEVEL)
{
	case 1: 
	InitScroll(&map, collision_tiles, 0); 
	PlayMusic(overworld_mod_Data, 5, 1);
	break;
	case 2: 
	InitScroll(&map2, collision_tiles, 0);
	PlayMusic(overworld_mod_Data, 5, 1);	
	break;
	case 3: 
	InitScroll(&map3, collision_tiles, 0); 
	PlayMusic(overworld_mod_Data, 5, 1);
	break;
	case 4: 
	InitScroll(&map4, collision_dungeon, 0); 
	PlayMusic(under_mod_Data, 6, 1);
	break;
	case 5: 
	InitScroll(&map5, collision_dungeon, 0);
	PlayMusic(under_mod_Data, 6, 1);	
	break;
}

	
	INIT_FONT(font, PRINT_WIN);
	WX_REG = 7;
	WY_REG = 136;
	
	InitWindow(0, 0, &window);

	SHOW_BKG;
	SHOW_WIN;


HUD();

	
	
	//GetMapSize(&map, &map_w, &map_h);
	//ScrollFindTile(&map, 1, 0, 0, map_w, map_h, &start_x, &start_y);
	
	//
}

void Update_StateGame() 
{
	
	/*
	if(KEY_TICKED(J_START))
	{
		reset();
		
	}		
	*/
}
