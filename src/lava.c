#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void lava_think(Entity* self);
void lava_update(Entity* self);
void lava_free(Entity* self);
void lava_damage(Entity* self, int element, GFC_Vector2D winddir);

int lavalive = 900;

Entity* lava_new_entity(GFC_Vector2D pos, Bool temp)
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

	if (temp == true)
		self->lifetime = lavalive;
	else
		self->lifetime = lavalive + 1;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void lava_think(Entity* self)
{
	if (!self)return;
}
void lava_update(Entity* self)
{
	if (!self)return;

	if (self->lifetime <= lavalive)
	{
		self->lifetime -= 0.10;
	}
	if (self->lifetime <= 0)
	{
		lava_free(self);
	}
}

void lava_free(Entity* self)
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

void lava_damage(Entity* self, int element, GFC_Vector2D winddir)
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

	if ((element == 4) && (self->state != 2))
	{
		Entity *proj;

		proj = create_projectile(gfc_vector2d(self->position.x, self->position.y), 1);
		proj->angle = winddir;

		proj = create_projectile(gfc_vector2d(self->position.x, self->position.y), 1);
		proj->angle = gfc_vector2d(winddir.x / 1.3, winddir.y / 1.3);

		proj = create_projectile(gfc_vector2d(self->position.x, self->position.y), 1);
		proj->angle = gfc_vector2d(winddir.x * 1.3, winddir.y * 1.3);
	}

	else if ((element == 4) && (self->state == 2))
	{
		self->position.x += 2.0 * winddir.x;
		self->position.y += 2.0 * winddir.y;

		self->hitbox.x = self->position.x;
		self->hitbox.y = self->position.y;
	}

	else if ((element == 5) && (self->state == 2))
	{
		self->position.x -= 2.0 * winddir.x;
		self->position.y -= 2.0 * winddir.y;  

		self->hitbox.x = self->position.x;
		self->hitbox.y = self->position.y;
	}

}