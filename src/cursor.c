#include "simple_logger.h"

#include "terrain.h"
#include "world.h"

void cursor_update(Entity* self);
void cursor_free(Entity* self);
void cursor_collide(Entity* self, Entity* collide, int roomnum); 

int eraserlive = 10;
const char* file;
Bool explosion = false;

Entity* eraser_entity(GFC_Vector2D pos, const char* filename, int roomnum, Bool explo)
{
	Entity* self; 

	self = entity_new();

	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}

	self->obj = "eraser";

	self->frame = 0; 
	self->position = gfc_vector2d(pos.x, pos.y); 

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 4, 4);

	if (explo == true)
	{
		rect = gfc_rect(pos.x - 128, pos.y - 128, 256, 256);
	}

	self->hitbox = rect; 

	self->update = cursor_update;
	self->free = cursor_free; 
	self->collide = cursor_collide; 

	self->height = 32;
	self->width = 32;

	self->roomnum = roomnum;

	file = filename;

	if (explo == true)
	{
		explosion = true;
	}
	else
	{
		explosion = false;
	}

	self->lifetime = eraserlive;

	return self;
}

void cursor_update(Entity* self)
{

	if (!self)return;

	if (self->lifetime <= eraserlive)
	{
		self->lifetime -= 0.10;
	}
	if (self->lifetime <= 0)
	{
		cursor_free(self);
	}

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

void cursor_free(Entity* self)
{
	if (!self)return;
	//if (self->free)self->free(self);
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
	}
	if (self->obj)
	{
		self->obj = NULL;
	}
	if (self->think)
	{
		self->think = NULL;
	}
	if (self->update)
	{
		self->update = NULL;
	}
	if (self->collide)
	{
		self->collide = NULL;
	}
	if (self->data)
	{
		self->data = NULL;
	}
	if (self->damage)
	{
		self->damage = NULL;
	}
	memset(self, 0, sizeof(Entity));
}

void cursor_collide(Entity* self, Entity* collide, int roomnum)
{
	if (!self)return;
	if (collide->obj == "lava" || collide->obj == "ice" || collide->obj == "water" || collide->obj == "metal" || collide->obj == "monster" || collide->obj == "barrel" || collide->obj == "stone" || collide->obj == "collect" || collide->obj == "gate")
	{
		if (explosion != true)
		{
			delete_entity(file, collide, self->roomnum); 
		}
		collide->free(collide); 
	}
}

