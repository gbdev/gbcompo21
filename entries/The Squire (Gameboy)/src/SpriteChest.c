#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Keys.h"
#include "SpriteManager.h"

struct ChestInfo 
{
INT8 CHEST_HP;

};



void Start_SpriteChest() 
{

	THIS->coll_x = 0;
	THIS->coll_y = 0;
	THIS->coll_w = 16;
	THIS->coll_h = 16;
struct ChestInfo* data = (struct ChestInfo*)THIS->custom_data;

data -> CHEST_HP = 2;
}

void Update_SpriteChest() 
{

struct ChestInfo* data = (struct ChestInfo*)THIS->custom_data;


UINT8 i;
struct Sprite* spr;

SPRITEMANAGER_ITERATE(i, spr) 
	{
		if(spr->type == SpriteProjectile) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				data -> CHEST_HP = data -> CHEST_HP - 1;
				SpriteManagerRemove(i);
				
			}
		}
	}

if(data-> CHEST_HP <=0)
{
	SpriteManagerRemove(THIS_IDX);
	SpriteManagerAdd(SpriteRupees, THIS-> x +4, THIS->y);
}

}


	

	
void Destroy_SpriteChest() 

{

}