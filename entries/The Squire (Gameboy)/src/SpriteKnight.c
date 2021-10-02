#include "Banks/SetBank2.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"

extern UINT8 WEAPON_DAMAGE;
extern UINT16 Score;
extern UINT8 LEVEL;

extern struct ProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
};

const UINT8 walk_Knight[] = {2, 0, 1};

struct KnightInfo 
{
INT8 HP;
UINT8 COOLDOWN_COUNTER;
UINT8 VALUE;
UINT8 MOVE_COUNTER;
INT8 DIR;
INT8 VX;
INT8 MOVE;
};

void Spawn_EnemyProjectile(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE);
void Create_FX(UINT16 x, UINT16 y, UINT8 FX_TYPE);
void HUD();

void Start_SpriteKnight() 
{
// COLLISION BOX
	THIS->coll_x = 2;
	THIS->coll_y = 2;
	THIS->coll_w = 14;
	THIS->coll_h = 14;

SetSpriteAnim(THIS, walk_Knight, 4);

struct KnightInfo* data = (struct KnightInfo*)THIS->custom_data;

data -> HP = 6;
data -> VALUE = 85;
data ->	COOLDOWN_COUNTER = 0;
data -> MOVE_COUNTER = 0;
data -> DIR = 0;
data -> VX = 1;

if(LEVEL == 4)
{
	data -> MOVE = 5;
}
else
{
	data -> MOVE = 6;
	data -> HP = 8;
}	

}

void Update_SpriteKnight() 
{

struct KnightInfo* data = (struct KnightInfo*)THIS->custom_data;

//COUNTER
data->MOVE_COUNTER = data -> MOVE_COUNTER +1;
data -> COOLDOWN_COUNTER++;

//SHOOTING
		
if(data -> COOLDOWN_COUNTER == 90)
	{
	data -> COOLDOWN_COUNTER = 0;
	Spawn_EnemyProjectile(THIS->x, THIS-> y+4, 0);
	}
	
//MOVEMENT
if(data-> MOVE_COUNTER == data -> MOVE)
	{
	data->MOVE_COUNTER = 0;
	TranslateSprite(THIS, data->VX,0);
			
		if(TranslateSprite(THIS, data->VX << delta_time, 0)) 
			{
			data->VX = -data->VX;
			}
			
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
				struct KnightInfo* data = (struct KnightInfo*)THIS->custom_data;

				data -> HP = data -> HP - WEAPON_DAMAGE;
				//SetSpriteAnim(THIS, hit_Knight, 10);
				//PlayFx(CHANNEL_4, 10, 0x3A, 0xA1, 0x00, 0xC0);
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

void Destroy_SpriteKnight() 
{
	
}