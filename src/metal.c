#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void metal_think(Entity* self);
void metal_update(Entity* self);
void metal_free(Entity* self);
void metal_damage(Entity* self, int element, GFC_Vector2D winddir);
void metal_collide(Entity* self, Entity* collide);

Entity* metal_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/metal.png",
		64,
		64,
		16,
		0);

	self->obj = "metal";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 64);

	self->hitbox = rect;

	self->think = metal_think;
	self->update = metal_update;
	self->free = metal_free;
	self->damage = metal_damage;
	self->collide = metal_collide;

	self->height = 64;
	self->width = 64;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void metal_think(Entity* self)
{
	if (!self)return;
}
void metal_update(Entity* self)
{
	if (!self)return;
}

void metal_free(Entity* self)
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

void metal_damage(Entity* self, int element, GFC_Vector2D winddir)
{
	if (!self)return;

	if (element == 3)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/metal2.png",
			64,
			64,
			16,
			0);

		self->state = 2;
	}
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

void metal_collide(Entity* self, Entity* collide)
{
	if ((self->state == 2) && ((collide->obj == "water") || (collide->obj == "metal") || (collide->obj == "gate")) && (collide->state == 1))
	{
		collide->damage(collide, 3, self->angle);
	}
}