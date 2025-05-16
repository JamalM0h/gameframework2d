#include "simple_logger.h"

#include "player.h"
#include "projectile.h"
#include "terrain.h"
#include "monster.h"
#include "world.h"

void player_think(Entity *self);
void player_update(Entity* self);
void player_free(Entity* self);
void player_collide(Entity* self, Entity *collide);

GFC_Vector2D dir = { 0 }; 
int jumps = 0;
int row = 0, column = 0;
int iframes = 200;

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
	self->position = gfc_vector2d(100, 100);

	GFC_Rect rect = gfc_rect(0, 0, 45, 65);

	self->hitbox = rect;  

	self->think = player_think;
	self->update = player_update;
	self->free = player_free;
	self->collide = player_collide;

	self->height = 65;
	self->width = 45;

	self->roomnum = 505;

	self->ice = false;
	self->electric = false;
	self->wind = false;
	self->vo = false;

	self->health = 5;

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

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && jumps > 0)
		{
			dir.y = -3.0;
			jumps -= 1;
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
		{
			if (self->element == 1 && self->ice == true)
				self->element = 2;
			else if (self->element < 3 && self->electric == true)
				self->element = 3;
			else if (self->element < 4 && self->wind == true)
				self->element = 4;
			else if (self->element < 5 && self->vo == true)
				self->element = 5;
			else if (self->element == 5)
				self->element = 1;
			else
				self->element = 1;
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_b)
		{
			if(self->element == 1)
			lava_new_entity(gfc_vector2d(mx - 32, my - 32), true);
			else if(self->element == 2)
			ice_new_entity(gfc_vector2d(mx - 32, my - 32), true);
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

		if(event.type == SDL_MOUSEBUTTONDOWN)
			if (event.button.button == SDL_BUTTON_LEFT)
					create_projectile(self->position, self->element);
	}  

	gfc_vector2d_scale(self->velocity, dir, 3); 
	gfc_vector2d_add(self->position, self->position, self->velocity);

	if (self->position.y < 0 - self->height)
	{
		self->position.y = 720;
		jumps = 3;
		self->roomnum -= 10;
	}
	if (self->position.y <= 720 + self->height)
	{
		dir.y += 0.1;
	}
	else if (self->position.y > 720 + self->height)
	{
		self->position.y = 5 - self->height;
		dir.y = 0.5;
		jumps = 3;
		self->roomnum += 10; 
	}
	if (self->position.x < 0 - self->width)
	{
		self->position.x = 1200;
		self->roomnum -= 1;
	}
	else if (self->position.x > 1200)
	{
		self->position.x = 0;
		self->roomnum += 1;
	}
}
void player_update(Entity* self)
{
	if (!self)return;  

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;

	if (iframes < 200)
	{
		iframes += 1;
	}

	if (self->health <= 0)
	{
		self->position.x = 100;
		self->position.y = 100;
		self->health = 5;
		self->roomnum = 505;

	}

}
void player_free(Entity* self)
{
	if (!self)return;
}

