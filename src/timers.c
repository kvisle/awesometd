/*
    Awesome Tower Defense
    Copyright (C) 2008-2009  Trygve Vea and contributors (read AUTHORS)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
