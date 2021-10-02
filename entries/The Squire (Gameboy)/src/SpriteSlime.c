#include "Banks/SetBank2.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"

#include "Sound.h"

#define MOVE_THRESHOLD 4

extern UINT8 WEAPON_DAMAGE;
extern UINT16 Score;
extern UINT8 LEVEL; 

extern struct ProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
};

const UINT8 walk_slime[] = {3, 0, 1, 2};

struct SlimeInfo 
{
INT8 HP;
UINT8 VALUE;
UINT8 MOVE_COUNTER;
};

void HUD();

void Spawn_EnemyProjectile(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE);



void Start_SpriteSlime() 
{
// COLLISION BOX
	THIS->coll_x = 2;
	THIS->coll_y = 2;
	THIS->coll_w = 10;
	THIS->coll_h = 10;

SetSpriteAnim(THIS, walk_slime, 10);

struct SlimeInfo* data = (struct SlimeInfo*)THIS->custom_data;
data -> HP = 1+LEVEL;
data -> VALUE = 15;
data-> MOVE_COUNTER = 0;

if(data -> HP >=4)
{
	data -> HP = 4;
}
}

void Update_SpriteSlime() 
{
struct SlimeInfo* data = (struct SlimeInfo*)THIS->custom_data;

//MOVEMENT

		data->MOVE_COUNTER = data -> MOVE_COUNTER +1;
	
		if(data-> MOVE_COUNTER == MOVE_THRESHOLD)
			{
			data->MOVE_COUNTER = 0;
			
			THIS->y++ << delta_time;
			}
		
// COLLISION		
	UINT8 i;
	struct Sprite* spr;

SPRITEMANAGER_ITERATE(i, spr) 
	{
		if(spr->type == SpriteProjectile) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				struct SlimeInfo* data = (struct SlimeInfo*)THIS->custom_data;

				data -> HP = data -> HP - WEAPON_DAMAGE;
				SpriteManagerRemove(i);
				
			}
		}
	}

	if(data-> HP <= 0)
	{
		if(LEVEL != 1)
		{
			Spawn_EnemyProjectile(THIS->x, THIS->y-4, 1);
		}
		SpriteManagerRemove(THIS_IDX);
		Score = Score + data -> VALUE;
		SpriteManagerAdd(SpriteFire, THIS->x+4, THIS->y);
		//PlayFx(CHANNEL_4, 40, 0x0c, 0x41, 0x30, 0xc0);
		HUD();
	}

}

void Destroy_SpriteSlime() 
{
	
}