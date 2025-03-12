#include "simple_logger.h"

#include "terrain.h"

void ice_think(Entity* self); 
void ice_update(Entity* self); 
void ice_free(Entity* self);
void ice_damage(Entity* self, int element, GFC_Vector2D winddir);

Entity* ice_new_entity(GFC_Vector2D pos, Bool temp)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/ice.png",
		64,
		64,
		16,
		0);

	self->obj = "ice";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 64);

	self->hitbox = rect;

	self->think = ice_think;
	self->update = ice_update;
	self->free = ice_free;
	self->damage = ice_damage;

	if (temp == true)
		self->lifetime = 800;
	else
		self->lifetime = 801;

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

void ice_think(Entity* self)
{
	if (!self)return;
}
void ice_update(Entity* self)
{
	if (!self)return;

	if (self->lifetime <= 800)
	{
		self->lifetime -= 0.10;
	}
	if (self->lifetime <= 0)
	{
		ice_free(self);
	}

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

void ice_free(Entity* self)
{
	if (!self)return;
	//if (self->free)self->free(self);
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
	}
	memset(self, 0, sizeof(Entity));
}

void ice_damage(Entity* self, int element, GFC_Vector2D winddir)
{
	if (!self)return;
	if (element == 1)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/ice2.png",
			64,
			64,
			16,
			0);

		self->state = 2;
	}

	if (element == 2)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/ice.png",
			64,
			64,
			16,
			0);

		self->state = 1;
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