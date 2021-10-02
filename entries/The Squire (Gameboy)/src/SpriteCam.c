#include "Banks/SetBank2.h"
#include "ZGBMain.h"


#include "Keys.h"
#include "SpriteManager.h"

#define CameraBounds 90
#define PlayerBoundary 112

UINT8 move_counter;
UINT16 boundary;
UINT16 cam_posy;
UINT8 start_counter;
INT16 cam_Start;

UINT16 CAM_POS;

extern UINT8 player_hp;


extern struct PlayerInfo
{
	
	INT16 pos;
};

void MovePlayer();

void Start_SpriteCam() 
{

cam_Start = THIS -> y;
move_counter = 0;
start_counter = 0;

//THIS -> y = THIS -> y - (160-64);
//am_posy = THIS->y;
//THIS-> y = THIS -> y -48;
}


void Update_SpriteCam() 
{
	//start_counter++
	
	if(THIS-> y > cam_Start - 112)
	{
		THIS -> y--;
		THIS -> y--;
		
	}
	
	cam_posy = THIS->y + CameraBounds;
	move_counter++;
	CAM_POS = THIS-> y;
	
	/*if(start_counter < 30)
	{
			start_counter++;
	}
	
if(start_counter == 30)
{
	start_counter = 31;
	THIS -> y = THIS -> y -PlayerBoundary;
}
*/

	
	if(move_counter == 5 && THIS -> y > 30 && player_hp != 0)
	{
		move_counter =0;
	
		TranslateSprite(THIS, 0, -1<< delta_time);
		
		
	
	}

	}




void Destroy_SpriteCam() {
}