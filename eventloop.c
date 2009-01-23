#include <SDL.h>

#include "timers.h"
#include "render.h"
#include "eventloop.h"
#include "cursor.h"
#include "sprites.h"
#include "gfx_buttons.h"
#include "level.h"

int gamecycle = 0;

void event_loop(void) {
    SDL_Event eventqueue;
    int quit = 0;
    while(1) {
        while(SDL_WaitEvent(&eventqueue)) {
            switch(eventqueue.type) {
                case SDL_QUIT:
                    return;
                    quit = 1;
                break;
                case SDL_MOUSEMOTION:
                    update_cursor(eventqueue.motion.x,eventqueue.motion.y);
                break;
                case SDL_MOUSEBUTTONDOWN:
                    if ( (eventqueue.button.x/RECTSIZE_X) > 0 && (eventqueue.button.x/RECTSIZE_X) < 19 && (eventqueue.button.y/RECTSIZE_Y) > 0 && (eventqueue.button.y/RECTSIZE_Y) < 13 ) {
                        if ( !is_path((eventqueue.button.x/RECTSIZE_X),(eventqueue.button.y/RECTSIZE_Y)) ) {
                            add_tower((eventqueue.button.x/RECTSIZE_X),(eventqueue.button.y/RECTSIZE_Y),0);
                        } else {
                            printf("Trying to build a tower on a path?\n");
                        }
                    } else if ( eventqueue.button.button == SDL_BUTTON_LEFT ) 
                        press_button(eventqueue.button.x, eventqueue.button.y);
                break;
                case SDL_MOUSEBUTTONUP:
                break;
                case SDL_USEREVENT:
                    switch(eventqueue.user.code) {
                        case TIMER_REPORTFPS:
                            getrenders();
                        break;
                        case TIMER_GAMECYCLE:
                            move_projectile();
                            move_monster();
                            if ( (gamecycle % 20) == 0 ) animate_sprites();
                            if ( (gamecycle % 10) == 0 ) shoot_towers();
                            if ( (gamecycle % 100) == 0 ) spawn_monster();
                            if ( gamecycle == 100 ) gamecycle = 0;
                            gamecycle++;
                        break;
                        case TIMER_RENDER:
                            render();
                        break;
                    }
                break;
                default:
                    printf("WARN: Unhandled event (type: %d)!\n", eventqueue.type);
                break;
            }
        }
        render();
        if ( quit == 1 ) return;
    }
}
