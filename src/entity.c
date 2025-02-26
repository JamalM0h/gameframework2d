#include "simple_logger.h"

#include "entity.h"

typedef struct
{
	Uint32 entity_max;
	Entity *entity_list;

}EntitySystem;

void entity_system_close();

static EntitySystem entity_system = {0};

void entity_system_init(Uint32 maxEnts)
{
	if (entity_system.entity_list)
	{
		slog("cannot have more then one entity manager");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot initialize entity system for zero entities");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!entity_system.entity_list)
	{
		slog("failed to allocate global entity list");
		return;
	}
	entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
}

void entity_system_close()
{
	entity_system_free_all(NULL);
	if (entity_system.entity_list)free(entity_system.entity_list);
	memset(&entity_system, 0, sizeof(entity_system));
	slog("entity system closed");
}

void entity_system_free_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		gf2d_sprite_free(entity_system.entity_list[i].sprite);
		if (entity_system.entity_list[i].free)entity_system.entity_list[i].free(entity_system.entity_list[i].data);
	}
}

Entity *entity_new()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)continue;
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		slog("entity spawned");
		return &entity_system.entity_list[i];
	}
	slog("failed to allocate new entity: list full");
	return NULL; 
}

void entity_free(Entity *self)
{
	if (!self)
	{
		return;
	}
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
	}
	if (self->free)self->free(self->data);
}

void entity_think(Entity *self)
{
	if (!self) return;
	if (self->think)self->think(self);

}

void entity_system_think()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
			entity_think(&entity_system.entity_list[i]);
	}
}

void entity_update(Entity *self)
{
	if (!self) return;
	if (self->update)self->update(self);

}

void entity_system_update()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_update(&entity_system.entity_list[i]);
	}
}

void entity_draw(Entity *self)
{
	if (!self)return;
	if (!self->sprite)return;
	if (self->sprite){
	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	}
}

void entity_collision(Entity *self)
{
	if (!self)return;
	if (!self->hitbox)return;
	if (self->hitbox) {
		int i;
		for (i = 0; i < entity_system.entity_max; i++)
		{
			if (!entity_system.entity_list[i]._inuse)continue;
			if (entity_system.entity_list[i].obj == self->obj)continue;
			if (entity_system.entity_list[i].obj == "projectile")continue;
			if (self->obj == "monster")continue;
			//if (gfc_rect_overlap(*self->hitbox, *entity_system.entity_list[i].hitbox))
			//{
			//	slog(self->obj); 
			//	slog("collide");
			//	slog(entity_system.entity_list[i].obj);
			//}
			if (self->position.x >= entity_system.entity_list[i].position.x && self->position.x <= entity_system.entity_list[i].position.x + entity_system.entity_list[i].width && self->position.y >= entity_system.entity_list[i].position.y && self->position.y <= entity_system.entity_list[i].position.y + entity_system.entity_list[i].height)
			{
				if (self->collide)
					self->collide(self, &entity_system.entity_list[i]);
			}
			else 
			{
				//slog(self->obj); 
				//slog("not collide");
				//slog(entity_system.entity_list[i].obj);
			}
		}
	}
}

void entity_system_draw()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_draw(&entity_system.entity_list[i]);
	}
}

void entity_system_collision()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_collision(&entity_system.entity_list[i]); 
	}
}