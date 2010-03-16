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

#ifndef __GAME_H__
#define __GAME_H__

#include <glib.h>

#define DIR_E 0
#define DIR_S 1
#define DIR_W 2
#define DIR_N 3

void GameStep(void);

typedef struct enemy {
    int x,y;
    int cur_hp, max_hp;
    int speed;
    int direction;
    int progress;
    int spawn_in;
    gint sp;
    gchar *name;
}Enemy;

typedef struct wave {
    gint start;
    gint *intervals;
    gint *types;
    gint *sp;
    gint enemies;
    gint blowup;
    gint texid;
}Wave;

struct gamedata{
    GSList *EnemyList;
    GHashTable *EnemyTemplates;
    GSList *WaveList;
    int GameStepN;
};

void EnemyFreeAll(void);
void EnemyTemplateAdd(int id,Enemy *e);
void WaveAdd(Wave *w);

struct gamedata Gamedata;

#endif /* __GAME_H__ */
