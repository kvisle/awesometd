/*
    Awesome Tower Defense
    Copyright (C) 2008-2010  Trygve Vea and contributors (read AUTHORS)

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

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <glib.h>

int LevelLoad(char *filename);

enum Level_Pathfinding {
    PF_BOUNCE,
    PF_LEE
};

typedef struct startposition {
    gint x,y,dir;
}StartPosition;

typedef struct level {
    int w, h;
    GHashTable *st;
    gint *map;
    enum Level_Pathfinding pf;
}t_Level;

t_Level Level;

int LevelCamera[2];

#endif /* __LEVEL_H__ */
