
#include "ParentedActors.h"
#include "Actor.h"
#include "DataManager.h"
#include "data_ptrs.h"

void PositionParentedActors() {
  UBYTE j;
	  // Linked sprite magic
  for (UBYTE i = 1; i < 10; ++i) // loop through the control variables
  {
	  UBYTE parent = script_variables[i+90] & 0x0F;
	  UBYTE L = script_variables[i+90] & 0x10;
	  UBYTE U = script_variables[i+90] & 0x20;
	  UBYTE D = script_variables[i+90] & 0x40;
	  UBYTE R = script_variables[i+90] & 0x80;
	  // set position relative to player
	  if (parent == 10) {
		  if (L) {
			  actors[i].pos.x = player.pos.x-16;
		  } else if (R) {
			  actors[i].pos.x = player.pos.x+16;
		  } else {
			actors[i].pos.x = player.pos.x;
		  }
		  if (U) {
			  actors[i].pos.y = player.pos.y-16;
		  } else if (D) {
			  actors[i].pos.y = player.pos.y+16;
		  } else {
			actors[i].pos.y = player.pos.y;
		  }

		  actors[i].dir.x = player.dir.x;
		  actors[i].dir.y = player.dir.y;
		  actors[i].frame = player.frame;
		  actors[i].rerender = TRUE;
		  
	  } else if (parent > 0) {
		  if (L) {
			  actors[i].pos.x = actors[parent].pos.x-16;
		  } else if (R) {
			  actors[i].pos.x = actors[parent].pos.x+16;
		  } else {
			actors[i].pos.x = actors[parent].pos.x;
		  }
		  if (U) {
			  actors[i].pos.y = actors[parent].pos.y-16;
		  } else if (D) {
			  actors[i].pos.y = actors[parent].pos.y+16;
		  } else {
			actors[i].pos.y = actors[parent].pos.y;
		  }

		  actors[i].dir.x = actors[parent].dir.x;
		  actors[i].dir.y = actors[parent].dir.y;

		  actors[i].frame = actors[parent].frame;
		  for (j = 0; j != actors_active_size; j++) {
			if (actors_active[j] == parent) {
			  ActivateActor(i);
			  break;
			}
		  }
		  actors[i].rerender = TRUE;
	  }
  }
}