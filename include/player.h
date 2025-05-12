#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"

Entity *player_new_entity();
void edit_create(GFC_Vector2D pos, int entityid, const char* filename);

#endif