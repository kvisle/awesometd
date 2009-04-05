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
#include "credits.h"
#include "gfx_charmap.h"

static const char* credits[] = {
    "code",
    "",
    "trygve vea",
    "henrik nordvik",
    "martin mikkelsen",
    "",
    "graphics",
    "",
    "trygve vea",
    "lars erik thorsplass",
    "pal kristian hamre",
    "henrik nordvik"
};

void show_credits(void) {
    current_screen = SCREEN_CREDITS;
    update_all();
}

void draw_credits(void) {
    int i;
    for (i=0;i<12;i++) {
        draw_text(screen,credits[i],200,(200+(i*16)));
    }
}
