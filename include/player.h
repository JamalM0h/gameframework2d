#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"
#include "world.h"

Entity *player_new_entity();
GFC_Vector2D edit_create(GFC_Vector2D pos, int entityid, const char* filename, int roomnum); 

#endif