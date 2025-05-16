#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void barrel_think(Entity* self);
void barrel_update(Entity* self);
void barrel_free(Entity* self);
void barrel_damage(Entity* self, int element, GFC_Vector2D winddir);
void barrel_collide(Entity* self, Entity* collide); 

Entity* barrel_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/barrel.png",
		72,
		64,
		16,
		0);

	self->obj = "barrel";

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 72, 64);
	GFC_Vector2D dirb = { 0 }; 

	self->hitbox = rect;

	self->think = barrel_think;
	self->update = barrel_update;
	self->free = barrel_free;
	self->damage = barrel_damage;
	self->collide = barrel_collide;

	self->dirb = dirb; 

	self->height = 64;
	self->width = 64;

	self->explode = false;

	self->state = 1;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	return self;
}

void barrel_think(Entity* self)
{
	if (!self)return;

	gfc_vector2d_scale(self->velocity, self->dirb, 3);
	gfc_vector2d_add(self->position, self->position, self->velocity);

	if (self->position.y <= 720 + self->height)
	{
		self->dirb.y += 0.01;
	}
	
}
void barrel_update(Entity* self)
{
	if (!self)return;

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

void barrel_free(Entity* self)
{
	if (!self)return;
	if(self->explode == true) 
	explosion_entity(gfc_vector2d(self->position.x - 128, self->position.y - 128)); 
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

void barrel_damage(Entity* self, int element, GFC_Vector2D winddir)
{
	if (!self)return;

	if (element == 1)
	{
		self->sprite = NULL; 

		eraser_entity(gfc_vector2d(self->position.x, self->position.y), NULL, NULL, true); 

		self->explode = true; 

		barrel_free(self); 
	}
}

void barrel_collide(Entity* self, Entity* collide)
{
	if (!self)return;

	if (collide->obj == "worldcol" || (collide->obj == "ice" && collide->state == 1) || (collide->obj == "lava") && (collide->state == 2) || collide->obj == "metal" || collide->obj == "stone" || collide->obj == "barrel" || collide->obj == "gate")
	{
		if (self->position.y + self->height / 1.1 < collide->position.y)
		{
			if (self->position.y >= collide->position.y - self->height)
			{
				self->position.y = collide->position.y - self->height - 0.5;
			}
			self->dirb.y = 0;
			//self->velocity.y = 0;
		}
		else
		{
			if (self->position.x < collide->position.x)
			{
				self->position.x = collide->position.x - self->width * 1.2;
				self->dirb.x *= -0.75;
			}
			if (self->position.x + self->width > collide->position.x + collide->width)
			{
				self->position.x = collide->position.x + collide->width * 1.2;
				self->dirb.x *= -0.75;
			}
		}
	}

	if (collide->obj == "player")
	{
		if (collide->position.x < self->position.x + (self->width / 2))
			self->dirb.x += 0.1;
		else if (collide->position.x + collide->width > self->position.x + (self->width / 2 ))
			self->dirb.x -= 0.1;
	}

	else
	{
		if (self->dirb.x > 0.05)
		{
			self->dirb.x -= 0.01;
		}
		else if (self->dirb.x < -0.05)
		{
			self->dirb.x += 0.01;
		}
		else
			self->dirb.x = 0;
	}

	if ((collide->obj == "water") && (collide->state == 1))
	{
		self->dirb.y = -1;
	}
}