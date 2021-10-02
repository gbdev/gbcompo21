#include "Sprites.h"
#include "Music.h"
#include "Enemy.h"


static UINT8 Left = 0;

static UINT8 Right = 0;

static UINT8 FrameCounter = 0;

static UINT8 TopParralaxEnabled = false;

static UINT8 Timer = 0;

static UINT8 StageTrigger = 0;

static UINT8 StageTimer = 0;

static UINT8 StageX = 0;

static UINT8 StageX2 = 144;

static UINT8 BInputTimer = 1;



UINT8 BackgoundOffset1;
UINT8 BackgoundOffset2;
UINT8 BackgoundOffset3;
UINT8 backgroundoffset3x;

static UINT8 PlayerTransition = false;

static UINT8 RepositionFlag = 0;


extern unsigned char earth_map_data[];
extern unsigned char earth_tile_data[];
extern unsigned char Stage_One[];



extern unsigned char moon_map_data[];
extern unsigned char moon_tile_data[];
extern unsigned char Stage_Two[];

extern unsigned char mars_map_data[];
extern unsigned char mars_tile_data[];
extern unsigned char Stage_Three[];

extern unsigned char jupiter_map_data[];
extern unsigned char jupiter_tile_data[];
extern unsigned char Stage_Four[];

extern unsigned char saturn_map_data[];
extern unsigned char saturn_tile_data[];
extern unsigned char Stage_Five[];

extern unsigned char pluto_map_data[];
extern unsigned char pluto_tile_data[];
extern unsigned char Stage_Six[];

extern unsigned char xous_map_data[];
extern unsigned char xous_tile_data[];
extern unsigned char Stage_Seven[];

extern unsigned char boss_data_1[];
extern unsigned char boss_data_2[];
extern unsigned char boss_data_3[];
extern unsigned char boss_data_4[];
extern unsigned char boss_data_5[];
extern unsigned char boss_data_6[];
extern unsigned char boss_data_7[];
extern unsigned char boss_data_8[];
extern unsigned char boss_data_9[];
extern unsigned char boss_data_10[];

extern unsigned char sniper_data_1[];
extern unsigned char sniper_data_2[];
extern unsigned char sniper_data_3[];
extern unsigned char sniper_data_4[];

extern unsigned char menu_map_data[];
extern unsigned char menu_tile_data[];

extern unsigned char thanks_data_1[];
extern unsigned char thanks_data_2[];
extern unsigned char thanks_data_3[];
extern unsigned char thanks_data_4[];
extern unsigned char thanks_data_5[];
extern unsigned char thanks_data_6[];
extern unsigned char thanks_data_7[];
extern unsigned char thanks_data_8[];

void Fade(){
	BGP_REG = 0xFE;
	delay(100);
	BGP_REG = 0xF9;
	delay(100);
	BGP_REG = 0xE4;
}




void DisplayHomeScreen(){
	set_bkg_data( 0, 15, menu_tile_data);
	set_bkg_tiles( 0, 0, 20, 18, menu_map_data);

	DISPLAY_ON;

	while (true){
		move_bkg(0,0);

		if (joypad()) {
			Fade();
			DISPLAY_OFF;
			break;
		}
		
	}
}

void InitGame(){
	PlayerX = 75;
	PlayerY = 75;


	TransitionFlag = 1;


	set_sprite_data(0, 1 , player_data_1);
	set_sprite_tile( 0, 0 );

	set_sprite_data(1, 2 , player_data_2);
	set_sprite_tile( 1, 1 );

	set_sprite_data(2, 3 , player_data_3);
	set_sprite_tile( 2, 2 );

	set_sprite_data(3, 4 , player_data_4);
	set_sprite_tile( 3, 3 );

	set_sprite_data(4, 5 , player_data_5);
	set_sprite_tile( 4, 4 );

	set_sprite_data(5, 6 , player_data_6);
	set_sprite_tile( 5, 5 );

	set_sprite_data(6, 7 , player_data_bullet);
	set_sprite_tile( 6, 6 );

	set_sprite_data(7, 8 , player_data_dust);
	set_sprite_tile( 7, 7 );

	set_sprite_data(8, 9 , enemy_data_1);
	set_sprite_tile( 8, 8 );

	set_sprite_data(9, 10 , enemy_data_2);
	set_sprite_tile( 9, 9 );

	set_sprite_data(10, 11 , enemy_data_3);
	set_sprite_tile( 10, 10 );

	set_sprite_data(11, 12 , enemy_data_4);
	set_sprite_tile( 11, 11 );

	set_sprite_data(12, 13 , enemy_data_5);
	set_sprite_tile( 12, 12 );

	set_sprite_data(13, 14 , enemy_data_6);
	set_sprite_tile( 13, 13 );

	set_sprite_data(14, 15 , Stage_1);
	set_sprite_tile( 14, 14 );

	set_sprite_data(15, 16 , Stage_2);
	set_sprite_tile( 15, 15 );

	set_sprite_data(16, 17 , Stage_3);
	set_sprite_tile( 16, 16 );

	set_sprite_data(17, 18 , zipper_data_1);
	set_sprite_tile( 17, 17 );

	set_sprite_data(18, 19 , zipper_data_1);
	set_sprite_tile( 18, 18 );

	set_sprite_data(19, 20 , shooter_data_1);
	set_sprite_tile( 19, 19 );

	set_sprite_data(20, 21 , shooter_data_2);
	set_sprite_tile( 20, 20 );

	set_sprite_data(21, 22 , shooter_data_3);
	set_sprite_tile( 21, 21 );

	set_sprite_data(22, 23 , shooter_data_4);
	set_sprite_tile( 22, 22 );

	set_sprite_data(23, 24 , shooter_data_5);
	set_sprite_tile( 23, 23 );

	set_sprite_data(24, 25 , player_data_bullet);
	set_sprite_tile( 24, 24 );

	set_sprite_prop(24, S_FLIPX);

	set_sprite_data(25, 26 , player_data_heart);
	set_sprite_tile( 25, 25 );

	set_sprite_data(26, 27 , player_data_heart);
	set_sprite_tile( 26, 26 );

	set_sprite_data(27, 28 , player_data_heart);
	set_sprite_tile( 27, 27 );

	DISPLAY_ON;

	Level = 1;
}



