#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void collect_free(Entity* self);
void collect_collide(Entity* self, Entity* collide); 

Entity* collect_new_entity(GFC_Vector2D pos, int elem)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	if(elem == 1) 
	self->sprite = gf2d_sprite_load_all(
		"images/collect1.png",
		64,
		64,
		16,
		0);

	else if(elem == 2)
	self->sprite = gf2d_sprite_load_all( 
		"images/collect2.png",
		64,
		64,
		16,
		0);

	else if (elem == 3)
	self->sprite = gf2d_sprite_load_all(
		"images/collect3.png",
		64,
		64,
		16,
		0);

	else if (elem == 4)
	self->sprite = gf2d_sprite_load_all(
		"images/collect4.png",
		64,
		64,
		16,
		0);

	self->obj = "collect";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x + 12, pos.y + 5, 32, 32);

	self->hitbox = rect;

	self->free = collect_free; 
	self->collide = collect_collide;  

	self->height = 32;
	self->width = 32;

	if(elem == 1)
	self->state = 1;
	else if (elem == 2)
	self->state = 2;
	else if (elem == 3)
	self->state = 3;
	else if (elem == 4)
	self->state = 4;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void collect_free(Entity* self)
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
void collect_collide(Entity* self, Entity* collide)
{
	if (collide->obj == "player" && self->state == 1)
	{
		collide->ice = true;
		collect_free(self);
	}
	else if (collide->obj == "player" && self->state == 2)
	{
		collide->electric = true;
		collect_free(self); 
	}
	else if (collide->obj == "player" && self->state == 3)
	{
		collide->wind = true;
		collect_free(self);
	}
	else if (collide->obj == "player" && self->state == 4)
	{
		collide->vo = true;
		collect_free(self);
	}
}