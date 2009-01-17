#include <stdio.h>
#include <SDL.h>

#include "timers.h"
#include "render.h"
#include "eventloop.h"


int main(int argc, char *argv[]) {
    printf("======================================================\n"
           " AWESOME TOWER DEFENSE\n"
           "             by Trygve Vea <trygve.vea@gmail.com>\n"
           "======================================================\n");
    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
        printf("SDL_Init failed : %s\n", SDL_GetError());
        return 1;
    }
    printf("* Initializing Video...\n");
    init_video();
    printf("* Initializing Timers...\n");
    init_timers();
    printf("* Entering Eventloop...\n");
    event_loop();
    return 0;
}
