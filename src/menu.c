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

#include "gfx_charmap.h"
#include "render.h"
#include "game.h"
#include "menu.h"
#include "credits.h"
#include "highscores.h"

#define MAX_OPTIONS 4

char options[4][12] = {
    "new game",
    "high scores",
    "credits",
    "quit"
};

int selected = 0;

void draw_menu(void) {
    int i;
    int ypos = 200;
    for (i=0;i<MAX_OPTIONS;i++) {
        if ( i == selected )
            draw_text_color(screen,options[i],400,ypos,255,255,0);
        else
            draw_text_color(screen,options[i],400,ypos,255,255,255);
        ypos += 16;
    }
}

int click_menu(void) {
    switch (selected) {
        case 0:
            new_game();
        break;
        case 1:
            show_highscores(SCORE_NOMESSAGE);
        break;
        case 2:
            show_credits();
        break;
        case 3:
            return 1;
        break;
    }
    return 0;
}

void update_selection(int x, int y) {
    if ( x >= 400 && x <= 488 && y >= 200 && y <= (200+(MAX_OPTIONS*16)) )
        selected = (y-200)/16;
}

void show_menu(void) {
    current_screen = SCREEN_MENU;
    update_all();
}
