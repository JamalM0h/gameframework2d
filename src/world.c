#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_graphics.h"

#include "world.h"
#include "terrain.h"
#include "monster.h"


void world_tile_layer_build(World *world)
{
	int i, j;
	GFC_Vector2D position;
	Uint32 frame;
	Uint32 index;

	if (!world)return;

	if (!world->tileSet)return;

	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileSet->frame_w, 
		world->tileHeight * world->tileSet->frame_h);

	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;

	if (!world->tileLayer->surface)
	{
		slog("failed to create tileLayer surface");
		return;
	}

	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0)continue;

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("Failed to convert world tile layer to texture");
		return;
	}
}

World* world_load(const char* filename, Bool loadent)
{
	World *world = NULL;
	SJson *json = NULL;
	SJson *wjson = NULL; 
	SJson* vertical, * horizontal;
	SJson *item;
	int tile;
	int w = 0, h = 0;
	int i, j;
	const char* tileSet;
	const char* background;
	int frame_w, frame_h;
	int frames_per_line;
	if (!filename)
	{
		slog("no filename provided for world_load");
		return NULL;
	}

	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json,"world");
	if (!wjson)
	{
		slog("%s missing 'world object", filename);
		return NULL;
		sj_free(json); 
	}
	vertical = sj_object_get_value(wjson, "tileMap");
	if (!vertical)
	{
		slog("%s missing 'tileMap object", filename);
		sj_free(json);
		return NULL;
	}

	h = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical, 0);
	w = sj_array_get_count(horizontal);
	world = world_new(w, h); 
	if (!world)
	{
		slog("failed to create space for a new world for file %s", filename);
		sj_free(json);
		return NULL; 
	}
	for (j = 0; j < h; j++)
	{
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal)continue;
		for (i = 0; i < w; i++)
		{
			item = sj_array_get_nth(horizontal, i);
			if (!item)continue;
			tile = 0;
			sj_get_integer_value(item, &tile); 
			world->tileMap[i + (j * w)] = tile;
			if(tile != 0)
			world_col_new_entity(gfc_vector2d(i * 64, j * 64)); 
		}
	}

	background = sj_object_get_value_as_string(wjson, "background");
	world->background = gf2d_sprite_load_image(background);

	tileSet = sj_object_get_value_as_string(wjson, "tileSet");

	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_w", &frame_h);
	sj_object_get_value_as_int(wjson, "frame_per_line", &frames_per_line);

	world->tileSet = gf2d_sprite_load_all(
		tileSet,
		frame_w,
		frame_h,
		frames_per_line,
		1);

	world_tile_layer_build(world);

	if(loadent == true)
	entity_load(filename); 

	sj_free(json);
	return world;
}


Entity* entity_load(const char* filename)
{
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* entity, * horizontalent;
	SJson* item;
	Entity* hold;
	int w = 0, h = 0;
	int i, j;
	int x, y, ent, index;
	if (!filename)
	{
		slog("no filename provided for entity_load");
		return NULL;
	}
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s missing 'world object", filename);
		return NULL;
		sj_free(json);
	}
	entity = sj_object_get_value(wjson, "entitys");
	if (!entity)
	{
		slog("%s missing 'entity object", filename);
		sj_free(json);
		return NULL;
	}
	h = sj_array_get_count(entity);
	for (j = 0; j < h; j++)
	{
		horizontalent = sj_array_get_nth(entity, j);

		item = sj_array_get_nth(horizontalent, 0);
		sj_get_integer_value(item, &ent);

		item = sj_array_get_nth(horizontalent, 1);
		sj_get_integer_value(item, &x);

		item = sj_array_get_nth(horizontalent, 2);
		sj_get_integer_value(item, &y);

		item = sj_array_get_nth(horizontalent, 3);
		sj_get_integer_value(item, &index); 

		if (ent == 0)
		{
			hold = ice_new_entity(gfc_vector2d(x, y), false);
			hold->index = index;  
		}
		
		else if (ent == 1)
		{
			hold = lava_new_entity(gfc_vector2d(x, y), false);
			hold->index = index; 
		}

		else if (ent == 2)
		{
			hold = water_new_entity(gfc_vector2d(x, y), false);
			hold->index = index; 
		}

		else if (ent == 3)
		{
			hold = monster_new_entity(gfc_vector2d(x, y));
			hold->index = index; 
		}
	}

	sj_free(json);
	return;
}

