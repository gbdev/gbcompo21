#include "Banks/SetBank2.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"

#define MOVE_THRESHOLD 4
#define MOVE_LEFT_OR_RIGHT 3
#define DIR_SPEED 1


extern UINT8 WEAPON_DAMAGE;
extern UINT16 Score;
extern UINT8 LEVEL; 

extern struct ProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
};

const UINT8 walk_Skull[] = {5, 0, 1,2,2,1};

struct SkullInfo 
{
INT8 HP;
UINT8 VALUE;
UINT8 SPECIAL_COUNTER;
UINT8 MOVE_COUNTER_DOWN;
UINT8 MOVE_COUNTER_LEFT_RIGHT;
UINT8 DIR;
UINT8 SPEED;

};

void HUD();


/*
void Create_Skull(UINT16 x, UINT16 y, UINT8 TYPE)
{
	struct Sprite* Skull_sprite = SpriteManagerAdd(SpriteSkull, x, y);
	struct SkullInfo* data = (struct SkullInfo*)Skull_sprite->custom_data;
	
	data -> TYPE = TYPE;
	
	switch(data-> TYPE)
	{
		case Skull:
		SetSpriteAnim(Skull_sprite, walk_Skull, 12);
		break;
		
		case WHEEL:
		SetSpriteAnim(Skull_sprite, walk_wheel, 12);
		break;
	
	}
	
}
*/
void Start_SpriteSkull() 
{
// COLLISION BOX
	THIS->coll_x = 2;
	THIS->coll_y = 2;
	THIS->coll_w = 14;
	THIS->coll_h = 14;

SetSpriteAnim(THIS, walk_Skull, 8);

if(LEVEL != 1)
{
SetSpriteAnim(THIS, walk_Skull, 10);
}

struct SkullInfo* data = (struct SkullInfo*)THIS->custom_data;
data -> HP = 3;
data -> VALUE = 25;
data -> SPEED = 5;
data->MOVE_COUNTER_LEFT_RIGHT = 0;
data->MOVE_COUNTER_DOWN = 0;
data-> SPECIAL_COUNTER = 0;


if(THIS -> x > 80)
{
	data-> DIR = 0;
}
else
{
	data-> DIR = 1;
}


switch(LEVEL)
{
	case 1: data -> SPEED = 5;
	case 2: data -> SPEED = 3;
}

}
void Update_SpriteSkull() 
{
struct SkullInfo* data = (struct SkullInfo*)THIS->custom_data;

//COUNTERS
		data->MOVE_COUNTER_LEFT_RIGHT = data -> MOVE_COUNTER_LEFT_RIGHT +1;
		data->MOVE_COUNTER_DOWN = data -> MOVE_COUNTER_DOWN +1;
		data->SPECIAL_COUNTER = data -> SPECIAL_COUNTER +1;
		
			
			//MOVEMENT
		
			
		if(data-> MOVE_COUNTER_DOWN == data-> SPEED)
			{
			data->MOVE_COUNTER_DOWN = 0;
			
			THIS->y++; 
			}
		
if(LEVEL == 1)
{	
		if(data-> MOVE_COUNTER_LEFT_RIGHT == 2)
			{
			data->MOVE_COUNTER_LEFT_RIGHT = 0;
			
				if(data -> DIR == 1)
				{
				THIS->x++; 
				}
				else
				{
				THIS->x--; 
				}
			}
}
else
{
	if(data-> MOVE_COUNTER_LEFT_RIGHT == 2)
			{
			data->MOVE_COUNTER_LEFT_RIGHT = 0;
			
				if(data -> DIR == 1)
				{
				THIS->x++; 
				THIS->x++; 
				}
				else
				{
				THIS->x--;
				THIS->x--;				
				}
			}
}
			
			//SWITCH MOVEMENT
			
			if(data->SPECIAL_COUNTER == 60)
			{
				data -> SPECIAL_COUNTER = 0;
				switch(data -> DIR)
				{
					case 0: data -> DIR = 1; break;
					case 1: data -> DIR = 0; break;
				}
			}
			
		/*	
			//bounce on left side, switch to right movement
		if(THIS->x == 0)
		{
			data-> DIR = 0;
		}
		//bounce on right side, switch to left movement
		if(THIS->x == 144)
		{
			data-> DIR = 1;
		}
		
		


			if(data -> vx < 2)
			{
			data -> vx = data -> vx - 1;
			}
			
	THIS->x = THIS-> x + (data -> vx);			
			}
			
		if(data-> MOVE_COUNTER_DOWN == MOVE_THRESHOLD)
			{
			data->MOVE_COUNTER_DOWN = 0;
			//TranslateSprite(THIS, 0, 2 << delta_time);
			//THIS->y = THIS-> y + data -> vy;
			THIS->y++; 
			THIS->y++;
			
			}
		
		data-> MOVE_COUNTER_LEFT_RIGHT = data-> MOVE_COUNTER_LEFT_RIGHT +1;
		
		if(data-> MOVE_COUNTER_LEFT_RIGHT == MOVE_LEFT_OR_RIGHT)
			{
			data->MOVE_COUNTER_LEFT_RIGHT = 0;
			TranslateSprite(THIS, data -> vx << delta_time,0 );
			}
		*/
// COLLISION		
	UINT8 i;
	struct Sprite* spr;

SPRITEMANAGER_ITERATE(i, spr) 
	{
		if(spr->type == SpriteProjectile) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				struct SkullInfo* data = (struct SkullInfo*)THIS->custom_data;

				data -> HP = data -> HP - WEAPON_DAMAGE;
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

void Destroy_SpriteSkull() 
{
	
}