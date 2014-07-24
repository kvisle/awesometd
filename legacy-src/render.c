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

#include "settings.h"

#include "render.h"
#include "sprites.h"
#include "cursor.h"
#include "gfx_charmap.h"
#include "gfx_buttons.h"
#include "level.h"
#include "game.h"
#include "timers.h"
#include "menu.h"
#include "credits.h"
#include "highscores.h"
#include "tutorial.h"
#include "macros.h"

SDL_Surface *screen;
static SDL_Surface *background, *menubackground;

// An array of integers representing the playfield grid ( X x Y ) ...
// If set to 1, indicates that it should be updated. Usually set by
// updaterect, which also makes sure that the background is blit first.
static int field[VIDEOMODE_WIDTH/RECTSIZE_X][VIDEOMODE_HEIGHT/RECTSIZE_Y];
static int rendercount = 0;
static char fpstext[16] = "";
char costtext[8] = "   100";

int current_screen = SCREEN_MENU;

static struct updatefield updatefield;

DEFIMAGE(menuscreen)

// Some stuff we wanna do before we start drawing stuff.
int init_video(void) {
    screen = SDL_SetVideoMode(VIDEOMODE_WIDTH, VIDEOMODE_HEIGHT, VIDEOMODE_DEPTH, SDL_HWSURFACE);
    SDL_WM_SetCaption("Awesome Tower Defense 0.3","");
    background = SDL_CreateRGBSurface(SDL_HWSURFACE, VIDEOMODE_WIDTH, VIDEOMODE_HEIGHT, VIDEOMODE_DEPTH, RMASK, GMASK, BMASK, AMASK);
    menubackground = SDL_LoadBMP_RW(SDL_RWFromMem(IMAGE_BUF(menuscreen),IMAGE_SIZE(menuscreen)),0);

    init_sprites();
    init_buttons();

    SDL_FillRect(background, NULL, SDL_MapRGB(background->format,0,0,0));
    load_level(background, 0);
    update_all();

    return 0;
}

static void draw_stuff_on_top(void) {
    int x,y;

    int m_x, m_y;

    int drawn_helptext = 0;
    int drawn_buttons = 0;

    get_cursor_location(&m_x, &m_y);

    switch(current_screen) {
        case SCREEN_CREDITS:
            draw_credits();
        break;
        case SCREEN_HIGHSCORES:
            draw_highscores();
        break;
        case SCREEN_TUTORIAL:
            draw_tutorial();
        break;
        case SCREEN_MENU:
            if (
                field[12][6] || field[13][6] || field[14][6] || field[15][6] ||
                field[12][7] || field[13][7] || field[14][7] || field[15][7] ||
                field[12][8] || field[13][8] || field[14][8] || field[15][8]
                ) {
                draw_menu();
            }
        break;
        case SCREEN_INGAME:
            draw_cursor(screen);

            for (x=0;x<20;x++) {
                for (y=0;y<15;y++) {
                    if ( x >= 8 && y >= 13 && drawn_helptext == 0 && field[x][y] == 1 ) {
                        drawn_helptext = 1;
                        draw_hint_text(screen);
                        drawn_buttons = 1;
                        draw_buttons(screen);
                        draw_text_color(screen,costtext,265,416,255,255,0);
                    }
                    if ( field[x][y] == 1 ) {
                        draw_tower(x,y);
                        draw_enemy(x,y);
                        draw_projectile(x,y);
                    }
                }
            }
            if ( field[3][0] == 1 ) {
                SDL_Rect rect = { 3*RECTSIZE_X, 0*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
                SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
            }
            if ( field[19][2] == 1 ) {
                SDL_Rect rect = { 19*RECTSIZE_X, 2*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
                SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
            }
            if ( field[17][0] == 1 || field[18][0] == 1 || field[19][0] == 1) {
                draw_text(screen, fpstext, 520, 0);
            }
            if ( field[2][13] || field[3][13] || field[4][13] || field[5][13] || field[6][13] || field[7][13] || field[8][13] || field[9][13] ||
                 field[2][14] || field[3][14] || field[4][14] || field[5][14] || field[6][14] || field[7][14] || field[8][14] || field[9][14]
                 ) {
                draw_numbers();
            }
            if ( field[16][0] || field[17][0] || field[18][0] || field[19][0] )
                draw_text(screen, fpstext, 520, 0);

            if ( field[8][0] || field[9][0] || field[10][0] || field[11][0] || field[12][0] || field[13][0] || field[14][0] || field[15][0] ) {
                draw_buttons2(screen);
            }
        break;
    }
}

// DRAW STUFF!
void render(void) {
    draw_stuff_on_top();

    if ( updatefield.count > 0 ) {
        memset(&field, 0x00, sizeof(int)*(VIDEOMODE_WIDTH/RECTSIZE_X)*(VIDEOMODE_HEIGHT/RECTSIZE_Y));
        SDL_UpdateRects(screen, updatefield.count, (SDL_Rect *) &updatefield.rects);
        updatefield.count = 0;
    }
    mark_rendered();
    rendercount++;
}

// Reports FPS
int getrenders(void) {
    int i = rendercount;
    sprintf(fpstext, "%11d fps", i);
//    draw_text(screen, fpstext, 520, 0);
    rendercount = 0;
    return i;
}

// Marks a cell in the grid as one who needs to be updated.
// This function should be called before we draw any sprites in the cell, since
// it will blit from the background surface if we have one.
void updaterect(int x, int y) {
    if ( x < 20 && y < 15 ) {
        if ( field[x][y] == 0 ) {
            field[x][y] = 1;
            updatefield.rects[updatefield.count].x = x * RECTSIZE_X;
            updatefield.rects[updatefield.count].y = y * RECTSIZE_Y;
            updatefield.rects[updatefield.count].w = RECTSIZE_X;
            updatefield.rects[updatefield.count].h = RECTSIZE_Y;
            if ( current_screen == SCREEN_INGAME )
                SDL_BlitSurface(background, &updatefield.rects[updatefield.count], screen, &updatefield.rects[updatefield.count]);
            if ( current_screen == SCREEN_MENU || current_screen == SCREEN_CREDITS || current_screen == SCREEN_HIGHSCORES || current_screen == SCREEN_TUTORIAL )
                SDL_BlitSurface(menubackground, &updatefield.rects[updatefield.count], screen, &updatefield.rects[updatefield.count]);
            updatefield.count++;
        }
    }
}

void update_all(void) {
    int x, y;
    if ( current_screen == SCREEN_INGAME ) SDL_BlitSurface(background, NULL, screen, NULL);
    if ( current_screen == SCREEN_MENU || current_screen == SCREEN_HIGHSCORES ) SDL_BlitSurface(menubackground, NULL, screen, NULL);
    for (x=0;x<20;x++) {
        for (y=0;y<15;y++) {
            updaterect(x,y);
        }
    }
}
