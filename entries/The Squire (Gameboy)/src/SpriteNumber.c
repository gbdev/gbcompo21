#include "Banks/SetBank2.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Sprite.h"

extern INT8 LEVEL; 

const UINT8 frame_0[] = {1, 0};
const UINT8 frame_1[] = {1, 1};
const UINT8 frame_2[] = {1, 2};
const UINT8 frame_3[] = {1, 3};
const UINT8 frame_4[] = {1, 4};
const UINT8 frame_5[] = {1, 5};
const UINT8 frame_6[] = {1, 6};
const UINT8 frame_7[] = {1, 7};
const UINT8 frame_8[] = {1, 8};
const UINT8 frame_9[] = {1, 9};
	
void Start_SpriteNumber() 
{
	switch(LEVEL)
	{
	case 0: SetSpriteAnim(THIS, frame_0, 0); break;
	case 1: SetSpriteAnim(THIS, frame_1, 0); break;
	case 2: SetSpriteAnim(THIS, frame_2, 0); break;
	case 3: SetSpriteAnim(THIS, frame_3, 0); break;
	case 4: SetSpriteAnim(THIS, frame_4, 0); break;
	case 5: SetSpriteAnim(THIS, frame_5, 0); break;
	case 6: SetSpriteAnim(THIS, frame_6, 0); break;
	case 7: SetSpriteAnim(THIS, frame_7, 0); break;
	case 8: SetSpriteAnim(THIS, frame_8, 0); break;
	case 9: SetSpriteAnim(THIS, frame_9, 0); break;
	}
}

void Update_SpriteNumber() 
{
}


void Destroy_SpriteNumber() {
}