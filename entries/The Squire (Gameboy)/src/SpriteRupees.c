#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Keys.h"
#include "SpriteManager.h"

#define Rupee_Value 150

extern UINT16 Score;

//const UINT8 shine_anim[] = {2, 0, 1};

struct RupeeInfo 
{
INT8 fade_counter;
};

void HUD();

void Start_SpriteRupees() 
{

	THIS->coll_x = 0;
	THIS->coll_y = 0;
	THIS->coll_w = 16;
	THIS->coll_h = 16;
	//SetSpriteAnim(THIS, shine_anim, 16);
	struct RupeeInfo* data = (struct RupeeInfo*)THIS->custom_data;
	data-> fade_counter = 0;
}

void Update_SpriteRupees() 
{
struct RupeeInfo* data = (struct RupeeInfo*)THIS->custom_data;
data-> fade_counter = data-> fade_counter+1;

THIS -> y--;

if(data->fade_counter == 20)
{
	SpriteManagerRemove(THIS_IDX);
	Score = Score + Rupee_Value;
	HUD();
}
UINT8 i;
	struct Sprite* spr;
	

	SPRITEMANAGER_ITERATE(i, spr) 
	{
		if(spr->type == SpritePlayer) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				SpriteManagerRemove(THIS_IDX);
				Score = Score + Rupee_Value;
				HUD();
				
			}
		}
	}


}


	


void Destroy_SpriteRupees() {
}