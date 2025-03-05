#include "simple_logger.h"

#include "terrain.h"

void lava_think(Entity* self);
void lava_update(Entity* self);
void lava_free(Entity* self);
void lava_damage(Entity* self, int element);

Entity* lava_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/lava.png",
		64,
		64,
		16,
		0);

	self->obj = "lava";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 64);

	self->hitbox = rect;

	self->think = lava_think;
	self->update = lava_update;
	self->free = lava_free;
	self->damage = lava_damage;

	self->height = 64;
	self->width = 64;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	slog("obj spawned");
	return self;
}

void lava_think(Entity* self)
{
	if (!self)return;
}
void lava_update(Entity* self)
{
	if (!self)return;
}

void lava_free(Entity* self)
{
	if (!self)return;
	//if (self->free)self->free(self);
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
	}
	memset(self, 0, sizeof(Entity));
}

void lava_damage(Entity* self, int element)
{
	if (!self)return;
	if (element == 2)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/lava2.png",
			64,
			64,
			16,
			0);

		self->state = 2;
	}

	if (element == 1)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/lava.png",
			64,
			64,
			16,
			0);

		self->state = 1;
	}

}