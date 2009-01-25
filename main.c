#include <stdio.h>
#include <SDL.h>

#include "timers.h"
#include "render.h"
#include "eventloop.h"
#include "sprites.h"


int main(int argc, char *argv[]) {
    printf("======================================================\n"
           " AWESOME TOWER DEFENSE\n"
           "             by Trygve Vea <trygve.vea@gmail.com>\n"
           "======================================================\n");
    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
        printf("SDL_Init failed : %s\n", SDL_GetError());
        return 1;
    }
    init_video();
    init_timers();
    init_sprites();
    event_loop();
    return 0;
}
