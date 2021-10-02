#include "ZGBMain.h"
#include "Math.h"

UINT8 next_state = StateIntro;

extern UINT8 LEVEL; 

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateGame) {
		if(U_LESS_THAN(255 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = 0;
			
			if(LEVEL == 4 || LEVEL == 5)
			{
			*tile = 124;	
			}	
			
			//chest
			if(*tile_ptr == 254)
			{
				*tile = 69;
			}
			
			return 255 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}

	return 255u;
}