#include <stdbool.h>
#include <rand.h>
#include "Player.h"
#define EMC 3

static UINT8 Level = 1; 
static UINT8 HitSoundFlag = 0;
static UINT8 KillCounter = 0;
static UINT8 MaxKills = 15;

static UINT8 EnemyBulletX = 0;
static UINT8 EnemyBulletY = 0;
static UINT8 EnemyBulletTrigger = false;
static UINT8 SoundEffectTimer = 1;
static UINT8 StageNumberTile = 29;

static UINT8 MiniBossDirection = 0;

static UINT8 BossHealth = 250;

static UINT8 BossAcceloration = 0;

static UINT8 BossHit = 0;

static UINT8 BossAttackCounter = 1;

static UINT8 BossAttackModeCounter = 1;


static UINT8 BossKillAnimationCounter = 1;

static UINT8 BossKillAnimation = false;







static UINT8 Random = 0;

typedef struct Enemy{
    UINT8 x;
    UINT8 y;
    UINT8 alive;
    
}Enemy;


static Enemy Enemys[EMC];




UINT8 GetCollision(UINT8 x, UINT8 y , UINT8 w, UINT8 h, UINT8 x2 , UINT8 y2 , UINT8 w2 , UINT8 h2){
    if( y+h <= y2 )
    {
        return false;
    }

    if( y >= y2+h2 )
    {
        return false;
    }

    if( x+w <= x2 )
    {
        return false;
    }

    if( x >= x2+w2 )
    {
        return false;
    }

    return true;

}
void ResetEnemy(UINT8 i){
    Enemys[i].x = 200;
    switch (i)
    {
        case 0:
            Enemys[i].y = 32 - 4;
            break;
        case 1:
            Enemys[i].y = 64;
            break;
        case 2:
            Enemys[i].y = 96+32;
            break;
    }
    Enemys[i].alive = true;
}


void ResetEnemys(){
    for (UINT8 i = 0 ; i < EMC ; i++){
        ResetEnemy(i);
        if (Level == 7){
            Enemys[1].x = 122;
        }
    }
}

void UnloadBoss(){
    move_sprite(StageNumberTile + 1  , 0, 0);
    move_sprite(StageNumberTile + 2  , 0, 0);
    move_sprite(StageNumberTile + 3  , 0, 0);
    move_sprite(StageNumberTile + 4  , 0, 0);
    move_sprite(StageNumberTile + 5 , 0, 0);
    move_sprite(StageNumberTile + 6  , 0, 0);
    move_sprite(StageNumberTile + 7  , 0, 0);
    move_sprite(StageNumberTile + 8  , 0, 0);
    move_sprite(StageNumberTile + 9  , 0, 0);
    move_sprite(StageNumberTile + 10 , 0, 0);
}



