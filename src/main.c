/*
    Awesome Tower Defense
    Copyright (C) 2008-2010  Trygve Vea and contributors (read AUTHORS)

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

#include "eventloop.h"
#include "video.h"
#include "game.h"

int main(int argc, char *argv[]) {
    printf("======================================================\n"
           " AWESOME TOWER DEFENSE\n"
           "             by Trygve Vea <trygve.vea@gmail.com>\n"
           "======================================================\n");
    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
        printf("SDL_Init failed : %s\n", SDL_GetError());
        return 1;
    }
    if (VideoInit()) return 1;
    int last_draw = SDL_GetTicks();
    while ( EventLoop() == 0 )
    {
        GameStep();
        VideoDraw();
        int t = (1000/FPS)-(SDL_GetTicks()-last_draw);
        if ( t > 0 && t < 1000 ) SDL_Delay(t);
        last_draw = SDL_GetTicks();
    }
    return 0;
}
