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
#include "render.h"
#include "highscores.h"
#include "gfx_charmap.h"

int messageno = 0;

char messages[3][50] = {
    "",
    "you won the game",
    "you lost the game"
};

void show_highscores(int message) {
    messageno = message;
    current_screen = SCREEN_HIGHSCORES;
    update_all();
}

void draw_highscores(void) {
    draw_text(screen,"player                    level  batch  score",104,136);

    draw_text(screen,"this is still not implemented",200,200);
    draw_text(screen,messages[messageno],200,250);
}
