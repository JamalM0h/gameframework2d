#ifndef _TERRAIN_H_ 
#define _TERRAIN_H_

#include "entity.h"

Entity* ice_new_entity(GFC_Vector2D pos, Bool temp);
Entity* lava_new_entity(GFC_Vector2D pos, Bool temp);
Entity* water_new_entity(GFC_Vector2D pos, Bool temp);
Entity* metal_new_entity(GFC_Vector2D pos);
Entity* barrel_new_entity(GFC_Vector2D pos);
Entity* stone_new_entity(GFC_Vector2D pos);
Entity* explosion_entity(GFC_Vector2D pos);
Entity* collect_new_entity(GFC_Vector2D pos, int elem);
Entity* eletricg_new_entity(GFC_Vector2D pos);
Entity* world_col_new_entity(GFC_Vector2D pos);
Entity* eraser_entity(GFC_Vector2D pos, const char* filename, int roomnum, Bool explo);

#endif