void save_entity(const char* filename, int entid, GFC_Vector2D pos, Entity* ent) 
{
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* entity;
	SJson* item, * val;
	int length;
	if (!filename)
	{
		slog("no filename provided for entity_load");
		return NULL;
	}
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s missing 'world object", filename);
		return NULL;
		sj_free(json);
	}
	entity = sj_object_get_value(wjson, "entitys");
	if (!entity)
	{
		slog("%s missing 'entity object", filename);
		sj_free(json);
		return NULL;
	}
	
	item = sj_array_new(); 

	sj_array_append(entity, item);

	length = sj_array_count(entity); 

	val = sj_new_int(entid);

	sj_array_append(item, val);

	val = sj_new_int(pos.x); 

	sj_array_append(item, val);

	val = sj_new_int(pos.y); 

	sj_array_append(item, val);

	val = sj_new_int(length);

	sj_array_append(item, val); 

	ent->index = length; 

	sj_save(json, filename);    

	slog("entity saved");

	sj_free(json);
	return;
}

void delete_entity(const char* filename, Entity* ent)
{
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* entity, * horizontalent;
	SJson* item, * index;
	SJson* arrayd, * oldVal; 
	int h, j, i;
	int uindex;

	if (!filename)
	{
		slog("no filename provided for entity_load");
		return NULL;
	}
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s missing 'world object", filename);
		return NULL;
		sj_free(json);
	}
	entity = sj_object_get_value(wjson, "entitys");
	if (!entity)
	{
		slog("%s missing 'entity object", filename);
		sj_free(json);
		return NULL;
	}

	h = sj_array_get_count(entity);
 
	for (j = 0; j < h; j++) {

		horizontalent = sj_array_get_nth(entity, j);
		index = sj_array_get_nth(horizontalent, 3);

		sj_get_integer_value(index, &uindex);  

		if (uindex == ent->index)
		{
			sj_array_delete_nth(entity, j); 
			slog("entity deleted"); 
		}
	}
	
	sj_save(json, filename); 

	sj_free(json);
	return;
}

void toggle_tile(const char* filename, int row, int column)
{
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* tileMap, * horizontaltile;
	SJson* tile, * act, * newtilemap, * newtiles, * val;
	int tileval;
	int j, i, h, w;

	if (!filename)
	{
		slog("no filename provided for entity_load");
		return NULL;
	}
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s missing 'world object", filename);
		return NULL;
		sj_free(json);
	}
	tileMap = sj_object_get_value(wjson, "tileMap");
	if (!tileMap)
	{
		slog("%s missing 'tileMap object", filename);
		sj_free(json);
		return NULL;
	}

	h = sj_array_get_count(tileMap);

	horizontaltile = sj_array_get_nth(tileMap, row);

	w = sj_array_get_count(horizontaltile); 

	newtilemap = sj_array_new();

	for (j = 0; j < h; j++)
	{
		horizontaltile = sj_array_get_nth(tileMap, 0);
		newtiles = sj_array_new();
		if (!horizontaltile)continue;
		for (i = 0; i < 19; i++)
		{
			tile = sj_array_get_nth(horizontaltile, i);
			sj_get_integer_value(tile, &tileval);
			if ((row - j == 0) && (i == column))
				
			{
				if (tileval == 0)
				{
					val = sj_new_int(1);
				}
				else
				{
					val = sj_new_int(0);
				}
			}
			else
			{
				val = sj_new_int(tileval);
			}
			sj_array_append(newtiles, val); 
		}
		sj_array_delete_nth(tileMap, 0);
		sj_array_append(tileMap, newtiles); 
	}

	sj_save(json, filename);  

	sj_free(json); 
	return;
}

World *world_test_new()
{
	int i, j;
	int width = 75, height = 45;
	World *world;

	world = world_new(width, height);

	if (!world)return NULL; 

	world->background = gf2d_sprite_load_image("images/backgrounds/whiteback.png"); 
	world->tileSet = gf2d_sprite_load_all(
		"images/TileSet.png",
		16,
		16,
		1,
		1);
	for (i = 0; i < width; i++)
	{
		world->tileMap[i] = 1;
		world->tileMap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++)
	{
		world->tileMap[i * width] = 1;
		world->tileMap[i * width + (width - 1)] = 1;
	}
	world_tile_layer_build(world);
	return world;
}

World *world_new(Uint32 width, Uint32 height)
{
	World* world;

	if ((!width) || (!height))
	{
		slog("cannot make a world with zero width and height");
		return NULL;
	}

	world = gfc_allocate_array(sizeof(World), 1);
	if (!world)
	{
		slog("failed to allocate a new world");
		return NULL;
	}
	world->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tileHeight = height;
	world->tileWidth = width;
	return world;
}

void world_free(World* world)
{
	if (!world)return;

	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	gf2d_sprite_free(world->tileLayer);
	free(world->tileMap);
	free(world);
}

void world_draw(World *world)
{
	if (!world)return;
	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0)); 
	if (!world->tileSet)return;
	gf2d_sprite_draw_image(world->tileLayer, gfc_vector2d(0, 0));
}
