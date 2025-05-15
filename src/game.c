#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "entity.h"
#include "player.h"
#include "world.h"
#include "monster.h"
#include "terrain.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0, menu = 0, menue = 0, loaded = 0, selectedent = -1;
    Bool edit = false;   
    const Uint8* keys;
    Sprite *sprite;
    World *world;
    GFC_Vector2D tilecords;  
    
    int mx,my;
    float mf = 0;
    Sprite* mouse, * elemui, * menui, *tileui;
    GFC_Color mouseGFC_Color = gfc_color8(255,100,255,200);
    Entity* player, *octo, *ice, *lava;
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/whiteback.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    tileui = gf2d_sprite_load_all("images/TileIcon.png", 64, 64, 1, 0); 

    player = player_new_entity();
    world = world_load("maps/testworld.json", true);
    //entity_load("maps/testworld.json");  
    //octo = monster_new_entity();   

    slog("press [escape] to quit");
    /*main game loop*/
    while (!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        if (keys[SDL_SCANCODE_RETURN] && (menue == 0) && (menu == 0))
        {
            menu = 1; 
        }
        else if (keys[SDL_SCANCODE_RETURN] && (menue == 1))
        {
            menu = 2;
            selectedent = 0;
        }
        else if (keys[SDL_SCANCODE_RETURN] && menue == 2)
        {
            done = 1;
        }
        if (keys[SDL_SCANCODE_TAB])
        {
            menu = 0;
            loaded = 1;
            edit = false;
            selectedent = -1;
        }

        if ((keys[SDL_SCANCODE_UP]) && (menu == 0))menue = 0; 
        else if (keys[SDL_SCANCODE_LEFT] && (menu == 0))menue = 1;
        else if (keys[SDL_SCANCODE_DOWN] && (menu == 0))menue = 2;

        if (keys[SDL_SCANCODE_KP_1] && menu == 2) {
            selectedent = 0;
        }
        else if (keys[SDL_SCANCODE_KP_2] && menu == 2) {
            selectedent = 1;
        }
        else if (keys[SDL_SCANCODE_KP_3] && menu == 2) {
            selectedent = 2;
        }
        else if (keys[SDL_SCANCODE_KP_4] && menu == 2) {
            selectedent = 3;
        }

        //if (keys[SDL_SCANCODE_DELETE] && menu == 2) { 
         //   delete_entity("maps/testworld.json");
        //}

        if ((keys[SDL_SCANCODE_E] && menu == 2))
        {
            gf2d_sprite_free(world->background); 
            gf2d_sprite_free(world->tileSet); 
            gf2d_sprite_free(world->tileLayer); 
            free(world->tileMap); 
            clear_all_worldcol();
            world = world_load("maps/testworld.json", false);
        }

        if (selectedent == -1)
        {
            mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0); 
        }
        else if (selectedent == 0)
        {
            mouse = gf2d_sprite_load_all("images/ice.png",
                64,
                64,
                1,
                0);
        }
        else if (selectedent == 1)
        {
            mouse = gf2d_sprite_load_all("images/lava.png",
                64,
                64,
                1,
                0);
        }
        else if (selectedent == 2)
        {
            mouse = gf2d_sprite_load_all("images/water.png",
                64,
                64,
                1,
                0);
        }
        else if (selectedent == 3)
        {
            mouse = gf2d_sprite_load_all("images/octo1.png",
                128,
                94,
                16,
                0);
        }

        gf2d_graphics_clear_screen(); 
        
        if (menu == 1 || menu == 2)  
        {
            /*update things here*/
            SDL_GetMouseState(&mx, &my);

        mf += 0.1;
        if (mf >= 16.0)mf = 0;

        if (menu == 1)
        {
            entity_system_think();
        }

        entity_system_update(); 
        entity_system_collision();  

        if (menu == 2)
        {
            tilecords = edit_create(gfc_vector2d(mx, my), selectedent, "maps/testworld.json"); 
        }

        // clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));

        world_draw(world);

        entity_system_draw();

        if (menu == 2)
        {
        gf2d_sprite_draw(
            tileui, 
            gfc_vector2d(tilecords.x * 64, tilecords.y * 64),
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);
        }

        //UI elements last
        gf2d_sprite_draw(
            mouse,
            gfc_vector2d(mx, my),
            NULL,
            NULL,
            NULL,
            NULL,
            NULL, 
            NULL); 

        if (menu == 1)
        {

            if (player->element == 1)
            {
                elemui = gf2d_sprite_load_all("images/fireui.png", 64, 64, 16, 0);
                gf2d_sprite_draw(
                    elemui,
                    gfc_vector2d(player->position.x - player->width / 3.5, player->position.y - player->height * 1.0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player->element == 2)
            {
                elemui = gf2d_sprite_load_all("images/iceui.png", 64, 64, 16, 0);
                gf2d_sprite_draw(
                    elemui,
                    gfc_vector2d(player->position.x - player->width / 3.5, player->position.y - player->height * 1.0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player->element == 4)
            {
                elemui = gf2d_sprite_load_all("images/windui.png", 64, 64, 16, 0);
                gf2d_sprite_draw(
                    elemui,
                    gfc_vector2d(player->position.x - player->width / 3.5, player->position.y - player->height * 1.0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player->element == 5)
            {
                elemui = gf2d_sprite_load_all("images/voidui.png", 64, 64, 16, 0);
                gf2d_sprite_draw(
                    elemui,
                    gfc_vector2d(player->position.x - player->width / 3.5, player->position.y - player->height * 1.0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player->element == 3)
            {
                elemui = gf2d_sprite_load_all("images/electricui.png", 64, 64, 16, 0);
                gf2d_sprite_draw(
                    elemui,
                    gfc_vector2d(player->position.x - player->width / 3.5, player->position.y - player->height * 1.0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
        }

         // render current draw frame and skip to the next frame
        }
        else
        {
            gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0)); 

            if (loaded == 1)
            {
                world_draw(world);

                entity_system_draw();
            }

            if (menue == 0)
            {
                menui = gf2d_sprite_load_all("images/menuS.png", 600, 500, 16, 0);
                gf2d_sprite_draw(
                    menui,
                    gfc_vector2d(280, 90),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            
            if (menue == 1)
            {
                menui = gf2d_sprite_load_all("images/menuEd.png", 600, 500, 16, 0);
                gf2d_sprite_draw(
                    menui,
                    gfc_vector2d(280, 90),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }

            if (menue == 2)
            {
                menui = gf2d_sprite_load_all("images/menuEx.png", 600, 500, 16, 0);
                gf2d_sprite_draw(
                    menui,
                    gfc_vector2d(280, 90),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
        }
        if (keys[SDL_SCANCODE_1])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
        gf2d_graphics_next_frame(); 
    }
    entity_free(player);
    //entity_free(octo);
    world_free(world);  

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
