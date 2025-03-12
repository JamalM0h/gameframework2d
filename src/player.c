#include "simple_logger.h"

#include "player.h"
#include "projectile.h"
#include "terrain.h"

void player_think(Entity *self);
void player_update(Entity* self);
void player_free(Entity* self);
void player_collide(Entity* self, Entity *collide);

GFC_Vector2D dir = { 0 }; 

Entity *player_new_entity()
{
	Entity *self;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all( 
			"images/BlackMage2.gif",
			45,
			65,
		0,
		0);

	self->obj = "player";  

	self->frame = 0;
	self->position = gfc_vector2d(0, 0);

	GFC_Rect rect = gfc_rect(0, 0, 45, 65);

	self->hitbox = rect;  

	self->think = player_think;
	self->update = player_update;
	self->free = player_free;
	self->collide = player_collide;

	self->height = 65;
	self->width = 45;

	self->element = 1;
	
	if (!self->sprite)
	{
		slog("no sprite");
	}

	slog("player spawned");
	return self;
}

void player_think(Entity* self)
{
	int mx = 0, my = 0;
	SDL_GetMouseState(&mx, &my);
	SDL_Event event;
	Entity* proj;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_a)
			{
				dir.x = -1;
				self->sprite = gf2d_sprite_load_all(
					"images/BlackMage2.gif",
					45,
					65,
					0,
					0);
			}

			if (event.key.keysym.sym == SDLK_d)
			{
				dir.x = 1;
				self->sprite = gf2d_sprite_load_all(
					"images/BlackMage1.gif",
					45,
					65,
					0,
					0);
			}
		}
	
		if (event.type == SDL_KEYUP)
			if (dir.x == -1 && event.key.keysym.sym == SDLK_d)
				continue;
			else if (dir.x == 1 && event.key.keysym.sym == SDLK_a)
				continue;
			else if (event.key.keysym.sym == SDLK_SPACE)
				continue;
			else if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_a)
				dir.x = 0;

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
			dir.y = -4;

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
		{
			if (self->element == 1)
				self->element = 2;
			else if (self->element == 2)
				self->element = 3;
			else if (self->element == 3)
				self->element = 4;
			else if (self->element == 4)
				self->element = 5;
			else if (self->element == 5)
				self->element = 1;

			slog("changed element");
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_b)
		{
			if(self->element == 1)
			lava_new_entity(gfc_vector2d(mx, my), true);
			else if(self->element == 2)
			ice_new_entity(gfc_vector2d(mx, my), true);
			else if (self->element == 3)
			{
				proj = create_projectile(gfc_vector2d(mx + gfc_random_int(150) - 75, my), self->element);
				proj->sprite = gf2d_sprite_load_all(
					"images/electricbolt.png",
					128,
					128,
					16,
					0);
				proj->angle = gfc_vector2d(0, 20);
			}
		}

		else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_b)
		{
			if (self->element == 4 || self->element == 5)
			{
				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(0, -2);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(2, 0);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(-2, 0);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(0, 2);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(2, 2);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(-2, 2);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(2, -2);

				proj = create_projectile(gfc_vector2d(mx, my), self->element);
				proj->angle = gfc_vector2d(-2, -2);
			}
			
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_o)
		{
			water_new_entity(gfc_vector2d(mx, my), true);
		}

		if(event.type == SDL_MOUSEBUTTONDOWN)
			if (event.button.button == SDL_BUTTON_LEFT)
					create_projectile(self->position, self->element);
	}  

	gfc_vector2d_scale(self->velocity, dir, 3); 
	gfc_vector2d_add(self->position, self->position, self->velocity);

	if (self->position.y <= 655)
	{
		dir.y += 0.1;
	}
	else
	{
		self->position.y = 655;
		dir.y = 0;
		self->velocity.y = 0;
	}
	
	if (self->position.x < 0)
		self->position.x = 0;

	if (self->position.x > 1155)
		self->position.x = 1155;

	if (self->position.y < 0)
	{
		self->position.y = 0;
		dir.y = 0;
	}
}
void player_update(Entity* self)
{
	if (!self)return;  

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;

}
void player_free(Entity* self)
{
	if (!self)return;
}

void player_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	slog("player collided");
	if (collide->obj == "ice" && collide->state == 1)
	{
		if (self->position.y < collide->position.y)
		{
			self->position.y = collide->position.y - self->height;
			dir.y = 0;
			//self->velocity.y = 0;
		}
		else if (self->position.y + self->height > collide->position.y + collide->height)
		{
			dir.y = 1;
		}
		//if (self->position.y + (self->height / 1.10) > collide->position.y && self->position.y + (self->height / 1.10) < collide->position.y + collide->height || self->position.y + (self->width / 8.0) > collide->position.x && self->position.x + (self->width / 8.0) < collide->position.x + collide->width)
		//{
		//	dir.x *= -1;
		//}
	}
	if ((collide->obj == "lava" && collide->state == 1) || (collide->obj == "water" && collide->state == 2))
	{
		dir.x *= -1;
		dir.y = -4;

	}
	else if((collide->obj == "lava") && (collide->state == 2))
	{
		if (self->position.y < collide->position.y)
		{
			self->position.y = collide->position.y - self->height;
			dir.y = 0;
			//self->velocity.y = 0;
		}
		else if (self->position.y + self->height > collide->position.y + collide->height)
		{
			dir.y = 1;
		}
	}

	else if ((collide->obj == "water") && (collide->state == 1))
	{
		dir.y = -1;
	}
}