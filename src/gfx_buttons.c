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

SDL_Surface *buttonsurface;
static SDL_Rect button_rects[3] = {
    { 260, 430, 48, 48 },
    { 308, 430, 48, 48 },
    { 356, 430, 48, 48 }
};

static const int button_count = 3;
int towerbutton = 0;
static int button_pressed[3] = { 1, 0, 0 };
static const char buttonhint[3][40] = {
    "build pillbox",
    "build artificial volcano",
    "build freezer"
};

void init_buttons(void) {
    buttonsurface = SDL_LoadBMP("buttonbar.bmp");
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

void press_button(int x, int y) {
    int i, n;
    for (i=0;i<button_count;i++) {
        if ( x <= (button_rects[i].x + button_rects[i].w) && x >= button_rects[i].x && y >= button_rects[i].y && y <= (button_rects[i].y + button_rects[i].h) ) {
            button_pressed[i] = 1;
            towerbutton = i;
            sprintf(costtext, "%6d", tower_definitions[i].price);
            for (n=0;n<button_count;n++) { if (n != i ) button_pressed[n] = 0; }
            draw_buttons(screen);
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
