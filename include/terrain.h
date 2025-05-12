#ifndef _TERRAIN_H_ 
#define _TERRAIN_H_

#include "entity.h"

Entity* ice_new_entity(GFC_Vector2D pos, Bool temp);
Entity* lava_new_entity(GFC_Vector2D pos, Bool temp);
Entity* water_new_entity(GFC_Vector2D pos, Bool temp);
Entity* world_col_new_entity(GFC_Vector2D pos);
Entity* eraser_entity(GFC_Vector2D pos); 

#endif