void player_collide(Entity* self, Entity* collide)
{
	Bool headbonk = false; 
	if (!self)return;

	if (collide->obj == "worldcol" || (collide->obj == "ice" && collide->state == 1) || (collide->obj == "lava") && (collide->state == 2) || collide->obj == "metal" || collide->obj == "stone" || collide->obj == "barrel")
	{
		if (self->position.y + self->height / 1.2 < collide->position.y)
		{
			if (self->position.y >= collide->position.y - self->height)
			{
				self->position.y = collide->position.y - self->height;
			}
			dir.y = 0;
			//self->velocity.y = 0;
			jumps = 3;
		}
		else if (self->position.y + self->height / 2 > collide->position.y + collide->height)
		{
			dir.y = 0.25;
		}
		else if (self->position.x < collide->position.x)
		{
			if (collide->obj != "barrel")
			{
				self->position.x = collide->position.x - self->width * 1.1;
			}
			dir.x = 0;
			
		}
		else if (self->position.x + self->width > collide->position.x + collide->width)
		{
			if (collide->obj != "barrel")
			{
				self->position.x = collide->position.x + collide->width * 1.1;
			}
			dir.x = 0;
			
		}
		else
		{
			self->position.y = collide->position.y - self->height;
			dir.y = 0;
		}
	}
	if (collide->obj == "gate" && collide->state != 2)
	{
		if (self->position.y + self->height / 1.2 < collide->position.y)
		{
			if (self->position.y >= collide->position.y - self->height)
			{
				self->position.y = collide->position.y - self->height;
			}
			dir.y = 0;
			//self->velocity.y = 0;
			jumps = 3;
		}
		else if (self->position.x < collide->position.x)
		{
			self->position.x = collide->position.x - self->width * 1.1;
			dir.x = 0;
		}
		else if (self->position.x + self->width > collide->position.x + collide->width)
		{
			self->position.x = collide->position.x + collide->width * 1.1;
			dir.x = 0;
		}
	}
	if ((collide->obj == "lava" && collide->state == 1) || (collide->obj == "water" && collide->state == 2) || (collide->obj == "metal" && collide->state == 2) && iframes >= 200)
	{
		self->health -= 1;
		iframes = 0;
		dir.x *= -1;
		dir.y = -3;

	}

	else if ((collide->obj == "water") && (collide->state == 1))
	{
		dir.y = -1;
	}
}

GFC_Vector2D edit_create(GFC_Vector2D pos, int entityid, const char* filename, int roomnum) 
{
	SDL_Event event;
	Entity* ent;
	Sprite* tile; 

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
			if (event.button.button == SDL_BUTTON_LEFT)
				if (entityid == 0)
				{
					ent = ice_new_entity(gfc_vector2d(pos.x, pos.y), false);
					save_entity(filename, 0, gfc_vector2d(pos.x, pos.y), ent, roomnum);

				}
				else if (entityid == 1)
				{
					ent = lava_new_entity(gfc_vector2d(pos.x, pos.y), false); 
					save_entity(filename, 1, gfc_vector2d(pos.x, pos.y), ent, roomnum);

				}
				else if (entityid == 2)
				{
					ent = water_new_entity(gfc_vector2d(pos.x, pos.y), false); 
					save_entity(filename, 2, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 4)
				{
					ent = monster_new_entity(gfc_vector2d(pos.x, pos.y)); 
					save_entity(filename, 4, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 3)
				{
					ent = metal_new_entity(gfc_vector2d(pos.x, pos.y)); 
					save_entity(filename, 3, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 5)
				{
					ent = barrel_new_entity(gfc_vector2d(pos.x, pos.y)); 
					save_entity(filename, 5, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 6)
				{
					ent = stone_new_entity(gfc_vector2d(pos.x, pos.y));
					save_entity(filename, 6, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 7)
				{
					ent = collect_new_entity(gfc_vector2d(pos.x, pos.y), 1); 
					save_entity(filename, 7, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 8)
				{
					ent = collect_new_entity(gfc_vector2d(pos.x, pos.y), 2);
					save_entity(filename, 8, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 9)
				{
					ent = collect_new_entity(gfc_vector2d(pos.x, pos.y), 3);
					save_entity(filename, 9, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 10)
				{
					ent = collect_new_entity(gfc_vector2d(pos.x, pos.y), 4);
					save_entity(filename, 10, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
				else if (entityid == 11)
				{
					ent = eletricg_new_entity(gfc_vector2d(pos.x, pos.y)); 
					save_entity(filename, 11, gfc_vector2d(pos.x, pos.y), ent, roomnum);
				}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			eraser_entity(gfc_vector2d(pos.x + 32, pos.y + 32), filename, roomnum, false); 
			//water_new_entity(gfc_vector2d(pos.x, pos.y), false);
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				if (row > 0)
					row -= 1;
				else
					row = 11;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				if (row < 11)
					row += 1;
				else
					row = 0;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				if (column > 0)
					column -= 1;
				else
					column = 18;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				if (column < 18)
					column += 1;
				else
					column = 0;
			}
			else if (event.key.keysym.sym == SDLK_e)
			{
				toggle_tile(filename, row, column, roomnum);
			}
		}
	}

	return gfc_vector2d(column, row);
}