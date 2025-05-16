#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void stone_think(Entity* self);
void stone_update(Entity* self);
void stone_free(Entity* self);
void stone_damage(Entity* self, int element, GFC_Vector2D winddir);

Entity* stone_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/stone.png",
		64,
		64,
		16,
		0);

	self->obj = "stone";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 64);

	self->hitbox = rect;

	self->think = stone_think;
	self->update = stone_update;
	self->free = stone_free;
	self->damage = stone_damage;

	self->height = 64;
	self->width = 64;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void stone_think(Entity* self)
{
	if (!self)return;
}
void stone_update(Entity* self)
{
	if (!self)return;

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

void stone_free(Entity* self)
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

void stone_damage(Entity* self, int element, GFC_Vector2D winddir)
{
	if (element == 4)
	{
		self->position.x += 0.6 * winddir.x;
		self->position.y += 0.6 * winddir.y;
	}

	if (element == 5)
	{
		self->position.x -= 1.2 * winddir.x;
		self->position.y -= 1.2 * winddir.y;
	}
}