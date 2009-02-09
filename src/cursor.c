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

#include "level.h"
#include "cursor.h"
#include "render.h"
#include "game.h"
#include "sprites.h"

static int mouse_x, mouse_y;
static int cell_x, cell_y;

void update_cursor(int x, int y) {
    if ( cell_y > 12 ) {
        draw_numbers();
    }
    updaterect(cell_x,cell_y);
    mouse_x = x;
    mouse_y = y;
    cell_x = x / RECTSIZE_X;
    cell_y = y / RECTSIZE_Y;
    updaterect(cell_x,cell_y);
}

void get_cursor_location(int *x, int *y) {
    *x = cell_x;
    *y = cell_y;
}

void draw_cursor(SDL_Surface *s) {
    if ( has_tower(cell_x,cell_y) || is_path(cell_x,cell_y) ) return;
    if ( cell_x > 0 && cell_x < 19 && cell_y > 0 && cell_y < 13 ) {
        SDL_Rect rect = { cell_x*RECTSIZE_X, cell_y*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
        updaterect(cell_x,cell_y);
        SDL_FillRect(s, &rect, SDL_MapRGB(s->format, 255,255,255));
    }
}
