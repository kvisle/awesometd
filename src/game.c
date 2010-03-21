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

#include <math.h>

#include "level.h"
#include "game.h"

#define PI 3.14159265

//static GSList *Gamedata.EnemyList;

void MessageAdd(char *string)
{
    String *s = g_malloc(sizeof(String));
    SDL_Color c = { 255,255,255,255 };
    printf("Message: %s\n",string);
    s = VideoLoadText(string,c);
    s->timeleft = 1000;
    s->alpha = 100;
    Gamedata.TextList = g_slist_insert(Gamedata.TextList,s,0);
}

void MessageDo(gpointer data, gpointer user_data)
{
    String *s = (String*)data;
    if ( s->timeleft > 0 ) s->timeleft--;
    else if ( s->timeleft == 0 && s->alpha > 0 ) s->alpha--;
    else if ( s->timeleft == 0 && s->alpha == 0 ) 
    {
        glDeleteTextures(1,&(s->texid));
        Gamedata.TextList = g_slist_remove(Gamedata.TextList,data);
    }
    else return;
}

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
        e->rotation = sp->dir * 90;
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

void EnemyCheckSquare(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)user_data;
    EndPosition *ep = (EndPosition*)data;
    if ( ep->x == e->x / 32 && ep->y == e->y / 32 )
    {
        e->cur_hp = 0;
        MessageAdd("Enemy killed you.");
    }
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
        e->moved++;
        if ( e->moved % 15 == 0 ) {
            e->frame++;
            if ( e->frame == e->tex->frames ) e->frame = 0;
        }
        if ( e->rotation != e->direction*90 )
        {
            e->rotation += e->rotdir*5;
            if ( e->rotation == 360 ) e->rotation = 0;
            if ( e->rotation < 0 ) e->rotation += 360;
        }
        e->progress += e->speed;
        if ( e->progress > 100 )
        {
            if ( e->y % 32 == 0 && e->x % 32 == 0 )
                g_slist_foreach(Level.ep,EnemyCheckSquare,e);
            int x = e->x / 32 - 1;
            int y = e->y / 32 - 1;
            switch(e->direction)
            {
                case DIR_N:
                    e->y--;
                    y = e->y / 32 - 1;
                    if ( e->y % 32 == 0 && Level.map[(y-1)*Level.w+x] == 0 && y > 0 )
                    {
                        if ( Level.map[y*Level.w+(x-1)] == 1 )
                        {
                            e->rotdir = -1;
                            e->direction = DIR_W;
                        }
                        else
                        {
                            e->rotdir = 1;
                            e->direction = DIR_E;
                        }
                    }
                    break;
                case DIR_E:
                    e->x++;
                    x = e->x / 32 - 1;
                    if ( e->x % 32 == 0 && Level.map[y*Level.w+(x+1)] == 0 && x < Level.w-1)
                    {
                        if ( Level.map[(y-1)*Level.w+x] == 1 )
                        {
                            e->rotdir = -1;
                            e->direction = DIR_N;
                        }
                        else
                        {
                            e->rotdir = 1;
                            e->direction = DIR_S;
                        }
                    }
                    break;
                case DIR_S:
                    e->y++;
                    y = e->y / 32 - 1;
                    if ( e->y % 32 == 0 && Level.map[(y+1)*Level.w+x] == 0 && y < Level.h-1)
                    {
                        if ( Level.map[y*Level.w+(x+1)] == 1 )
                        {
                            e->rotdir = -1;
                            e->direction = DIR_E;
                        }
                        else
                        {
                            e->rotdir = 1;
                            e->direction = DIR_W;
                        }
                    }
                    break;
                case DIR_W:
                    e->x--;
                    x = e->x / 32 - 1;
                    if ( e->x % 32 == 0 && Level.map[y*Level.w+(x-1)] == 0 && x > 0)
                    {
                        if ( Level.map[(y+1)*Level.w+x] == 1 )
                        {
                            e->rotdir = -1;
                            e->direction = DIR_S;
                        }
                        else
                        {
                            e->rotdir = 1;
                            e->direction = DIR_N;
                        }
                    }
                    break;
            }
            e->progress -= 100;
        }
        return;
    }
    if ( e->spawn_in == 0 )
    MessageAdd("Spawning enemy !");
}

