#ifndef _WORLD_H_
#define _WORLD_H_

#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "entity.h"

typedef struct
{
	Sprite *background;
	Sprite *tileLayer;
	Sprite *tileSet;
	Uint8 *tileMap;
	Uint32 tileHeight;
	Uint32 tileWidth;

}World;

World* world_load(const char* filename, Bool loadent); 

void save_entity(const char* filename, int entid, GFC_Vector2D pos, Entity* ent);

void delete_entity(const char* filename, Entity* ent);

void toggle_tile(const char* filename, int row, int column);

World *world_test_new();

World *world_new(Uint32 width, Uint32 height); 

void world_free(World *world);

void world_draw(World* world);


#endif
