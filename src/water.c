#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void water_update(Entity* self);
void water_free(Entity* self);
void water_damage(Entity* self, int element, GFC_Vector2D winddir);
void water_collide(Entity* self, Entity* collide); 

int waterlive = 900;

Entity* water_new_entity(GFC_Vector2D pos, Bool temp)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/water.png",
		64,
		64,
		16,
		0);

	self->obj = "water";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 64);

	self->hitbox = rect;

	self->update = water_update;
	self->free = water_free;
	self->damage = water_damage;
	self->collide = water_collide;

	self->height = 64;
	self->width = 64;

	if (temp == true)
		self->lifetime = waterlive; 
	else
		self->lifetime = waterlive + 1;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void water_update(Entity* self)
{
	if (!self)return;

	if (self->lifetime <= waterlive)
	{
		self->lifetime -= 0.10;
	}
	if (self->lifetime <= 0)
	{
		water_free(self);
	}
}

void water_free(Entity* self)
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

void water_damage(Entity* self, int element, GFC_Vector2D winddir)
{
	if (!self)return;

	if (element == 2)
	{
		ice_new_entity(self->position, true); 
		water_free(self);
	}

	else if (element == 3)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/water2.png",
			64,
			64,
			16,
			0);

		self->state = 2;
	}
}

void water_collide(Entity* self, Entity* collide)
{
	if ((self->state == 2) && ((collide->obj == "water")) && (collide->state == 1))
	{
		collide->damage(collide, 3, self->angle);	
	}
}