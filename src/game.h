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

#include "video.h"
#include <glib.h>

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

#define PROJECTILE_TYPE_IMPACT  0
#define PROJECTILE_TYPE_POISON  1

void GameStep(void);

typedef struct enemy {
    int x,y;
    int cur_hp, max_hp;
    int speed;
    int direction;
    int progress;
    int spawn_in;
    int frame;
    int poisoned;
    int poisontimeleft;
    float poisonfade;
    gint sp;
    gchar *name;
    Texture *tex;
    int moved;
    int rotation;
    int rotdir;
    int score;
    int money;
}Enemy;

typedef struct projectile {
    float x,y;
    gchar *name;
    int damage;
    int type;
    int modifier;
    float speed;
    float dx,dy;
    int used;
    int rotation;
}Projectile;

typedef struct tower {
    int x,y;
    gchar *name;
    int reloadtime;
    int reloadtimeleft;
    int price;
    int frame;
    int rotation;
    int rotationgoal;
    int range;
    Texture *tex;
    Projectile *projectile;
}Tower;

typedef struct wave {
    gint start;
    gint *intervals;
    gint *types;
    gint *sp;
    gint enemies;
    gint blowup;
    Texture tex;
}Wave;

typedef struct particlegroup {
    GLfloat xpos, ypos;
    GLfloat particles[8];
    GLfloat r,g,b,alpha;
    GLfloat size;
}ParticleGroup;

struct gamedata{
    GSList *EnemyList;
    GSList *TowerList;
    GSList *ProjectileList;
    GHashTable *EnemyTemplates;
    GHashTable *TowerTemplates;
    GHashTable *ProjectileTemplates;
    GSList *WaveList;
    GSList *TextList;
    GSList *ParticleList;
    int GameStepN;
    int money;
    int score;
    int button_selected;
};

void EnemyFreeAll(void);
void EnemyTemplateAdd(int id,Enemy *e);
void TowerTemplateAdd(int id,Tower *t);
void TowerAdd(int id, int x, int y);
void WaveAdd(Wave *w);

struct gamedata Gamedata;

#endif /* __GAME_H__ */
