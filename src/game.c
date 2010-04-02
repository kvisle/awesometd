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

typedef struct towerdecidetarget {
    Tower *t;
    guint bestmatch;
    float dx;
    float dy;
}TowerDecideTarget;

void PoisonCloudAdd(int x, int y)
{
    int i;
    ParticleGroup *p = g_malloc(sizeof(ParticleGroup));
    for ( i=0 ; i<4 ; i++ )
    {
        int rx = rand() % 32 - 16;
        int ry = rand() % 32 - 16;
        p->particles[i*2] = rx;
        p->particles[i*2+1] = ry;
    }
    p->xpos = x;
    p->ypos = y;
    p->r = p->b = 0.0;
    p->g = 1.0;
    p->alpha = 0.5;
    p->size = 6.0;
    Gamedata.ParticleList = g_slist_insert(Gamedata.ParticleList,p,-1);
}

void PoisonCloudMove(gpointer data, gpointer user_data)
{
    ParticleGroup *p = (ParticleGroup*)data;
    p->alpha -= 0.0125;
    p->size += 0.5;
    if ( p->alpha == 0.0 ) Gamedata.ParticleList = g_slist_remove(Gamedata.ParticleList,p);
}

void ClickToolbarButton(int button)
{
    Tower *t;
    if ( button == 0 )
    {
        Gamedata.button_selected = 0;
        return;
    }
    t = g_hash_table_lookup(Gamedata.TowerTemplates,&button);
    if ( t )
    {
        Gamedata.button_selected = button;
    }
}

void ClickMap(int x, int y)
{
    if ( Gamedata.button_selected > 0 )
        TowerAdd(Gamedata.button_selected,x,y);
}

void MessageAdd(char *string)
{
    String *s;// = g_malloc(sizeof(String));
    SDL_Color c = { 255,255,255,255 };
    printf("Message: %s\n",string);
    s = VideoLoadText(string,c,0);
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
//        glDeleteTextures(1,&(s->texid));
        Gamedata.TextList = g_slist_remove(Gamedata.TextList,data);
        VideoFreeText(s);
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
        Gamedata.score += e->score;
        Gamedata.money += e->money;
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
        if ( e->poisoned > 0 )
        {
            e->poisontimeleft--;
            if ( e->poisonfade < 1.0 ) e->poisonfade += 0.05;
            if ( e->poisontimeleft % 10 == 0 ) {
                e->cur_hp -= e->poisoned;
                PoisonCloudAdd(e->x-16,e->y-16);
            }
            if ( e->poisontimeleft == 0 ) e->poisoned = 0;
        }
        if ( e->poisoned == 0 && e->poisonfade > 0.0 ) e->poisonfade -= 0.05;
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

void ProjectileTemplateAdd(int id,Projectile *p)
{
    gint *pid = g_malloc(sizeof(gint));
    Projectile *pt = g_malloc(sizeof(Projectile));
    memcpy(pt,p,sizeof(Projectile));
    *pid = id;
    printf("inserting\n");
    g_hash_table_insert(Gamedata.ProjectileTemplates,pid,pt);
    printf("inserted\n");
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
    if ( w->start == 0 ) 
    {
        EnemySpawn(w);
        Gamedata.WaveList = g_slist_remove(Gamedata.WaveList,w);
    }
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
        switch(p->type)
        {
            case PROJECTILE_TYPE_IMPACT:
                if ( p->used ) break;
                e->cur_hp -= p->damage;
                p->used = 1;
                break;
            case PROJECTILE_TYPE_POISON:
                if ( p->used ) break;
                e->poisoned = 1;
                e->poisontimeleft = p->modifier;
                p->used = 1;
                break;
        }
    }
}

