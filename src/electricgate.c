#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void eletricg_free(Entity* self);
void eletricg_damage(Entity* self, int element, GFC_Vector2D winddir);
void eletricg_collide(Entity* self, Entity* collide);

Entity* eletricg_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/gate.png",
		64,
		200,
		16,
		0);

	self->obj = "gate";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 200);

	self->hitbox = rect;

	self->free = eletricg_free; 
	self->damage = eletricg_damage;
	self->collide = eletricg_collide; 

	self->height = 124;
	self->width = 64;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void eletricg_free(Entity* self)
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

void eletricg_damage(Entity* self, int element, GFC_Vector2D winddir)
{
	if (!self)return;

	if (element == 3)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/metal.png",
			64,
			64,
			16,
			0);

		self->state = 2;
		metal_new_entity(gfc_vector2d(self->position.x, self->position.y));
		eletricg_free(self);
	}
}

void eletricg_collide(Entity* self, Entity* collide) 
{
	if ((self->state == 2) && ((collide->obj == "water") || (collide->obj == "metal")) && (collide->state == 1))
	{
		collide->damage(collide, 3, self->angle);
	}
}