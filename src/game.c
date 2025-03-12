#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "entity.h"
#include "player.h"
#include "monster.h"
#include "terrain.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite* mouse, *elemui;
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

    player = player_new_entity();
    octo = monster_new_entity();  

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        entity_system_think();
        entity_system_update();
        entity_system_collision();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            
            entity_system_draw();

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);
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


        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    entity_free(octo);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
