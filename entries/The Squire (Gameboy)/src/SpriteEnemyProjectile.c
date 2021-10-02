#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Keys.h"
#include "SpriteManager.h"

#define ENEMY_ARROW 0
#define ENEMY_PROJECTILE 1

const UINT8 frame_arrow[] = {1, 0};
const UINT8 frame_projectile[] = {1, 1};

struct EnemyProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
};


void Spawn_EnemyProjectile(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE)

{
	struct Sprite* EnemyProjectile_sprite = SpriteManagerAdd(SpriteEnemyProjectile, x+4, y+8);
	struct EnemyProjectileInfo* data = (struct EnemyProjectileInfo*)EnemyProjectile_sprite->custom_data;
	
	data -> WEAPON_TYPE = WEAPON_TYPE;
	
}


void Start_SpriteEnemyProjectile() 
{

	THIS->coll_x = 2;
	THIS->coll_y = 0;
	THIS->coll_w = 2;
	THIS->coll_h = 12;
	

}

void Update_SpriteEnemyProjectile() 
{
	
	struct EnemyProjectileInfo* data = (struct EnemyProjectileInfo*)THIS->custom_data;
	
	if(data-> WEAPON_TYPE == 0)
	{
		THIS->y++;
		THIS->y++;
		SetSpriteAnim(THIS, frame_arrow, 0);
	}
	
	if(data-> WEAPON_TYPE == ENEMY_PROJECTILE)
	{
		THIS->y++;
		//THIS->y++;
		SetSpriteAnim(THIS, frame_projectile, 0);
	}
	
	
	
	
	/*
	UINT8 weapon_damage;
	
	//settings of players EnemyProjectiles.
	
	struct EnemyProjectileInfo* data = (struct EnemyProjectileInfo*)THIS->custom_data;

	switch(data->WEAPON_TYPE)
	{
		case SWORD: 
		projetile_max = 1;
		TranslateSprite(THIS, 0, -3 << delta_time);
		SetSpriteAnim(THIS, sword_anim, 10);
		WEAPON_DAMAGE = 1;
		break;
		
		case AXE: 
		projetile_max = 1;
		TranslateSprite(THIS, 0, -2 << delta_time);
		SetSpriteAnim(THIS, arrow_anim, 10);
		WEAPON_DAMAGE = 2;
		break;
	}
	
	weapon_damage = data -> DAMAGE;
	
	
	UINT8 i;
	struct Sprite* spr;
	

	SPRITEMANAGER_ITERATE(i, spr) 
	{
		if(spr->type == SpritePlayer) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				SpriteManagerRemove(THIS_IDX);
			}
		}
		
	}
	*/
	if(TranslateSprite(THIS, 0, 1 << delta_time))
	{
		
	SpriteManagerRemove(THIS_IDX);
		}	
	
	
	
	
	}
	





void Destroy_SpriteEnemyProjectile() {
	
	SpriteManagerAdd(SpriteHit, THIS->x, THIS->y);
}