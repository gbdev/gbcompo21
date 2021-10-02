#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Sound.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Sprite.h"

const UINT8 hit_anim[] = {2, 0, 1};


	

void Start_SpriteHit() 
{
SetSpriteAnim(THIS, hit_anim, 20);

}


void Update_SpriteHit() 
{
	

	if(THIS->anim_frame == 1) 
		{
	SpriteManagerRemove(THIS_IDX);		
		}
	


}


void Destroy_SpriteHit() {
	
	
}