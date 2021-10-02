#ifndef _SOUND
#define _SOUND

#define IDLE		0
#define SET			1
#define STOP		2

#define SND_START	0
#define SND_SPEED	1
#define SND_PAUSE	2
#define SND_BEEP1	3
#define SND_BEEP2	4
#define SND_BLIB	5
#define SND_HIT		6

void init_sound();
void set_sound( UBYTE sound );
void update_sound();
void stop_sound();

#endif