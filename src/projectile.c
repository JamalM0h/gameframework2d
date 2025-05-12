#include "simple_logger.h"

#include "projectile.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
void projectile_free(Entity* self);
void projectile_collide(Entity* self, Entity *collide); 

Entity* create_projectile(GFC_Vector2D playerpos, int element)
{
	Entity* self;
	int mx = 0, my = 0; 

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new projectile");
		return NULL;
	}

	if (element == 1)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/fireball.png",
			128,
			128,
			16,
			0);

		self->element = 1;
	}

	else if (element == 2)
	{

		self->sprite = gf2d_sprite_load_all(
			"images/iceball.png",
			128,
			128,
			16,
			0);

		self->element = 2;
	}

	else if (element == 4)
	{

		self->sprite = gf2d_sprite_load_all(
			"images/wind.png",
			128,
			128,
			16,
			0);

		self->element = 4;
	}

	else if (element == 5)
	{

		self->sprite = gf2d_sprite_load_all(
			"images/void.png",
			128,
			128,
			16,
			0);

		self->element = 5;
	}

	else if (element == 3)
	{

		self->sprite = gf2d_sprite_load_all(
			"images/electricball.png",
			128,
			128,
			16,
			0);

		self->element = 3;
	}

	self->obj = "projectile";

	self->frame = 0;

	SDL_GetMouseState(&mx, &my); 

	self->position = playerpos;

	self->position.x = playerpos.x - 10;

	self->angle.x = mx - playerpos.x;
	self->angle.y = my - playerpos.y - 10;

	gfc_vector2d_set_magnitude(&self->angle, 10);

	GFC_Rect rect = gfc_rect(0, 0, 32, 32); 

	self->hitbox = rect; 

	self->lifetime = 0;

	//self->angle = gfc_vector2d_get_normal(self->angle); 

	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;
	self->collide = projectile_collide;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void projectile_think(Entity* self)
{
	if (!self)return;
}
void projectile_update(Entity* self)
{
	if (!self)return;

	self->position.x += (self->angle.x);
	self->position.y += (self->angle.y);

	self->hitbox.x = self->position.x + 17;
	self->hitbox.y = self->position.y + 15;

	self->lifetime += 1;
	if (self->lifetime >= 120)
	{
		projectile_free(self);
	}
}
void projectile_free(Entity* self)
{
	if (!self)return;
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite); 
	}
	memset(self, 0, sizeof(Entity));
}

void projectile_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if (collide->obj == "lava" || collide->obj == "ice" || collide->obj == "water")
	{
		collide->damage(collide, self->element, self->angle);
		if (collide->obj == "lava" && (self->element == 3 || self->element == 4))
		{
			self->free(self);
		}
	}
	else if (collide->obj == "worldcol")
	{
		self->free(self);
	}
	else if (!collide->health)return;
	else
	{
		collide->damage(collide, 1, gfc_vector2d(0,0));
		self->free(self);
	}
}