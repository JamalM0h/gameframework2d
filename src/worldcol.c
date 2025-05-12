#include "simple_logger.h"

#include "projectile.h"
#include "terrain.h"

void world_col_free(Entity* self);

Entity* world_col_new_entity(GFC_Vector2D pos)
{
	Entity* self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new obj");
		return NULL;
	}

	self->sprite = NULL;

	self->obj = "worldcol";

	self->position = gfc_vector2d(pos.x, pos.y);

	GFC_Rect rect = gfc_rect(pos.x, pos.y, 64, 64);

	self->hitbox = rect;

	self->free = world_col_free;

	self->height = 64;
	self->width = 64;

	return self;
}

void world_col_free(Entity* self)
{
	if (!self)return;
	//if (self->free)self->free(self);
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
	}
	memset(self, 0, sizeof(Entity));
}
