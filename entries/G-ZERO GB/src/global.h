#ifndef _GLOBAL
#define _GLOBAL

#include <gb/gb.h>
#include "bcd.h"
#include "data.h"
#include "player.h"
#include "race.h"
#include "race_finished.h"
#include "end.h"
#include "title.h"
#include "string.h"
#include "music.h"
#include "sound.h"
#include "gbt_player.h"
#include "unapack.h"

#define UPDATE_KEYS()	old_input = input; input = joypad()
#define KEY_PRESSED(K)	(input & (K))
#define KEY_DEBOUNCE(K) ((input & (K)) && (old_input & (K)))
#define KEY_TICKED(K)   ((input & (K)) && !(old_input & (K)))
#define KEY_RELEASED(K) ((old_input & (K)) && !(input & (K)))

#define SN_PLAYER1			0
#define SN_PLAYER2			1
#define SN_PLAYER3			2
#define SN_MINI_PLAYER1		3
#define SN_MESSAGE1			4
#define SN_MESSAGE2			5
#define SN_MESSAGE3			6
#define SN_MESSAGE4			7
#define SN_MESSAGE5			8
#define SN_MESSAGE6			9
#define SN_MESSAGE7			10
#define SN_MESSAGE8			11
#define SN_MESSAGE9			12

#define ST_PLAYER1			0
#define ST_PLAYER2			2
#define ST_PLAYER3			4
#define ST_PLAYER4			6
#define ST_PLAYER5			8
#define ST_PLAYER6			10
#define ST_MINI_PLAYER1		12
#define ST_ONE1				14
#define ST_TWO1				16
#define ST_THREE1			18
#define ST_GO1				20
#define ST_GO2				22
#define ST_GO3				24
#define ST_FINISH1			26
#define ST_FINISH2			28
#define ST_FINISH3			30
#define ST_FINISH4			32
#define ST_FINISH5			34
#define ST_FINISH6			36
#define ST_FINISH7			38
#define ST_TIMEUP1			40
#define ST_TIMEUP2			42
#define ST_TIMEUP3			44
#define ST_TIMEUP4			46
#define ST_TIMEUP5			48
#define ST_TIMEUP6			50
#define ST_TIMEUP7			52
#define ST_POWEROUT1		54
#define ST_POWEROUT2        56
#define ST_POWEROUT3        58
#define ST_POWEROUT4        60
#define ST_POWEROUT5        62
#define ST_POWEROUT6        64
#define ST_POWEROUT7        66
#define ST_POWEROUT8        68
#define ST_POWEROUT9        70
#define ST_RACENAME1		72
#define ST_RACENAME2		74
#define ST_RACENAME3		76
#define ST_RACENAME4		78
#define ST_RACENAME5		80
#define ST_RACENAME6		82
#define ST_RACENAME7		84
#define ST_RACENAME8		86
#define ST_RACENAME9		88
#define ST_LAST				90
	
#define MSG_ONE				0
#define MSG_TWO				1
#define MSG_THREE			2
#define MSG_GO				3
#define MSG_TIMEUP			4
#define MSG_POWEROUT		5
#define MSG_FINISH			6
#define MSG_RACENAME		7

#define GS_TITLE			0
#define GS_RACE				1
#define GS_RACE_FINISHED	2
#define GS_END				3

extern UBYTE game_state;

extern UBYTE input;
extern UBYTE old_input;

extern BCD score;
extern BCD high_score;

extern UBYTE buf[4096];

void show_message( UBYTE message );
void hide_message();

void fade_from_white();
void fade_to_white();

#endif