void EnemySpawn(Wave *w)
{
    int i,interval = 0;
    StartPosition *sp;
    MessageAdd("Wave spawning!");
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

void ProjectileCheckHit(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    Projectile *p = (Projectile*)user_data;
    if ( p->x >= e->x-32 && p->y >= e->y-32 &&
         p->x < e->x && p->y < e->y )
    {
        e->cur_hp--;
        p->used = 1;
    }
}

void ProjectileMove(gpointer data, gpointer user_data)
{
    Projectile *p = (Projectile*)data;
    float dx = p->tx - p->x;
    float dy = p->ty - p->y;
    float d = sqrt(dx*dx + dy*dy);
    float x = dx/d;
    float y = dy/d;
    p->x += x * p->speed;
    p->y += y * p->speed;
    p->tx += x*10;
    p->ty += y*10;
    g_slist_foreach(Gamedata.EnemyList,ProjectileCheckHit,p);
    if ( p->x < 0 || p->x > Level.w*32 || p->y < 0 || p->y > Level.h*32 ) p->used++;
    if ( p->used )
        Gamedata.ProjectileList = g_slist_remove(Gamedata.ProjectileList,p);
}

void TowerLocatedAt(gpointer data, gpointer user_data)
{
    gint *loc = (gint*)user_data;
    Tower *t = (Tower*)data;
    if ( t->x == loc[0] && t->y == loc[1] ) loc[2] = 1;
}

void TowerCheckForWeakestEnemy(gpointer data, gpointer user_data)
{
    guint *ce = (guint*)user_data;
    Enemy *e = (Enemy*)data;
    if ( (guint)e->cur_hp < ce[2] )
    {
        ce[2] = e->cur_hp;
        ce[3] = e->x;
        ce[4] = e->y;
    }
}

void TowerCheckForStrongestEnemy(gpointer data, gpointer user_data)
{
    guint *ce = (guint*)user_data;
    Enemy *e = (Enemy*)data;
    if ( (guint)e->cur_hp > ce[2] || ce[2] == 0xffffffff )
    {
        ce[2] = e->cur_hp;
        ce[3] = e->x;
        ce[4] = e->y;
    }
}

void TowerCheckDistanceToEnemy(gpointer data, gpointer user_data)
{
    guint *ce = (guint*)user_data;
    Enemy *e = (Enemy*)data;
    gint k1 = (e->x-ce[0]) * (e->x-ce[0]);
    gint k2 = (e->y-ce[1]) * (e->y-ce[1]);
    guint h = sqrt(k1+k2);

    if ( h < ce[2] && h < ce[5] )
    {
        ce[2] = h;
        ce[3] = e->x;
        ce[4] = e->y;
    }
}

void TowerMove(gpointer data, gpointer user_data)
{
    int i;
    Tower *t = (Tower*)data;
    guint ce[] = { (t->x*32), (t->y*32), 0xffffffff, 0xffffffff, 0xffffffff, t->range };
    g_slist_foreach(Gamedata.EnemyList,TowerCheckDistanceToEnemy,&ce);
    if ( t->reloadtimeleft > 0 ) t->reloadtimeleft--;
    if ( ce[3] != 0xffffffff )
    {
        float angle = (float)(atan2((int)ce[4]-(int)ce[1],(int)ce[3]-(int)ce[0])*180)/PI;
        t->rotationgoal = angle+90;
    }
    if ( t->rotationgoal != t->rotation )
    {
        int rotate = 3;
        int number1 = (t->rotation+90) - (t->rotationgoal+90);
        if ( number1 < 0 ) number1 += 360;
        int number2 = (t->rotationgoal+90) - (t->rotation+90);
        if ( number2 < 0 ) number2 += 360;


        if ( abs(number1-number2) < 3 ) rotate = abs(number1-number2);

        if ( number1 > number2 ) t->rotation += rotate;
        else t->rotation -= rotate;
        if ( t->rotation >= 270 ) t->rotation -= 360;
        else if ( t->rotation < -90 ) t->rotation += 360;
    }
    else if ( t->reloadtimeleft == 0 && ce[2] < 0xffffffff )
    {
        t->reloadtimeleft = t->reloadtime;
        Projectile *p = g_malloc(sizeof(Projectile));
        p->x = ce[0]-16;
        p->y = ce[1]-16;
        p->tx = ce[3]-16;
        p->ty = ce[4]-16;
        p->speed = 5;
        p->used = 0;
        p->rotation = t->rotationgoal;
        Gamedata.ProjectileList = g_slist_insert(Gamedata.ProjectileList,p,0);
    }
}

void TowerAdd(int id, int x, int y)
{
    if ( x > Level.w || x < 0 || y > Level.h || y < 0 ) return;

    gint loc[] = { x,y,0 };
    g_slist_foreach(Gamedata.TowerList,TowerLocatedAt,&loc);
    if ( loc[2] == 1 )
    {
        MessageAdd("You can't build a tower on top of another tower!");
        return;
    }

    Tower *t = g_hash_table_lookup(Gamedata.TowerTemplates,&id);
    if ( t ) 
    {
        if ( Level.map[(x-1)+(y-1)*Level.w] != 0 ) {
            MessageAdd("You can only build towers on grass.");
            return;
        }
        // TODO: Check if we have enough money to buy the tower.
        Tower *nt = g_malloc(sizeof(Tower));
        memcpy(nt,t,sizeof(Tower));
        nt->x = x;
        nt->y = y;
        nt->rotationgoal = nt->rotation = (rand() % 360)-90;
        Gamedata.TowerList = g_slist_insert(Gamedata.TowerList,nt,-1);
    }
    else
    {
        MessageAdd("Could not build tower (tower not found in hash table - this is a bug!)");
    }
}

void TowerTemplateAdd(int id,Tower *t)
{
    gint *tid = g_malloc(sizeof(gint));
    Tower *tt = g_malloc(sizeof(Tower));
    memcpy(tt,t,sizeof(Tower));
    *tid = id;
    g_hash_table_insert(Gamedata.TowerTemplates,tid,tt);
}
void TowerTemplatePrint(gpointer key, gpointer value, gpointer user_data)
{
    Tower *t = (Tower*)value;
    printf("Tower:\n\tname\t: %s\n\treload\t: %d\n",t->name,t->reloadtime);
}

void GameNew(void)
{
    Gamedata.TowerList = NULL;
    Gamedata.EnemyTemplates = g_hash_table_new(g_int_hash,g_int_equal);
    Gamedata.TowerTemplates = g_hash_table_new(g_int_hash,g_int_equal);
    Level.st = g_hash_table_new(g_int_hash,g_int_equal);
    // TODO: Validate if the load was a success.
    LevelLoad("original.lvl");
    Gamedata.GameStepN = 0;
    g_hash_table_foreach(Gamedata.EnemyTemplates,EnemyTemplatePrint,NULL);
    g_slist_foreach(Gamedata.WaveList,WavePrint,NULL);
    g_hash_table_foreach(Level.st,StartPositionPrint,NULL);
    g_hash_table_foreach(Gamedata.TowerTemplates,TowerTemplatePrint,NULL);
}

void GameStep(void)
{
    if ( Gamedata.GameStepN == 0 ) GameNew();
    Gamedata.GameStepN++;
    g_slist_foreach(Gamedata.EnemyList,EnemyMove,NULL);
    g_slist_foreach(Gamedata.TowerList,TowerMove,NULL);
    g_slist_foreach(Gamedata.ProjectileList,ProjectileMove,NULL);
    g_slist_foreach(Gamedata.WaveList,WaveMove,NULL);
    g_slist_foreach(Gamedata.TextList,MessageDo,NULL);
    EnemyFreeDead();
}
