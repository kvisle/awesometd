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
#include <stdio.h>
#include <stdint.h>

#include "level.h"
#include "render.h"
#include "macros.h"

static const int levels[1][15][20] = {
    { // Level 1!
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2 },
        { 0,1,1,2,1,1,1,2,2,2,2,2,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,1,1,1,1,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,1,1,1,1,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,1,1,1,1,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0 },
        { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
    }
};

static SDL_Rect level_tiles[8] = {
    { 0,0,32,32 }, // Path going North/South
    { 32,0,32,32 }, // Path going West/East
    { 64,0,32,32 }, // Grass
    { 96,0,32,32 }, // Path going West/North
    { 0,32,32,32 }, // Path going North/East
    { 32,32,32,32 }, // Path going South/East
    { 64,32,32,32 }, // Path going West/South
    { 96,32,32,32 } // Crossroads
};

// tilepicker is a lookup table
//
// tilepicker[a][b][c][d][e]
//
// a = center tile
// b = north tile
// c = east tile
// d = south tile
// e = west tile
static const unsigned char tilepicker[2][2][2][2] = {{{{ 2,2 },{ 2,6 }},{{ 2,1 },{ 5,2 }}},{{{ 2,3 },{ 0,2 }},{{ 4,2 },{ 2,7 }}}};

static SDL_Surface *tiles;

static int currentlevel = 0;

DEFIMAGE(leveltiles)

void load_level(SDL_Surface *bg, int lid) {
    int a,b,c,d,e;
    SDL_Rect *tile;
    uint32_t black, green, brown;
    black = SDL_MapRGB(bg->format,0,0,0);
    green = SDL_MapRGB(bg->format,0,128,0);
    brown = SDL_MapRGB(bg->format,128,64,0);

    tiles = SDL_LoadBMP_RW(SDL_RWFromMem(IMAGE_BUF(leveltiles),IMAGE_SIZE(leveltiles)),0);

    SDL_Rect rect = { 0,0,RECTSIZE_X,RECTSIZE_Y};
    int x,y;
    for(y=0;y<15;y++) {
        for(x=0;x<20;x++) {
            rect.x = x*RECTSIZE_X;
            rect.y = y*RECTSIZE_Y;
            if ( x > 0 && x < 19 && y > 0 && y < 13 ) {
                if ( levels[lid][y][x] == 2 ) {
                    b = levels[lid][y-1][x]-1;
                    c = levels[lid][y][x+1]-1;
                    d = levels[lid][y+1][x]-1;
                    e = levels[lid][y][x-1]-1;
                    if ( a < 0 ) a = 1;
                    if ( b < 0 ) b = 1;
                    if ( c < 0 ) c = 1;
                    if ( d < 0 ) d = 1;
                    if ( e < 0 ) e = 1;
                    tile = &level_tiles[tilepicker[b][c][d][e]];
                } else {
                    tile = &level_tiles[2];
                }

                SDL_BlitSurface(tiles, tile, bg, &rect);
                updaterect(x,y);
            }
        }
    }
    currentlevel = lid;
}
int is_path(int x, int y) {
    if ( x > 19 ) return -1;
    if ( y > 14 ) return -1;
    if ( levels[currentlevel][y][x] == 2 ) return 1;
    return 0;
}
