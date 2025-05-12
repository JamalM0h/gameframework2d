#include "simple_logger.h"

#include "terrain.h"

void cursor_update(Entity* self);
void cursor_free(Entity* self);
void cursor_collide(Entity* self, Entity* collide); 

int eraserlive = 50;

Entity* eraser_entity(GFC_Vector2D pos)  
{
	Entity* self; 

	self = entity_new();

	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}

	self->obj = "eraser";

	self->frame = 0; 
	self->position = gfc_vector2d(pos.x, pos.y); 

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 4, 4);

	self->hitbox = rect; 

	self->update = cursor_update;
	self->free = cursor_free; 
	self->collide = cursor_collide; 

	self->height = 32;
	self->width = 32;

	self->lifetime = eraserlive;

	return self;
}

void cursor_update(Entity* self)
{

	if (!self)return;

	if (self->lifetime <= eraserlive)
	{
		self->lifetime -= 0.10;
	}
	if (self->lifetime <= 0)
	{
		cursor_free(self);
	}

	//cursor_free(self);

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

void cursor_free(Entity* self)
{
	if (!self)return;
	memset(self, 0, sizeof(Entity));
}

void cursor_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if (collide->obj == "lava" || collide->obj == "ice" || collide->obj == "water" || collide->obj == "monster")
	{
		collide->free(collide);  
		slog("removed obj");
	}
}

