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
    event_loop();
    return 0;
}