void UpdateEnemys(){

    if (SoundEffectTimer != 1){
        SoundEffectTimer --;
        NR52_REG = 0x80;
        NR51_REG = 0x11;
        NR50_REG = 0x77;

        NR10_REG = 0x20;
        NR11_REG = 0x81;
        NR12_REG = 0x43;
        NR13_REG = 0xC0;
        NR14_REG = 0x85;
    } 
    for (UINT8 i = 0 ; i < EMC ; i++){

        if (Level != 7){

            if (Enemys[i].alive){

                if (i == 1){

                    move_sprite(8,Enemys[i].x ,Enemys[i].y);
                    move_sprite(9,Enemys[i].x + 8 ,Enemys[i].y);
                    move_sprite(10,Enemys[i].x + 16 ,Enemys[i].y);
                    move_sprite(11,Enemys[i].x ,Enemys[i].y + 8);
                    move_sprite(12,Enemys[i].x + 8 ,Enemys[i].y + 8);
                    move_sprite(13,Enemys[i].x + 16 ,Enemys[i].y + 8);
                    if (AnimationTicker >= 0 && AnimationTicker <= 50){
                        Enemys[i].y --;
                    }
                    else {
                        Enemys[i].y ++;
                    }

                }
                else {
                    Enemys[i].x --;

                    if (AnimationTicker == 50){
                        Enemys[i].y -= 10;
                    }
                    if (AnimationTicker == 10){
                        Enemys[i].y += 10;
                    }
                }
                if (i == 2){
                    if (Level == 1){
                        move_sprite(17, Enemys[i].x ,Enemys[i].y);
                    }
                    else{
                        move_sprite(19 , Enemys[i].x + 8 ,Enemys[i].y );
                        move_sprite(20 , Enemys[i].x + 16 ,Enemys[i].y );
                        move_sprite(21 , Enemys[i].x ,Enemys[i].y + 8 );
                        move_sprite(22 , Enemys[i].x + 8 ,Enemys[i].y + 8 );
                        move_sprite(23 , Enemys[i].x + 16 ,Enemys[i].y + 8 );

                        if (AnimationTicker % 10 == 0 && EnemyBulletTrigger == false){
                            EnemyBulletTrigger = true;
                            EnemyBulletX = Enemys[i].x;
                            EnemyBulletY = Enemys[i].y;
                        }
                        if (EnemyBulletTrigger == true){
                            move_sprite(24 , EnemyBulletX , EnemyBulletY);

                            EnemyBulletX -= 4;

                            if (EnemyBulletX <= 8){
                                EnemyBulletTrigger = false;
                                move_sprite(24 , 0, 0);
                                EnemyBulletX = 0;
                                EnemyBulletY = 0;
                            }
                        }

                    }

                }
                if (i == 0){

                    if (Level <= 4){
                        move_sprite(18, Enemys[i].x ,Enemys[i].y);
                    }
                    else {
                        move_sprite(StageNumberTile + 2, Enemys[i].x ,Enemys[i].y);
                        move_sprite(StageNumberTile + 3, Enemys[i].x + 8 ,Enemys[i].y);
                        move_sprite(StageNumberTile + 4, Enemys[i].x + 16,Enemys[i].y);
                        move_sprite(StageNumberTile + 5, Enemys[i].x + 24 ,Enemys[i].y);
                        Enemys[i].x -= 2;
                    }
                }

                Enemys[i].x --;

                if (Enemys[i].y >= 112){
                    Enemys[i].y --;
                }
                

                if (GetCollision(Enemys[i].x , Enemys[i].y, 16 , 8 , ProjectileX , ProjectileY , 8 , 8) && ProjectileTrigger == 1){
                    Enemys[i].alive = false;
                    SoundEffectTimer = 2;
                    KillCounter ++;
                }

                if (GetCollision(PlayerX , PlayerY, 16 , 8 , EnemyBulletX , EnemyBulletY , 8 , 8) && Hit == 0){
                    Hit = 1;
                    PlayerHealth --;
                    SoundEffectTimer = 2;
                }
                if (GetCollision(Enemys[i].x , Enemys[i].y, 24 , 16 , PlayerX , PlayerY , 24 , 16) && Hit == 0){
                    Hit = 1;
                    PlayerHealth --;
                    SoundEffectTimer = 2;
                }

            
            }
            else{
                move_sprite(8,0 ,0);
                move_sprite(9,0 ,0);
                move_sprite(10,0 ,0);
                move_sprite(11,0 ,0);
                move_sprite(12,0 ,0);
                move_sprite(13,0 ,0);
                ResetEnemy(i);
                Enemys[i].alive = true;
            }

        

        }
        if (Level == 7){
            if (Enemys[i].alive){
                if (i == 1){

                    if (BossKillAnimation == true){
                        if (AnimationTicker % 2 == 1){
                            move_sprite(StageNumberTile + 1 , Enemys[i].x , Enemys[i].y);
                            move_sprite(StageNumberTile + 2 , Enemys[i].x + 8, Enemys[i].y);
                            move_sprite(StageNumberTile + 3 , Enemys[i].x + 16, Enemys[i].y);
                            move_sprite(StageNumberTile + 4 , Enemys[i].x + 24, Enemys[i].y);
                            move_sprite(StageNumberTile + 5 , Enemys[i].x + 32, Enemys[i].y);

                            move_sprite(StageNumberTile + 6  , 0, 0);
                            move_sprite(StageNumberTile + 7  , 0, 0);
                            move_sprite(StageNumberTile + 8  , 0, 0);
                            move_sprite(StageNumberTile + 9  , 0, 0);
                            move_sprite(StageNumberTile + 10 , 0, 0);
                            
                        }
                        else{
                            move_sprite(StageNumberTile + 1  , 0, 0);
                            move_sprite(StageNumberTile + 2  , 0, 0);
                            move_sprite(StageNumberTile + 3  , 0, 0);
                            move_sprite(StageNumberTile + 4  , 0, 0);
                            move_sprite(StageNumberTile + 5 , 0, 0);

                            move_sprite(StageNumberTile + 6 , Enemys[i].x , Enemys[i].y + 8);
                            move_sprite(StageNumberTile + 7 , Enemys[i].x + 8, Enemys[i].y + 8);
                            move_sprite(StageNumberTile + 8 , Enemys[i].x + 16, Enemys[i].y + 8);
                            move_sprite(StageNumberTile + 9 , Enemys[i].x + 24, Enemys[i].y + 8);
                            move_sprite(StageNumberTile + 10 , Enemys[i].x + 32, Enemys[i].y + 8);
                        }
                        BossKillAnimationCounter ++;

                        if (BossKillAnimationCounter == 120){
                            KillCounter = 100;
                        }

                    }
                    else{
                        move_sprite(StageNumberTile + 1 , Enemys[i].x , Enemys[i].y);
                        move_sprite(StageNumberTile + 2 , Enemys[i].x + 8, Enemys[i].y);
                        move_sprite(StageNumberTile + 3 , Enemys[i].x + 16, Enemys[i].y);
                        move_sprite(StageNumberTile + 4 , Enemys[i].x + 24, Enemys[i].y);
                        move_sprite(StageNumberTile + 5 , Enemys[i].x + 32, Enemys[i].y);

                        move_sprite(StageNumberTile + 6 , Enemys[i].x , Enemys[i].y + 8);
                        move_sprite(StageNumberTile + 7 , Enemys[i].x + 8, Enemys[i].y + 8);
                        move_sprite(StageNumberTile + 8 , Enemys[i].x + 16, Enemys[i].y + 8);
                        move_sprite(StageNumberTile + 9 , Enemys[i].x + 24, Enemys[i].y + 8);
                        move_sprite(StageNumberTile + 10 , Enemys[i].x + 32, Enemys[i].y + 8);
                    }

                    if (!BossKillAnimation){

                        if (MiniBossDirection == 0){
                            Enemys[i].y -= (1 + BossAcceloration / 10);
                        }

                        if (MiniBossDirection == 1){
                            Enemys[i].y += (1 + BossAcceloration / 10);
                        }

                        if (BossAcceloration != 30){
                            BossAcceloration ++;
                        }

                        if (Enemys[i].y <= 16){
                            MiniBossDirection = 1;
                            BossAcceloration = 0;
                        }

                        if (Enemys[i].y >= 112){
                            MiniBossDirection = 0;
                            BossAcceloration = 0;
                        }

                        if (GetCollision(Enemys[i].x , Enemys[i].y, 32 , 16 , ProjectileX , ProjectileY , 8 , 8) && BossHit == 0){
                            SoundEffectTimer = 2;
                            BossHit = 1;
                            BossHealth -= 10;
                        }

                        if (GetCollision(Enemys[i].x , Enemys[i].y, 32 , 16 , PlayerX , PlayerY , 8 , 8) && Hit == 0){
                            PlayerHealth --;
                            Hit = 1;
                            SoundEffectTimer = 2;
                        }


                        if (AnimationTicker == 50 && BossAttackCounter != 10){

                            if (BossAttackCounter == 4)BossAttackModeCounter = 255;
                            BossAttackCounter += 1;
                        }

                        if (BossAttackCounter == 10){
                            BossAttackModeCounter --;
                            if (Enemys[i].x <= PlayerX){
                                Enemys[i].x ++;
                            }
                            if (Enemys[i].x >= PlayerX){
                                Enemys[i].x --;
                            }
                            if (BossAttackModeCounter == 1){
                                Enemys[i].x = 112;
                                BossAttackCounter = 0;
                            }
                        }

                        if (BossHit == 1){
                            if (AnimationTicker % 2 == 0){
                                for (int i = 1 ; i <= 10 ; i ++){
                                    move_sprite(StageNumberTile + i, 0, 0);
                                }
                            }

                            if (AnimationTicker == 1){
                                BossHit = 0;
                            }
                        }

                        if (BossHealth <= 0){
                            BossKillAnimation = true;
                        }

                    }

                }
            }
        }

    }


}

void ResetEnemysprites(){
    move_sprite(8,0 ,0);
    move_sprite(9,0 ,0);
    move_sprite(10,0 ,0);
    move_sprite(11,0 ,0);
    move_sprite(12,0 ,0);
    move_sprite(13,0 ,0);
    move_sprite(17,0 ,0);
    move_sprite(18,0 ,0);
    move_sprite(19,0 ,0);
    move_sprite(20,0 ,0);
    move_sprite(21,0 ,0);
    move_sprite(22,0 ,0);
    move_sprite(23,0 ,0);
}