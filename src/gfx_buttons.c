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

#include "render.h"
#include "sprites.h"
#include "gfx_charmap.h"

extern char _binary_buttonbar2_bmp_start;
extern char _binary_buttonbar_bmp_start;

SDL_Surface *buttonsurface, *buttonsurface2;
static SDL_Rect button_rects[3] = {
    { 260, 430, 48, 48 },
    { 308, 430, 48, 48 },
    { 356, 430, 48, 48 }
};
static SDL_Rect button_rects2[7] = {
    { 256, 8, 24, 24 },
    { 304, 8, 24, 24 },
    { 328, 8, 24, 24 },
    { 352, 8, 24, 24 },
    { 376, 8, 24, 24 },
    { 400, 8, 24, 24 },
    { 448, 8, 24, 24 }
};

static const int button_count = 3;
static const int button2_count = 7;
int towerbutton = 0;
int button2 = 0;
int button2tooltip = -1;
static int button_pressed[3] = { 1, 0, 0 };
static const char buttonhint[3][40] = {
    "build pillbox",
    "build artificial volcano",
    "build freezer"
};
static const char buttonhint2[7][33] = {
    "sell tower                     ",
    "shoot enemy travelled the least",
    "shoot enemy travelled farthest ",
    "shoot enemy with most hp       ",
    "shoot enemy with least hp      ",
    "shoot the fastest enemy        ",
    "toggle shooting                "
};

void update_tooltip(int x, int y) {
    int i;
    for (i=0;i<button2_count;i++) {
        if ( x > button_rects2[i].x && x < (button_rects2[i].x+button_rects2[i].w) &&
             y > button_rects2[i].y && y < (button_rects2[i].y+button_rects2[i].h) ) {
            button2tooltip = i;
            return;
        }
    }
    button2tooltip = -1;
}

void init_buttons(void) {
    buttonsurface = SDL_LoadBMP_RW(SDL_RWFromMem(&_binary_buttonbar_bmp_start,0x0000d836),0);
    buttonsurface2 = SDL_LoadBMP_RW(SDL_RWFromMem(&_binary_buttonbar2_bmp_start,0x00007e36),0);
}

void draw_buttons(SDL_Surface *s) {
    int i,ax,ay;
    SDL_Rect rect = { 0,0,48,48 };
    for (i=0;i<button_count;i++) {
        if ( button_pressed[i] == 1 ) rect.x = 48;
        else rect.x = 0;

        rect.y = i*48;
        for (ay=(button_rects[i].y/RECTSIZE_Y);ay<=((button_rects[i].y+button_rects[i].w+1)/RECTSIZE_Y);ay++) {
            for (ax=(button_rects[i].x/RECTSIZE_Y);ax<=((button_rects[i].x+button_rects[i].x+1)/RECTSIZE_X);ax++) {
                updaterect(ax,ay);
            }
        }
        SDL_BlitSurface(buttonsurface, &rect, s, &button_rects[i]);
    }
}

void draw_buttons2(SDL_Surface *s) {
    int i;
    SDL_Rect rect = { 0,0,24,24 };

    for(i=8;i<=15;i++)
        updaterect(i,0);

    if ( selected_tower == -1 ) return;

    if ( button2tooltip > -1 ) draw_text(screen, buttonhint2[button2tooltip], 256,0 );
    else draw_text(screen, "                                 ", 256, 0 );

    for (i=0;i<button2_count;i++) {
        if ( i < 6 ) {
            if (towers[selected_tower].target_algorithm == i) rect.x = 24;
            else rect.x = 0;
        } else if ( i == 6 ) {
            if (towers[selected_tower].paused == 1) rect.x = 24;
            else rect.x = 0;
        }
        rect.y = i*24;
        SDL_BlitSurface(buttonsurface2,&rect, s, &button_rects2[i]);
    }
}

void press_buttonnumber(int i) {
    int n;
    button_pressed[i-1] = 1;
    towerbutton = ((i-1)*3);
    sprintf(costtext, "%6d", tower_definitions[towerbutton].price);
    for (n=0;n<button_count;n++) { if (n != (i-1) ) button_pressed[n] = 0; }
    draw_buttons(screen);
}

void press_button(int x, int y) {
    int i, n;
    for (i=0;i<button_count;i++) {
        if ( x <= (button_rects[i].x + button_rects[i].w) && x >= button_rects[i].x && y >= button_rects[i].y && y <= (button_rects[i].y + button_rects[i].h) ) {
            button_pressed[i] = 1;
            towerbutton = (i*3);
            sprintf(costtext, "%6d", tower_definitions[(i*3)].price);
            for (n=0;n<button_count;n++) { if (n != i ) button_pressed[n] = 0; }
            draw_buttons(screen);
            return;
        }
    }
    for (i=0;i<button2_count;i++) {
        if ( x <= (button_rects2[i].x + button_rects2[i].w) && x >= button_rects2[i].x && y >= button_rects2[i].y && y <= (button_rects2[i].y + button_rects2[i].h) ) {
            if ( i == 0 ) sell_tower(selected_tower);
            else if ( i == 6 ) toggle_shooting(selected_tower);
            else tower_algorithm(selected_tower, i);
            return;
        }
    }
}

void draw_hint_text(SDL_Surface *s) {
    int i;
    for (i=0;i<button_count;i++) {
        if ( button_pressed[i] == 1 ) draw_text(s, buttonhint[i], 320, 416);
    }
}
