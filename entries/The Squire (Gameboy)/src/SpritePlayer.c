#include "Banks/SetBank2.h"
#include "ZGBMain.h"

#include "../res/src/level.h"

#include "Keys.h"
#include "SpriteManager.h"

#define CameraBounds 88

#define SWORD 	0
#define AXE 	1
#define ARROW		2


const UINT8 move_anim[] = {2, 0, 1};
const UINT8 shielded_anim[] = {2, 3, 4};
const UINT8 death_anim[] = {2, 2, 3};
const UINT8 fade_anim[] = {3, 7, 8,9};
//PLAYER VARIABLES
UINT8 projectile_count;
UINT8 projetile_max;
extern UINT8 WEAPON;
UINT8 death_counter;
UINT8 player_hp;
INT8 MULTI_SCORE;

extern UINT16 Score;

INT8 player_scroll;

extern INT8 LIFE;
extern UINT8 LEVEL; 
extern UINT8 CONTROLS_OFF;

extern UINT16 CAM_POS;



extern UINT16 boundary;

extern UINT16 cam_posy;

extern struct ProjectileInfo 
{
UINT8 DAMAGE;
UINT8 WEAPON_TYPE;
};

void MovePlayer()
{
	
	//TranslateSprite(THIS_IDX, 0, -2 << delta_time);

}

void death();
void HUD();

void Spawn_Projectile(UINT16 x, UINT16 y, UINT8 WEAPON_TYPE);

void Start_SpritePlayer() {
	
player_hp = 1;	
death_counter = 0;
projectile_count = 0;
projetile_max = 2;
SetSpriteAnim(THIS, move_anim, 6);
//SetSpriteAnim(THIS, shielded_anim, 6);

THIS->coll_x = 1;
	THIS->coll_y = 1;
	THIS->coll_w = 15;
	THIS->coll_h = 15;



}

void GameOver()
{
	SetState(StateGameOver);
}

void Update_SpritePlayer() 
{

if(player_hp == 0 && death_counter < 60)
	{
		death_counter++;
		
	}
	
if(death_counter == 45)
		{
			
			//SetSpriteAnim(THIS, fade_anim, 10);
		}
if(death_counter == 60)
		{
			
			if(LIFE == 1)
			{
				SetState(StateGameOver);
			}
			else
			{
				SetState(StateLevel);
				//
			}
			//SpriteManagerRemove(THIS_IDX);
			
			//ameOver();
		}

//SHOOTING
if((KEY_TICKED(J_A)||KEY_TICKED(J_B)) && (projectile_count < projetile_max&& player_hp != 0) && CONTROLS_OFF != 1)
{
	projectile_count++;
	Spawn_Projectile(THIS->x, THIS->y-8, WEAPON);
}
	//MOVEMENT
	if(KEY_PRESSED(J_UP) && THIS -> y != 0 && THIS -> y >CAM_POS-30 && player_hp != 0 && CONTROLS_OFF != 1) 
	{
	TranslateSprite(THIS, 0, -1 << delta_time);
	} 
	
	if(KEY_PRESSED(J_DOWN) && THIS -> y < cam_posy&& player_hp != 0 && CONTROLS_OFF != 1)
 		
	{

	TranslateSprite(THIS, 0, 1 << delta_time);
	}
	
	
	
	if(KEY_PRESSED(J_LEFT) && THIS -> x != 0&& player_hp != 0 && CONTROLS_OFF != 1) 
	{
    TranslateSprite(THIS, -1 << delta_time, 0);
	}
	
	if(KEY_PRESSED(J_RIGHT) && THIS -> x != 144 && player_hp != 0 && CONTROLS_OFF != 1)
	{

	TranslateSprite(THIS, 1 << delta_time, 0);
	
	}
	
	//FINISH LEVEL
	if(CAM_POS < 48)
	{
		CONTROLS_OFF = 1;
		TranslateSprite(THIS, 0, -1 << delta_time);
	}
	
	if(THIS -> y < 16)
	{
		
		if(LEVEL != 5)
		{	
		LEVEL++;
		LIFE++;
		SpriteManagerRemove(THIS_IDX);
		SetState(StateLevel);
		}
		else
		{
		for(MULTI_SCORE = 0; MULTI_SCORE<LIFE; MULTI_SCORE++)
		{
			Score = Score + 2500;
		}			
		SetState(StateEnd);
		}	
	}
	
	
	
	if(THIS -> y > cam_posy)
	{
		TranslateSprite(THIS, 0, -1 << delta_time);
	}
	// GAME OVER SCREEN
	if(THIS -> y > cam_posy+1 && player_hp != 0)
	{
		
		
			
		HUD();
		SetSpriteAnim(THIS, death_anim, 20);
		player_hp = 0;
		
	}
	
	/*if(THIS -> y > cam_posy+4)
	{
		reset();
	}
		
		*/
		
	UINT8 i;
	struct Sprite* spr;
	

	SPRITEMANAGER_ITERATE(i, spr) 
	{
		if(spr->type == SpriteSlime 
		|| spr->type == SpriteKnight 
		|| spr->type == SpriteSkull 
		|| spr->type == SpriteHoblin 
		|| spr->type == SpriteEnemyProjectile 
		&& player_hp > 0) 
		{
			if(CheckCollision(THIS, spr)) 
			{
				
				HUD();
				SetSpriteAnim(THIS, death_anim, 20);
				player_hp = 0;
				WEAPON = SWORD;
				if(spr->type == SpriteEnemyProjectile)
				{
				SpriteManagerRemove(i);
				}
				//SpriteManagerRemove(THIS_IDX);
				//SetState(StateGameOver);
				//reset();
			}
		}
		
	}
		
		

		
		
	}





void Destroy_SpritePlayer() {
	
}