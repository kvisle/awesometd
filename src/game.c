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

//static GSList *Gamedata.EnemyList;

void EnemyPrint(gpointer data, gpointer user_data);
void EnemyTemplatePrint(gpointer key, gpointer value, gpointer user_data);

void EnemyAdd(gint id,gint delay,StartPosition *sp)
{
    Enemy *e;
    Enemy *t = g_hash_table_lookup(Gamedata.EnemyTemplates,&id);
    if ( t )
    {
        e = g_malloc(sizeof(Enemy));
        memcpy(e,t,sizeof(Enemy));
        e->cur_hp = e->max_hp;
        e->spawn_in = delay;
        e->x = sp->x*32;
        e->y = sp->y*32;
        e->direction = sp->dir;
        Gamedata.EnemyList = g_slist_insert(Gamedata.EnemyList,e, -1);
    }
    else
    {
        printf("Could not spawn undefined enemy.\n");
    }
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
    if ( e->spawn_in > 0 )
    {
        e->spawn_in--;
    }
    else
    {
        e->progress += e->speed;
        if ( e->progress > 100 )
        {
            int x = e->x / 32 - 1;
            int y = e->y / 32 - 1;
            switch(e->direction)
            {
                case DIR_N:
                    e->y--;
                    y = e->y / 32 - 1;
                    if ( e->y % 32 == 0 && Level.map[(y-1)*Level.w+x] == 0 )
                    {
                        if ( Level.map[y*Level.w+(x-1)] == 1 )
                            e->direction = DIR_W;
                        else
                            e->direction = DIR_E;
                    }
                    break;
                case DIR_E:
                    e->x++;
                    x = e->x / 32 - 1;
                    if ( e->x % 32 == 0 && Level.map[y*Level.w+(x+1)] == 0 )
                    {
                        if ( Level.map[(y-1)*Level.w+x] == 1 )
                            e->direction = DIR_N;
                        else
                            e->direction = DIR_S;
                    }
                    break;
                case DIR_S:
                    e->y++;
                    y = e->y / 32 - 1;
                    if ( e->y % 32 && Level.map[(y+1)*Level.w+x] == 0 )
                    {
                        if ( Level.map[y*Level.w+(x+1)] == 1 )
                            e->direction = DIR_E;
                        else
                            e->direction = DIR_W;
                    }
                    break;
                case DIR_W:
                    e->x--;
                    x = e->x / 32 - 1;
                    if ( e->x % 32 == 0 && Level.map[y*Level.w+(x-1)] == 0 )
                    {
                        if ( Level.map[(y+1)*Level.w+x] == 1 )
                            e->direction = DIR_S;
                        else
                            e->direction = DIR_N;
                    }
                    break;
            }
            e->progress -= 100;
        }
        return;
    }
    if ( e->spawn_in == 0 )
    printf("Spawning enemy !\n");
    
//    printf("enemyhp: %d\n",e->cur_hp);
}

void EnemySpawn(Wave *w)
{
    int i,interval = 0;
    StartPosition *sp;
    for (i=0;i<w->enemies;i++)
    {
        if ( w->sp[i] > 0 )
            sp = g_hash_table_lookup(Level.st,&(w->sp[i]));
        interval += w->intervals[i];
        EnemyAdd(w->types[i],interval,sp);
    }
    return;
}

void EnemyPrint(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    printf("The enemy has %d in hp - spawning in %d!\n",e->cur_hp, e->spawn_in);
}

void EnemyTemplateAdd(int id,Enemy *e)
{
    gint *eid = g_malloc(sizeof(gint));
    Enemy *et = g_malloc(sizeof(Enemy));
    memcpy(et,e,sizeof(Enemy));
    *eid = id;
    g_hash_table_insert(Gamedata.EnemyTemplates,eid,et);
}

void EnemyTemplatePrint(gpointer key, gpointer value, gpointer user_data)
{
    gint *id = (gint*)key;
    Enemy *e = (Enemy*)value;
    printf("Template ID#%d\n",*id);
    printf("\tMax HP: %d\n\tSpeed: %d\n\tName: %s\n",e->max_hp,e->speed,e->name);
}

void WaveAdd(Wave *w)
{
    Gamedata.WaveList = g_slist_insert(Gamedata.WaveList,w,-1);
}

void WavePrint(gpointer data, gpointer user_data)
{
    Wave *w = (Wave*)data;
    printf("Wave starts in %d ticks.\n",w->start);
}

void WaveMove(gpointer data, gpointer user_data)
{
    Wave *w = (Wave*)data;
    if ( w->start == 0 && w->blowup > 20 ) return;
    if ( w->start > 0 ) w->start--;
    else {
        w->blowup++;
        return;
    }
    if ( w->start == 0 ) EnemySpawn(w);
}

void StartPositionPrint(gpointer key, gpointer value, gpointer user_data)
{
    StartPosition *sp = (StartPosition*)value;
    gint *k = (gint*)key;
    printf("Key: %d, x:%d y:%d dir:%d\n",*k,sp->x,sp->y,sp->dir);
}

void GameNew(void)
{
    Gamedata.EnemyTemplates = g_hash_table_new(g_int_hash,g_int_equal);
    Level.st = g_hash_table_new(g_int_hash,g_int_equal);
    // TODO: Validate if the load was a success.
    LevelLoad("original.lvl");
    Gamedata.GameStepN = 0;
//    EnemyAdd(1);
//    EnemyAdd(42);
//    EnemyAdd(52);
//    g_slist_foreach(Gamedata.EnemyList,EnemyPrint,NULL);
    g_hash_table_foreach(Gamedata.EnemyTemplates,EnemyTemplatePrint,NULL);
    g_slist_foreach(Gamedata.WaveList,WavePrint,NULL);
    g_hash_table_foreach(Level.st,StartPositionPrint,NULL);
}

void GameStep(void)
{
    if ( Gamedata.GameStepN == 0 ) GameNew();
    Gamedata.GameStepN++;
    g_slist_foreach(Gamedata.EnemyList,EnemyMove,NULL);
    g_slist_foreach(Gamedata.WaveList,WaveMove,NULL);
    EnemyFreeDead();
}
