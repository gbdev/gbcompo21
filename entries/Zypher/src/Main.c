#include <gb/gb.h>
#include <stdio.h>
#include "Core.h"
#include <gb/metasprites.h>



void main(){

	SPRITES_8x8;

	ENABLE_RAM_MBC1;
	SWITCH_4_32_MODE_MBC1;
	SWITCH_RAM_MBC1(0);
    DISPLAY_OFF;
	SHOW_BKG; 
	SHOW_SPRITES;

	DisplayHomeScreen();
	InitParallax();
	InitGame();

	ResetEnemys();

	while(1){

		if(KillCounter == MaxKills){
			ResetEnemysprites();
			PlayerTransition = true;
		}

		if (PlayerTransition){

			if (RepositionFlag == 0){
				if (PlayerX >= 75){
					PlayerX -= 1;
				}
				if (PlayerX <= 75){
					PlayerX += 1;
				}

				if (PlayerY >= 75){
					PlayerY -= 1;
				}

				if (PlayerY <= 75){
					PlayerY += 1;
				}

				if (GetCollision(PlayerX,PlayerY , 8 , 8 , 75 , 75 , 8 , 8)){
					RepositionFlag = 1;
				}
			}
			else {
				PlayerX ++;
				if (PlayerX == 160){
					Level ++;
					TransitionFlag = 1;
				}
				if (PlayerX == 70){
					PlayerTransition = false;
				}

			}
		}
		else {
			if (StageTrigger != 1){
				if (Level != 8)UpdateEnemys();
			}
		}

		if (Level != 0){
			if (StageTrigger == 1){

				move_sprite(14 , StageX , 64);
				move_sprite(15 , StageX + 8, 64);
				move_sprite(16 , StageX + 16, 64);

				move_sprite(StageNumberTile , StageX2, 64);

				if (StageX >= 72){
					if (StageX2 >= 72+28){
						StageX2 -= 4;
					}

					if (joypad()){
						if (StageTimer == 0)StageTimer = 1;
					}
					if (StageTimer != 0){

						if (AnimationTicker % 2 == 0){
							move_sprite(14 , 0 , 0);
							move_sprite(15 , 0 , 0);
							move_sprite(16 , 0 , 0);
							move_sprite(StageNumberTile  , 0 , 0);
						}
						else{
							move_sprite(14 , StageX , 64);
							move_sprite(15 , StageX + 8, 64);
							move_sprite(16 , StageX + 16, 64);
							move_sprite(StageNumberTile  , StageX2, 64);
						}

						StageTimer ++;

						if (StageTimer == 60){
							StageX = 0;
							StageX2 = 144;
							StageTimer = 0;
							StageTrigger = 0;
							Acceloration = 0;
							move_sprite(14 , 0 , 0);
							move_sprite(15 , 0 , 0);
							move_sprite(16 , 0 , 0);
							move_sprite(StageNumberTile  , 0 , 0);
						}

					}
				}
				else {
					StageX += 1 + Acceloration / 10;

					if (Acceloration != 30){
						Acceloration ++;
					}
				}
			}
			else{
				if (Level != 8)UpdateInputs();
			}
			UpdatePlayer();
			scroll_bkg(1,0);
			if (Level == 8)DrawPlayer(0);

			if (TopParralaxEnabled){
				BackgoundOffset1 += 1;
				BackgoundOffset2 += 4;
				BackgoundOffset3 += 2;
			}
			else {
				BackgoundOffset1 += 1;
				BackgoundOffset2 += 2;
			}

			if (PlayerY >= 112){
				PlayerY -= 2;
			}

			if (AnimationTicker <= 100){
				AnimationTicker ++;
				if (AnimationTicker  % 2 == 0){
					set_sprite_prop(7, S_FLIPX);
				}
				else {
					set_sprite_prop(7, !S_FLIPX);
				}
			}
			else {
				AnimationTicker = 0;
			}
			if (TransitionFlag == 1){
				DISPLAY_OFF;
				TopParralaxEnabled = false;

				BackgoundOffset1 = 0;
				BackgoundOffset2 = 0;
				BackgoundOffset3 = 0;

				ResetEnemys();
				UnloadBoss();
				move_sprite(6,0,0);
				move_sprite(24 , 0, 0);
				ProjectileTrigger = 0;
				Fade();
				switch (Level){
					case 2:
						SWITCH_RAM_MBC1(0);
						set_bkg_data( 0, 8, moon_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, moon_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_Two);
						set_sprite_tile( StageNumberTile, StageNumberTile );
						MaxKills = 20;
						TopParralaxEnabled = true;
						break;
					case 3:
						SWITCH_RAM_MBC1(0);
						set_bkg_data( 0, 13, mars_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, mars_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_Three);
						set_sprite_tile( StageNumberTile, StageNumberTile );
						MaxKills = 30;
						TopParralaxEnabled = false;
						break;
					case 4:
						SWITCH_RAM_MBC1(1);
						set_bkg_data( 0, 13, jupiter_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, jupiter_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_Four);
						set_sprite_tile( StageNumberTile, StageNumberTile );
						MaxKills = 30;
						TopParralaxEnabled = true;
						break;
					case 5:
						SWITCH_RAM_MBC1(1);
						set_bkg_data( 0, 20, saturn_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, saturn_map_data);

						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_Five);
						set_sprite_tile( StageNumberTile, StageNumberTile );

						MaxKills = 35;
						TopParralaxEnabled = true;
						break;
					case 6:
						SWITCH_RAM_MBC1(1);
						set_bkg_data( 0, 18, pluto_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, pluto_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_Six);
						set_sprite_tile( StageNumberTile, StageNumberTile );
						MaxKills = 40;
						TopParralaxEnabled = true;
						break;
					case 7:
						SWITCH_RAM_MBC1(1);
						set_bkg_data( 0, 18, xous_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, xous_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_Seven);
						set_sprite_tile( StageNumberTile, StageNumberTile );

						set_sprite_data(StageNumberTile + 1 , StageNumberTile + 2, boss_data_1);
						set_sprite_tile( StageNumberTile+ 1, StageNumberTile + 1);

						set_sprite_data(StageNumberTile + 2 , StageNumberTile + 3, boss_data_2);
						set_sprite_tile( StageNumberTile+ 2, StageNumberTile + 2);

						set_sprite_data(StageNumberTile + 3 , StageNumberTile + 4, boss_data_3);
						set_sprite_tile( StageNumberTile+ 3, StageNumberTile + 3);

						set_sprite_data(StageNumberTile + 4 , StageNumberTile + 5, boss_data_4);
						set_sprite_tile( StageNumberTile+ 4, StageNumberTile + 4);

						set_sprite_data(StageNumberTile + 5 , StageNumberTile + 6, boss_data_5);
						set_sprite_tile( StageNumberTile+ 5, StageNumberTile + 5);
						
						set_sprite_data(StageNumberTile + 6 , StageNumberTile + 7, boss_data_6);
						set_sprite_tile( StageNumberTile+ 6, StageNumberTile + 6);

						set_sprite_data(StageNumberTile + 7 , StageNumberTile + 8, boss_data_7);
						set_sprite_tile( StageNumberTile+ 7, StageNumberTile + 7);

						set_sprite_data(StageNumberTile + 8 , StageNumberTile + 9, boss_data_8);
						set_sprite_tile( StageNumberTile+ 8, StageNumberTile + 8);

						set_sprite_data(StageNumberTile + 9 , StageNumberTile + 10, boss_data_9);
						set_sprite_tile( StageNumberTile+ 9, StageNumberTile + 9);

						set_sprite_data(StageNumberTile + 10 , StageNumberTile + 11, boss_data_10);
						set_sprite_tile( StageNumberTile + 10, StageNumberTile + 10);

						MaxKills = 100;
						TopParralaxEnabled = false;
						break;

					case 8:
						SWITCH_RAM_MBC1(0);
						set_bkg_data( 0, 15, earth_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, earth_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_One);
						set_sprite_tile( StageNumberTile, StageNumberTile );

						set_sprite_data(StageNumberTile + 2 , StageNumberTile + 3, thanks_data_1);
						set_sprite_tile( StageNumberTile+ 2, StageNumberTile + 2);

						set_sprite_data(StageNumberTile + 3 , StageNumberTile + 4, thanks_data_2);
						set_sprite_tile( StageNumberTile+ 3, StageNumberTile + 3);

						set_sprite_data(StageNumberTile + 4 , StageNumberTile + 5, thanks_data_3);
						set_sprite_tile( StageNumberTile+ 4, StageNumberTile + 4);

						set_sprite_data(StageNumberTile + 5 , StageNumberTile + 6, thanks_data_4);
						set_sprite_tile( StageNumberTile+ 5, StageNumberTile + 5);
						
						set_sprite_data(StageNumberTile + 6 , StageNumberTile + 7, thanks_data_5);
						set_sprite_tile( StageNumberTile+ 6, StageNumberTile + 6);

						set_sprite_data(StageNumberTile + 7 , StageNumberTile + 8, thanks_data_6);
						set_sprite_tile( StageNumberTile+ 7, StageNumberTile + 7);

						set_sprite_data(StageNumberTile + 8 , StageNumberTile + 9, thanks_data_7);
						set_sprite_tile( StageNumberTile+ 8, StageNumberTile + 8);

						set_sprite_data(StageNumberTile + 9 , StageNumberTile + 10, thanks_data_8);
						set_sprite_tile( StageNumberTile+ 9, StageNumberTile + 9);
						MaxKills = 200;
						break;

					default:
						SWITCH_RAM_MBC1(0);
						set_bkg_data( 0, 15, earth_tile_data);
						set_bkg_tiles( 0, 0, 32, 18, earth_map_data);
						set_sprite_data(StageNumberTile , StageNumberTile + 1, Stage_One);
						set_sprite_tile( StageNumberTile, StageNumberTile );
						MaxKills = 15;
						break;
				}
				TransitionFlag = 0;
				KillCounter = 0;

				if (Level != 8)StageTrigger = 1;

				if (Level >= 5 && Level != 7 && Level != 8){
					set_sprite_data(StageNumberTile + 2 , StageNumberTile + 3, sniper_data_1);
					set_sprite_tile( StageNumberTile+ 2, StageNumberTile + 2);

					set_sprite_data(StageNumberTile + 3 , StageNumberTile + 4, sniper_data_2);
					set_sprite_tile( StageNumberTile+ 3, StageNumberTile + 3);

					set_sprite_data(StageNumberTile + 4 , StageNumberTile + 5, sniper_data_3);
					set_sprite_tile( StageNumberTile+ 4, StageNumberTile + 4);

					set_sprite_data(StageNumberTile + 5 , StageNumberTile + 6, sniper_data_4);
					set_sprite_tile( StageNumberTile+ 5, StageNumberTile + 5);
				}
			}
			DISPLAY_ON;
		}

		if (Level ==  8){
			for (int i = 1; i <= 8 ; i ++)move_sprite(StageNumberTile + 1 + i , 52 + i*8, 48);
			if (joypad() & J_START) {
				ResetEnemysprites();

				PlayerTransition = true;
			}
		}


        wait_vbl_done();

	}
}