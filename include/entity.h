#ifndef _ENTITY_H_ 
#define _ENTITY_H_ 

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_types.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

typedef struct Entity_S
{
	Uint8				_inuse;
	GFC_TextLine		name; 
	Sprite				*sprite;
	char                *obj;
	float               frame;
	int					health;
	int					element;
	int					index;
	const char*         world;
	int					roomnum;
	Bool				explode;
	Bool				ice;
	Bool				electric;
	Bool				wind;
	Bool				vo;
	GFC_Vector2D		position;
	GFC_Vector2D		velocity;
	GFC_Vector2D		dirb;
	GFC_Vector2D		acceleration;
	GFC_Vector2D		angle;
	GFC_Rect            hitbox;
	int					width;
	int					height;
	int					state;
	int					lifetime;
	float				rotation;
	void (*think)(struct Entity_S *self);
	void (*update)(struct Entity_S *self);
	void (*free)(struct Entity_S *self);
	void (*collide)(struct Entity_S *self, struct Entity_S *collider);
	void (*damage)(struct Entity_S* self, int damage, GFC_Vector2D winddir);
	void *data;

}Entity;

// Initialize entity sub entity_system_init
// Initial

void entity_system_init(Uint32 maxEnts);

void entity_system_free_all();

Entity* entity_new();

void entity_free(Entity *self);

void entity_system_think();

void entity_system_update();

void entity_system_draw();

void entity_system_collision();

void clear_all_worldcol();

void clear_all_ents();

#endif 