#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Keys.h"
#include "SpriteManager.h"
#include "Sprite.h"

const UINT8 fire_anim[] = {3, 0, 1, 2};


void Start_SpriteFire() 
{
	SetSpriteAnim(THIS, fire_anim, 10);
}


void Update_SpriteFire() 
{
	

	if(THIS->anim_frame == 2) 
		{
		SpriteManagerRemove(THIS_IDX);		
		}
	


}


void Destroy_SpriteFire() {
	
	
}