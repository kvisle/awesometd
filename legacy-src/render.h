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

#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL.h>
#include "settings.h"

#define RECTSIZE_X  32
#define RECTSIZE_Y  32

#define SCREEN_MENU     1
#define SCREEN_INGAME   2
#define SCREEN_HIGHSCORES    3
#define SCREEN_CREDITS  4
#define SCREEN_TUTORIAL 5

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif

struct updatefield {
    int count;
    SDL_Rect rects[((VIDEOMODE_WIDTH/RECTSIZE_X)*(VIDEOMODE_HEIGHT/RECTSIZE_Y))];
};

int init_video(void);
void render(void);
void updaterect(int x, int y);
void update_all(void);
int getrenders(void);
extern SDL_Surface *screen;
extern char costtext[8];
extern int current_screen;

#endif /* __RENDER_H__ */
