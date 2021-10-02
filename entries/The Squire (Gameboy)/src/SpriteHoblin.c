#include "Banks/SetBank2.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"

#define MOVE_THRESHOLD 10



extern UINT8 WEAPON_DAMAGE;
extern UINT16 Score;
extern UINT8 LEVEL; 

extern struct ProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
};

const UINT8 walk_Hoblin[] = {2, 0, 1};


struct HoblinInfo 
{
UINT8 TYPE;
INT8 HP;
UINT8 COOLDOWN_COUNTER;
UINT8 VALUE;
UINT8 MOVE_COUNTER_DOWN;
UINT8 MOVE_COUNTER_LEFT_RIGHT;

UINT8 DIR;
};


void Spawn_EnemyProjectile(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE);
void Create_FX(UINT16 x, UINT16 y, UINT8 FX_TYPE);
void HUD();

/*
void Create_Knight(UINT16 x, UINT16 y, UINT8 TYPE)
{
	struct Sprite* Knight_sprite = SpriteManagerAdd(SpriteHoblin, x, y);
	struct HoblinInfo* data = (struct HoblinInfo*)Knight_sprite->custom_data;
	
	data -> TYPE = TYPE;
	
	switch(data-> TYPE)
	{
		case Knight:
		SetSpriteAnim(Knight_sprite, walk_Knight, 12);
		break;
		
		case WHEEL:
		SetSpriteAnim(Knight_sprite, walk_wheel, 12);
		break;
	
	}
	
}
*/
void Start_SpriteHoblin() 
{
// COLLISION BOX
	THIS->coll_x = 2;
	THIS->coll_y = 2;
	THIS->coll_w = 12;
	THIS->coll_h = 12;
	
SetSpriteAnim(THIS, walk_Hoblin, 6);

struct HoblinInfo* data = (struct HoblinInfo*)THIS->custom_data;
data -> HP = 3+LEVEL;
data -> VALUE = 75;
data-> COOLDOWN_COUNTER = 0;
data-> MOVE_COUNTER_DOWN = 0;

if(data -> HP >=6)
{
	data -> HP = 6;
}

if(THIS -> x > 80)
{
	data-> DIR = 0;
}
else
{
	data-> DIR = 1;
}

}

void Update_SpriteHoblin() 
{
struct HoblinInfo* data = (struct HoblinInfo*)THIS->custom_data;

//MOVEMENT


		//SHOOTING
		data -> COOLDOWN_COUNTER++;
		
		if(data -> COOLDOWN_COUNTER == 120)
			
		{data -> COOLDOWN_COUNTER = 0;
		
		Spawn_EnemyProjectile(THIS->x, THIS-> y+4, 0);
		
		}
	
		data->MOVE_COUNTER_DOWN = data -> MOVE_COUNTER_DOWN +1;
	
		if(data-> MOVE_COUNTER_DOWN == 6)
			{
			data->MOVE_COUNTER_DOWN = 0;
			
			//TranslateSprite(THIS, 0, 1 << delta_time);
			THIS -> y++;
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
				struct HoblinInfo* data = (struct HoblinInfo*)THIS->custom_data;

				data -> HP = data -> HP - WEAPON_DAMAGE;
				//SetSpriteAnim(THIS, hit_Knight, 10);
				SpriteManagerRemove(i);
				
			}
		}
	}

	if(data-> HP <= 0)
	{
		SpriteManagerRemove(THIS_IDX);
		Score = Score + data -> VALUE;
		SpriteManagerAdd(SpriteFire, THIS->x+4, THIS->y);
		HUD();
	}

}

void Destroy_SpriteHoblin() 
{
	
}