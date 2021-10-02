#include "Banks/SetBank2.h"
#include "ZGBMain.h"

#include "Sound.h"
#include "Keys.h"
#include "SpriteManager.h"

#define SWORD 	0
#define ARROW 	1
#define AXE		2

extern UINT8 projectile_count;
extern UINT8 projetile_max;

UINT8 WEAPON_DAMAGE;

UINT8 tile_collision;


extern struct EnemyInfo 
{
UINT8 TYPE;
INT8 HP;
UINT8 COOLDOWN;
UINT8 COOLDOWN_COUNTER;
UINT8 VALUE;
};


const UINT8 sword_anim[] = {1, 0};
const UINT8 arrow_anim[] = {1, 1};
const UINT8 axe_anim[] = {4, 2, 3, 4, 5};

struct ProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
INT8 vy;
};




void Spawn_Projectile(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE)

{
	struct Sprite* projectile_sprite = SpriteManagerAdd(SpriteProjectile, x, y);
	struct ProjectileInfo* data = (struct ProjectileInfo*)projectile_sprite->custom_data;
	
	data -> WEAPON_TYPE = WEAPON_TYPE;
	
}

void Start_SpriteProjectile() 
{

	THIS->coll_x = 4;
	THIS->coll_y = 4;
	THIS->coll_w = 8;
	THIS->coll_h = 12;
	

}

void Update_SpriteProjectile() 
{
	
	UINT8 weapon_damage;
	
	//settings of players projectiles.
	
	struct ProjectileInfo* data = (struct ProjectileInfo*)THIS->custom_data;

	switch(data->WEAPON_TYPE)
	{
		case SWORD: 
		projetile_max = 1;
		data -> vy = 2;
		TranslateSprite(THIS, 0, -data->vy  << delta_time);
		
		SetSpriteAnim(THIS, sword_anim, 10);
		WEAPON_DAMAGE = 2;
		break;
		
		case ARROW: 
		projetile_max = 1;
		data -> vy = 3;
		TranslateSprite(THIS, 0, -data-> vy << delta_time);
		SetSpriteAnim(THIS, arrow_anim, 10);
		WEAPON_DAMAGE = 2;
		break;
		
		case AXE: 
		projetile_max = 1;
		data -> vy = 1;
		TranslateSprite(THIS, 0, -data-> vy << delta_time);
		SetSpriteAnim(THIS, axe_anim, 20);
		WEAPON_DAMAGE = 3;
		break;
	}
	
	weapon_damage = data -> DAMAGE;
	
	UINT8 j;
	if(TranslateSprite(THIS, 0, -data->vy << delta_time))
	{
		tile_collision = TranslateSprite(THIS, 0, -data->vy << delta_time);
		if(tile_collision > 99 && tile_collision < 128 )
		{
			for(j=0; j<data->vy; j++)
			{
				THIS->y--;
			}
		}
		else
		{
			
			SpriteManagerRemove(THIS_IDX);
		}	
		
		
		//SpriteManagerAdd(SpriteHit, THIS->x, THIS->y);
		
	}
	
	

	
	
	}





void Destroy_SpriteProjectile() {
	
	
	SpriteManagerAdd(SpriteHit, THIS->x+4, THIS->y);
	projectile_count--;
}