void ProjectileMove(gpointer data, gpointer user_data)
{
    Projectile *p = (Projectile*)data;
    // Calculate direction in normalized units
    float dx = p->dx;
    float dy = p->dy;
    float d = 1.0f/sqrt(dx*dx + dy*dy);
    float x = dx*d;
    float y = dy*d;
    p->x += x * p->speed;
    p->y += y * p->speed;
    //p->tx += x*10;
    //p->ty += y*10;
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
    TowerDecideTarget *tdt = (TowerDecideTarget*)user_data;
    Enemy *e = (Enemy*)data;
    if ( (guint)e->cur_hp < tdt->bestmatch )
    {
        tdt->bestmatch = e->cur_hp;
        tdt->bestmatch = e->x;
        tdt->bestmatch = e->y;
    }
}

void TowerCheckForStrongestEnemy(gpointer data, gpointer user_data)
{
    TowerDecideTarget *tdt = (TowerDecideTarget*)user_data;
    Enemy *e = (Enemy*)data;
    if ( (guint)e->cur_hp > tdt->bestmatch || tdt->bestmatch == 0xffffffff )
    {
        tdt->bestmatch = e->cur_hp;
        tdt->dx = e->x;
        tdt->dy = e->y;
    }
}

void TowerCheckDistanceToEnemy(gpointer data, gpointer user_data)
{
    TowerDecideTarget *tdt = (TowerDecideTarget*)user_data;
    Enemy *e = (Enemy*)data;
    gint k1 = (e->x - (tdt->t->x*32)) * (e->x - (tdt->t->x*32));
    gint k2 = (e->y - (tdt->t->y*32)) * (e->y - (tdt->t->y*32));
    guint h = sqrt(k1+k2);

    if ( h < tdt->bestmatch && h < tdt->t->range )
    {
        tdt->bestmatch = h;
        tdt->dx = e->x;
        tdt->dy = e->y;
    }
}

void TowerMove(gpointer data, gpointer user_data)
{
    int i;
    Tower *t = (Tower*)data;
    TowerDecideTarget tdt = { t, 0xffffffff, 0xffffffff, 0xffffffff };
//    guint ce[] = { (t->x*32), (t->y*32), 0xffffffff, 0xffffffff, 0xffffffff, t->range };
    g_slist_foreach(Gamedata.EnemyList,TowerCheckDistanceToEnemy,&tdt);
    if ( t->reloadtimeleft > 0 ) t->reloadtimeleft--;
    if ( tdt.bestmatch != 0xffffffff )
    {
        float angle = (float)(atan2((int)tdt.dy-((int)t->y*32),(int)tdt.dx-((int)t->x*32))*180)/M_PI;
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
    else if ( t->reloadtimeleft == 0 && tdt.bestmatch < 0xffffffff )
    {
        t->reloadtimeleft = t->reloadtime;
        Projectile *p = g_malloc(sizeof(Projectile));
        memcpy(p,t->projectile,sizeof(Projectile));
        p->x = t->x*32-16;
        p->y = t->y*32-16;
        p->dx = tdt.dx-16 - p->x;
        p->dy = tdt.dy-16 - p->y;
//        p->speed = 5;
        p->used = 0;
        p->rotation = t->rotationgoal;
        Gamedata.ProjectileList = g_slist_insert(Gamedata.ProjectileList,p,0);
    }
}

void TowerAdd(int id, int x, int y)
{
    if ( id == 0 ) return;
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
        if ( Gamedata.money < t->price )
        {
            MessageAdd("You can't afford to build this tower now.");
            return;
        }
        Gamedata.money -= t->price;
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
    Gamedata.ProjectileTemplates = g_hash_table_new(g_int_hash,g_int_equal);
    Gamedata.money = 0;
    Gamedata.score = 0;
    Gamedata.button_selected = 0;
    Gamedata.NextLevel = 0;
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
    g_slist_foreach(Gamedata.ParticleList,PoisonCloudMove,NULL);
    g_slist_foreach(Gamedata.TextList,MessageDo,NULL);
    EnemyFreeDead();
    if ( g_slist_length(Gamedata.EnemyList) + g_slist_length(Gamedata.WaveList) == 0 && Gamedata.NextLevel == 0 )
    {
        Gamedata.NextLevel++;
        MessageAdd("We completed the level! YAY!");
    }
}
