#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Keys.h"
#include "SpriteManager.h"
#include "Sprite.h"

#define SWORD 0
#define AXE	1
#define ARROW 2

#define ANIMATION_ORB 20

const UINT8 orb_sword[] = {2, 0, 3};
const UINT8 orb_axe[] = {2, 1, 3};
const UINT8 orb_arrow[] = {2, 2, 3};

extern UINT16 Score;

extern UINT8 WEAPON;

 
struct OrbInfo 
{
UINT8 UPGRADE_TYPE;
UINT8 up_counter;
INT8 dir;
};
	
void HUD();

// dir 1 = right, dir 2 = left
void Spawn_Orb(UINT16 x, UINT16 y, UINT8 UPGRADE_TYPE)

{
	struct Sprite* Orb_sprite = SpriteManagerAdd(SpriteOrb, x, y);
	struct OrbInfo* data = (struct OrbInfo*)Orb_sprite->custom_data;
	
	data -> UPGRADE_TYPE = UPGRADE_TYPE;
	
}

void Start_SpriteOrb() 
{

THIS->coll_x = 1;
	THIS->coll_y = 1;
	THIS->coll_w = 14;
	THIS->coll_h = 14;

SetSpriteAnim(THIS, orb_sword, ANIMATION_ORB);


struct OrbInfo* data = (struct OrbInfo*)THIS->custom_data;

data -> up_counter = 0;
data -> UPGRADE_TYPE= SWORD;

/*
if(THIS-> x > 80)
{
	data-> dir = 1;
}
else
{
	data-> dir = 2;
}

*/
}


void Update_SpriteOrb() 
{
/*	


if(data-> dir == 1)	
{
	THIS-> x++;
}
else
{
	THIS-> x--;
}

if(THIS-> x == 144)
{
	data-> dir = 2;
	
}

if(THIS-> x == 0)
{
	data-> dir = 1;
	
}
	

	
data -> up_counter = data -> up_counter +1 ;

if(data -> up_counter == 20)
{
	data -> up_counter = 0;
	THIS-> y--;
}
*/
struct OrbInfo* data = (struct OrbInfo*)THIS->custom_data;

data -> up_counter = data -> up_counter +1 ;
if(data -> up_counter == 3)
{
	data -> up_counter = 0;
	THIS-> y++;
}


UINT8 i;
struct Sprite* spr;




SPRITEMANAGER_ITERATE(i, spr) 
	{
		struct OrbInfo* data = (struct OrbInfo*)THIS->custom_data;
		if(spr->type == SpritePlayer) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				WEAPON = data -> UPGRADE_TYPE;
				Score = Score + 50;	
				HUD();
				SpriteManagerRemove(THIS_IDX);
			}
		}
		
		
		
		
		
		if(spr->type == SpriteProjectile) 
		{
			if(CheckCollision(THIS, spr)) 
			{
			
				switch(data-> UPGRADE_TYPE)
				{
					case SWORD: 
					SetSpriteAnim(THIS, orb_axe, ANIMATION_ORB);
					data -> UPGRADE_TYPE = AXE;
					break;
					case AXE: 
					SetSpriteAnim(THIS, orb_arrow, ANIMATION_ORB);
					data -> UPGRADE_TYPE = ARROW;
					break;
					case ARROW: 
					SetSpriteAnim(THIS, orb_sword, ANIMATION_ORB);
					data -> UPGRADE_TYPE = SWORD;
					break;
				}
				
					
				SpriteManagerRemove(i);
				
			}
		}
	}




}
void Destroy_SpriteOrb() {
	
	
}