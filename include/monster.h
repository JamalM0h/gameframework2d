#ifndef _MONSTER_H_ 
#define _MONSTER_H_

#include "entity.h"

Entity *monster_new_entity(GFC_Vector2D pos);

Entity* entity_load(const char* filename); 

#endif