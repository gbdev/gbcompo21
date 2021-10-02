#ifndef _PLAYER
#define _PLAYER

extern UWORD player_x;
extern UWORD player_speed;
extern UBYTE player_count;
extern UBYTE player_range;
extern UBYTE player_animation;
extern UBYTE player_power;
//extern UWORD player_boost;

void init_player();
void update_player();

#endif