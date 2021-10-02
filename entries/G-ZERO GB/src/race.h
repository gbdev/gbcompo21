#ifndef _RACE
#define _RACE

#define HUD_SPEED	0
#define HUD_POWER	1
#define HUD_LAP		2
#define HUD_TIME	3
	
#define RS_RACE		0
#define RS_TIMEUP	1
#define RS_POWEROUT	2
#define RS_FINISH	3

#define RN_RACE1	0
#define RN_RACE2	1
#define RN_RACE3	2
#define RN_RACE4	3

extern UBYTE race_number;
extern UWORD race_count;
extern UWORD race_score;
extern UBYTE race_scroll;
extern UBYTE race_lap;
extern UBYTE race_state;

extern BYTE input_acceleration;
extern BYTE curve_acceleration;
extern BYTE bounce_acceleration;

extern BCD minutes;
extern BCD seconds;
extern BCD hundredth;
extern UBYTE hundredth_count;

void update_hud( UBYTE type );
void run_race();

#endif