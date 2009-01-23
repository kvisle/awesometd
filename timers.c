#include <SDL.h>

#include "timers.h"

static SDL_TimerID render_id, report_fps_id, game_cycle_id;

static SDL_Event render_t, report_fps_t, game_cycle_t;

void init_timers(void) {
    render_t.type = SDL_USEREVENT;
    render_t.user.code = TIMER_RENDER;
    render_id = SDL_AddTimer(1000/60, timedrenderevent, &render_t);

    report_fps_t.type = SDL_USEREVENT;
    report_fps_t.user.code = TIMER_REPORTFPS;
    report_fps_id = SDL_AddTimer(1000, timeduserevent, &report_fps_t);

    game_cycle_t.type = SDL_USEREVENT;
    game_cycle_t.user.code = TIMER_GAMECYCLE;
    game_cycle_id = SDL_AddTimer(10, timeduserevent, &game_cycle_t);
}

static int waitingforrender = 0;

Uint32 timedrenderevent(Uint32 interval, void* param) {
    SDL_Event *event = (SDL_Event*)param;
    if ( waitingforrender == 0 ) {
        SDL_PushEvent(event);
        waitingforrender = 1;
    }
    else {
//        printf("Dropping frame, since we still haven't rendered the previous one. WE DO NOT WANT TO MAKE A RENDERQUEUE - THAT'S JUST FUCKING STUPID!\n");
    }
    return interval;
}

Uint32 timeduserevent(Uint32 interval, void* param) {
    SDL_Event *event = (SDL_Event*)param;
    SDL_PushEvent(event);
    return interval;
}

void mark_rendered(void) {
    if ( waitingforrender == 0 ) {
        printf("WTF? waitingforrender == 0?!\n");
    }
    waitingforrender = 0;
}
