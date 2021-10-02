#include "Banks/SetBank2.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"





const UINT8 walk_trap[] = {4, 0, 1, 2, 3};



struct TrapInfo 
{
INT8 VX;
};

void Start_SpriteTrap() 
{
	THIS->coll_x = 1;
	THIS->coll_y = 1;
	THIS->coll_w = 15;
	THIS->coll_h = 15;
	
	
	struct TrapInfo* data = (struct TrapInfo*)THIS->custom_data;
	data-> VX = 1;
	SetSpriteAnim(THIS, walk_trap, 12);

}

void Update_SpriteTrap() 
{
	struct TrapInfo* data = (struct TrapInfo*)THIS->custom_data;
	if(TranslateSprite(THIS, data->VX, 0)) 
	{
		data->VX = -data->VX;
	}
}



void Destroy_SpriteTrap() 
{
	
}