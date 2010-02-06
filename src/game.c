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

#include "level.h"
#include "game.h"

static int GameStepN = 0;
static Gamedata gamedata;

void EnemyAdd(int hp)
{
    Enemy *e;
    if ( gamedata.EnemyList == 0 )
    {
        gamedata.EnemyList = malloc(sizeof(Enemy));
        e = gamedata.EnemyList;
        e->next = NULL;
    }
    else
    {
        int i = 0;
        e = gamedata.EnemyList;
        while ( e->next != NULL )
        {
            i++;
            e = e->next;
        }
        e->next = malloc(sizeof(Enemy));
        e->next->next = NULL;
        e = e->next;
    }
    e->cur_hp = hp;
}

void EnemyFreeDead(void)
{
    Enemy *e = gamedata.EnemyList;
    Enemy *last = NULL;
    Enemy *next;
    while ( e != NULL )
    {
        next = e->next;
        if ( e->cur_hp <= 0 )
        {
            if ( last == NULL ) gamedata.EnemyList = next;
            else last->next = next;
            free(e);
        }
        else {
            last = e;
        }
        e = next;
    }
}

void GameNew(void)
{
    LevelLoad("original.lvl");
    GameStepN = 0;
    memset(&gamedata, '\0', sizeof(Gamedata));
}

void GameStep(void)
{
    if ( GameStepN == 0 ) GameNew();
    GameStepN++;
}
