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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "level.h"
#include "game.h"

static int GameStepN = 0;

//static GSList *Gamedata.EnemyList;

void EnemyAdd(int hp)
{
    Enemy *e = malloc(sizeof(Enemy));
    e->cur_hp = hp;
    Gamedata.EnemyList = g_slist_insert(Gamedata.EnemyList,e, -1);
}

void EnemyFreeIfDead(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    if ( e->cur_hp <= 0 ) {
        Gamedata.EnemyList = g_slist_remove(Gamedata.EnemyList,e);
        free(e);
    }
}

void EnemyFreeDead(void)
{
    g_slist_foreach(Gamedata.EnemyList,EnemyFreeIfDead,NULL);
}

void EnemyFree(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    Gamedata.EnemyList = g_slist_remove(Gamedata.EnemyList,e);
    free(e);
}

void EnemyFreeAll(void)
{
    g_slist_foreach(Gamedata.EnemyList,EnemyFree,NULL);
}

void EnemyLoseHP(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*) data;
    // eh wat?
}

void EnemyMove(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    printf("enemyhp: %d\n",e->cur_hp);
}

void EnemyPrint(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    printf("The enemy has %d in hp!\n",e->cur_hp);
}

void GameNew(void)
{
    // TODO: Validate if the load was a success.
    LevelLoad("original.lvl");
    GameStepN = 0;
    EnemyAdd(1);
    EnemyAdd(42);
    EnemyAdd(52);
    g_slist_foreach(Gamedata.EnemyList,EnemyPrint,NULL);
}

void GameStep(void)
{
    if ( GameStepN == 0 ) GameNew();
    GameStepN++;
    g_slist_foreach(Gamedata.EnemyList,EnemyMove,NULL);
    EnemyFreeDead();
}
