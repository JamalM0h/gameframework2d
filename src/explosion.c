#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void explo_update(Entity* self);
void explo_free(Entity* self);

int ttl = 50;
int mf = 0;

Entity* explosion_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all( 
		"images/explosion.png",
		256,
		256,
		16,
		0);

	self->frame = 0;
	self->position = gfc_vector2d(pos.x + 40, pos.y + 40);

	self->update = explo_update;
	self->free = explo_free;

	self->height = 256;
	self->width = 256;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	self->lifetime = ttl;

	return self;
}

void explo_update(Entity* self)
{
	if (!self)return;

	if (self->lifetime <= ttl)
	{
		self->lifetime -= 0.10;
	}
	if (self->lifetime <= 0)
	{
		explo_free(self); 
	}
}

void explo_free(Entity* self)
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