void ParralaxInterrupt(){
	if (TopParralaxEnabled){
		switch (LYC_REG)
		{
			case 0x00:
				move_bkg(BackgoundOffset1,0);
				LYC_REG = 0x1F;
				break;
			case 0x1F:
				if (TopParralaxEnabled == true){ 
					move_bkg(BackgoundOffset3,0);
					LYC_REG = 0x70;
				}
				break;  
			case 0x70:
				move_bkg(BackgoundOffset2,0);
				LYC_REG = 0x00;
				break;        
		}
	}
	else {
		switch (LYC_REG)
		{
			case 0x00:
				move_bkg(BackgoundOffset1,0);
				LYC_REG = 0x70;
				break;
			case 0x70:
				move_bkg(BackgoundOffset2,0);
				LYC_REG = 0x00;
				break;        
		}
	}
}

void InitParallax(){
    STAT_REG = 0x45;
    LYC_REG = 0x00;

    disable_interrupts();
    add_LCD(ParralaxInterrupt);
    enable_interrupts();

    set_interrupts(VBL_IFLAG | LCD_IFLAG);
}

void UpdateInputs() {

    if (!PlayerTransition){

        if (joypad() & J_RIGHT) {
            PlayerX += 1;
            

            scroll_bkg(1,0);

            move_sprite(7,PlayerX - 8 , PlayerY + 8);

            Right = 1;
            Direction = 2;


        }
        else{
            move_sprite(7,160, 8);
            Right = 0;
        }

        if (joypad() & J_LEFT) {

            PlayerX -= 1;

            Left = 1;

            Direction = 4;

        }

        else {
            Left = 0;
        }
        if (joypad() & J_UP) {

            if (PlayerY >= 8)PlayerY -= 1;
            Direction = 1;

        }

        if (joypad() & J_DOWN) {

            PlayerY += 1;
            Direction = 3;

        }

        if (joypad() & J_B) {
            if (BInputTimer <= 60)BInputTimer += 20;
        }

        if (joypad() & J_A){
            if (ProjectileTrigger == 0){
                ProjectileX = PlayerX + 24;

                ProjectileY = PlayerY + 8;

                ProjectileTrigger = 1;
                NR52_REG = 0x80;
                NR51_REG = 0x11;
                NR50_REG = 0x77;

                NR10_REG = 0x24;
                NR11_REG = 0x12;
                NR12_REG = 0x3F;
                NR13_REG = 0xFD;
                NR14_REG = 0x85;
            }
        }


        if (Right == 0 && Left == 0){
            if (Acceloration != 0){
                Acceloration --;
            }
        }
        else {
            if (Acceloration != 30){
                Acceloration ++;
            }
        }

        if (Acceloration != 0){

            if (Direction == 2){
                PlayerX += (Acceloration / 10);
                BackgoundOffset1 +=1;
                BackgoundOffset2 +=2;
            }

            if (Direction == 4){
                PlayerX -= (Acceloration / 10);
            }
        }

        if(PlayerX >= 140 && Direction == 2){
            PlayerX -= 1 + Acceloration / 10;
        }


        if(PlayerX <= 8 && Direction == 4){
            PlayerX += 1 + Acceloration / 10;
        }
    }

}

void DrawPlayer(UINT8 mode ){
    if (mode == 1){
        move_sprite(0,PlayerX,PlayerY);
        move_sprite(1,PlayerX + 8,PlayerY);
        move_sprite(2,PlayerX + 16,PlayerY);
        move_sprite(3,PlayerX,PlayerY + 8);
        move_sprite(4,PlayerX + 8,PlayerY + 8);
        move_sprite(5,PlayerX + 16,PlayerY + 8);
    }
    else{
        move_sprite(0,0,0);
        move_sprite(1,0,0);
        move_sprite(2,0,0);
        move_sprite(3,0,0);
        move_sprite(4,0,0);
        move_sprite(5,0,0);
    }
}


void UpdatePlayer(){
    if (Level != 8){
        if (PlayerHealth >= 2 ){
            move_sprite(25,14,18);
        }
        else {
            move_sprite(25,0,0);
        }
        if (PlayerHealth >= 4 ){
            move_sprite(26,24,18);
        }
        else {
            move_sprite(26,0,0);
        }
        if (PlayerHealth >= 6 ){
            move_sprite(27,34,18);
        }
        else {
            move_sprite(27,0,0);
        }

        if (PlayerHealth == 0){
            Level = 1;
            TransitionFlag = 1;
            ResetEnemysprites();
            PlayerX = 75;
            PlayerY = 75;
            PlayerHealth = 6;
        }


        if (Hit == 1){
            if (AnimationTicker % 2 == 0){
                DrawPlayer(1);
            }
            else {
                DrawPlayer(0);
            }

            if (AnimationTicker == 99){
                Hit = 0;
            }
        }
        else{
            DrawPlayer(1);
        }

        

        if (ProjectileTrigger == 1){
            move_sprite(6 , ProjectileX , ProjectileY);
            ProjectileX += 4;

            if (ProjectileX >= 180){
                ProjectileTrigger = 0;
            }
        }
    }
    else {
        move_sprite(25,0,0);
        move_sprite(26,0,0);
        move_sprite(27,0,0);
    }
}
