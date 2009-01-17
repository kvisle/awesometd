#include <SDL.h>
#include <stdio.h>

#include "level.h"
#include "render.h"

int levels[1][15][20] = {
    { // Level 1!
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2 },
        { 0,1,1,2,1,1,1,2,2,2,2,2,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,1,1,1,1,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,1,1,1,1,2,1,1,1,2,1,1,0 },
        { 0,1,1,2,1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 },
        { 0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0 },
        { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
    }
};

SDL_Rect level_tiles[8] = {
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
int tilepicker[2][2][2][2] = {{{{ 2,2 },{ 2,6 }},{{ 2,1 },{ 5,2 }}},{{{ 2,3 },{ 0,2 }},{{ 4,2 },{ 2,7 }}}};

SDL_Surface *tiles;

int currentlevel = 0;

void load_level(SDL_Surface *bg, int lid) {
    int a,b,c,d,e;
    SDL_Rect *tile;
    Uint32 color, black, green, brown;
    black = SDL_MapRGB(bg->format,0,0,0);
    green = SDL_MapRGB(bg->format,0,128,0);
    brown = SDL_MapRGB(bg->format,128,64,0);

    tiles = SDL_LoadBMP("leveltiles.bmp");

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
