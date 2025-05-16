#include "simple_logger.h"

#include "monster.h"

void monster_think(Entity* self);
void monster_update(Entity* self);
void monster_free(Entity* self);
void monster_damage(Entity* self, int damage, GFC_Vector2D winddir);
void monster_collide(Entity* self, Entity* collide);

Entity* monster_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new monster entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/octo1.png",
		128,
		94,
		16,
		0);

	self->obj = "monster"; 

	self->frame = 0;
	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 128, 94); 

	self->hitbox = rect;

	self->health = 5;

	self->height = 64;
	self->width = 128;

	self->think = monster_think;
	self->update = monster_update;
	self->free = monster_free;
	self->damage = monster_damage;

	self->collide = monster_collide;

	if (!self->sprite)
	{
		slog("no sprite");
	}

	//slog("monster spawned");
	return self;
}

void monster_think(Entity* self)
{
	if (!self)return;

	self->position.x += 3;
	if (self->position.x >= 1200)
	{
		self->position.x = -100;
	}

	if (self->health <= 0)
		monster_free(self);
}
void monster_update(Entity* self)
{
	if (!self)return;

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;

	//self->position.x = self->hitbox->x;
	//self->position.y = self->hitbox->y;

}
void monster_free(Entity* self)
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

void monster_damage(Entity* self, int damage, GFC_Vector2D winddir)
{
	if (!self)return;
	if (!&self->health)return;
	self->health -= damage;
}

void monster_collide(Entity* self, Entity* collide)
{
	if(!self)return;
	if ((collide->obj == "ice" || collide->obj == "metal") && collide->state == 1)
	{
		self->position.x -= 3;
	}
}