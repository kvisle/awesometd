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

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glib.h>

#include "common.h"
#include "video.h"
#include "video-game.h"
#include "game.h"
#include "level.h"

#define SNAP_CAMERA_AT_W    128
#define SNAP_CAMERA_AT_H    128

typedef struct vgicons {
    Texture *money;
    Texture *score;

    Texture *pointer;
    Texture *point;
    int tooltip_no;
    String *tooltip;
}VGIcons_t;

static VGIcons_t VGIcons;

void VideoGameUpdateTooltip(int button)
{
    if ( button == VGIcons.tooltip_no ) return;

    SDL_Color fg = { 255,255,255 };
    Tower *t;
    gchar *string;

    VideoFreeText(VGIcons.tooltip);
    if ( button == 0 )
    {
        VGIcons.tooltip = VideoLoadText("Cursor",fg,1);
        VGIcons.tooltip_no = 0;
        VGIcons.tooltip->alpha = 100;
        return;
    }
    t = g_hash_table_lookup(Gamedata.TowerTemplates,&button);
    if ( t )
    {
        string = g_strdup_printf("%s, costs %d$",t->name,t->price);
        VGIcons.tooltip = VideoLoadText(string,fg,1);
        VGIcons.tooltip_no = button;
        g_free(string);
    }
    else
    {
        VGIcons.tooltip = VideoLoadText(" ",fg,1);
        VGIcons.tooltip_no = -1;
    }
    VGIcons.tooltip->alpha = 100;
}

void VideoGameInitIcons(void)
{
    Texture *t = g_hash_table_lookup(TextureTable,"icoMoney.png");
    if ( t )
    {
        if ( t->texid == 0 )
        {
            *t = VideoLoadTexture(t->filename,TEXTURE_SIZE_48x64);
        }
        VGIcons.money = t;
    }
    t = g_hash_table_lookup(TextureTable,"icoScore.png");
    if ( t )
    {
        if ( t->texid == 0 )
        {
            *t = VideoLoadTexture(t->filename,TEXTURE_SIZE_48x64);
        }
        VGIcons.score = t;
    }
    t = g_hash_table_lookup(TextureTable,"icoPointer.png");
    if ( t )
    {
        if ( t->texid == 0 )
        {
            *t = VideoLoadTexture(t->filename,TEXTURE_SIZE_128x128);
        }
        VGIcons.pointer = t;
    }
    t = g_hash_table_lookup(TextureTable,"genPoint.png");
    if ( t )
    {
        if ( t->texid == 0 )
        {
            *t = VideoLoadTexture(t->filename,TEXTURE_SIZE_32x32);
        }
        VGIcons.point = t;
    }

    SDL_Color fg = { 255,255,255 };
    VGIcons.tooltip = VideoLoadText(" ",fg,1);
    VGIcons.tooltip_no = -1;
    VGIcons.tooltip->alpha = 100;
}

void VideoGameDrawIcons(void)
{
    VideoDrawTexturedQuad(screen->w-24, screen->h-48, 24, 32, 0.0, VGIcons.money, 0,1.0);
    VideoDrawTexturedQuad(screen->w-24, screen->h-20, 24, 32, 0.0, VGIcons.score, 0,1.0);
}

void VideoGameSnapCamera(void)
{
    if ( LevelCamera[0] > screen->w - SNAP_CAMERA_AT_W )      LevelCamera[0] = screen->w - SNAP_CAMERA_AT_W;
    if ( LevelCamera[0] < -1*((Level.w*32)-SNAP_CAMERA_AT_W)) LevelCamera[0] = -1*((Level.w*32)-SNAP_CAMERA_AT_W);
    if ( LevelCamera[1] > screen->h - SNAP_CAMERA_AT_H )      LevelCamera[1] = screen->h - SNAP_CAMERA_AT_H;
    if ( LevelCamera[1] < -1*((Level.h*32)-SNAP_CAMERA_AT_H)) LevelCamera[1] = -1*((Level.h*32)-SNAP_CAMERA_AT_H);
}

void VideoGameDrawLevel(void)
{
    int i;
    glPushMatrix();
    glTranslatef(LevelCamera[0],LevelCamera[1],0.0);
    for (i=1;i<=(Level.w*Level.h);++i)
    {
        switch(Level.map[i-1])
        {
            case 0: VideoDrawColoredQuad(16.0f, 16.0f, CELL_SIZE_W, CELL_SIZE_H, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f); break;
            case 1: VideoDrawColoredQuad(16.0f, 16.0f, CELL_SIZE_W, CELL_SIZE_H, 0.0f, 0.5f,0.25f, 0.0f, 1.0f); break;
            case 2: VideoDrawColoredQuad(16.0f, 16.0f, CELL_SIZE_W, CELL_SIZE_H, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f); break;
            case 3: VideoDrawColoredQuad(16.0f, 16.0f, CELL_SIZE_W, CELL_SIZE_H, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f); break;
        }
        glTranslatef(CELL_SIZE_W,0.0f,0.0f);
        if ( i % Level.w == 0 && i > 0 ) 
            glTranslatef(-CELL_SIZE_W*Level.w,CELL_SIZE_H,0.0f);
    }
    glPopMatrix();
}

void VideoGameDrawCursor(void)
{
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    glPushMatrix();
    if ( mx-LevelCamera[0] >= 0 &&
         my-LevelCamera[1] >= 0 &&
         mx-LevelCamera[0] < CELL_SIZE_W*Level.w &&
         my-LevelCamera[1] < CELL_SIZE_H*Level.h )
    {
        glTranslatef(LevelCamera[0]+(((mx-LevelCamera[0])/CELL_SIZE_W)*CELL_SIZE_W)+16.0,
                     LevelCamera[1]+(((my-LevelCamera[1])/CELL_SIZE_H)*CELL_SIZE_H)+16.0,
                     0.0);
        VideoDrawColoredQuad(0.0, 0.0, 32.0, 32.0, 0.0, 1.0, 1.0, 1.0, 0.5);

        if ( Gamedata.button_selected > 0 )
        {
            Tower *t = g_hash_table_lookup(Gamedata.TowerTemplates,&Gamedata.button_selected);
            VideoDrawCircle(t->range);
        }
    }
    glPopMatrix();
}

void VideoGameDrawEnemy(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    float pos_x = LevelCamera[0]-16+e->x;
    float pos_y = LevelCamera[1]-16+e->y;
    VideoDrawTexturedQuadC(pos_x, pos_y, 32.0, 32.0, e->rotation, e->tex, e->frame,1.0-e->poisonfade,1.0,1.0-e->poisonfade,1.0);
    VideoDrawColoredQuad(pos_x, pos_y+14, 28.0, 3.0, 0.0, 1.0, 0.0, 0.0, 1.0);
    VideoDrawColoredQuad(floorf(pos_x-((((float)e->max_hp-e->cur_hp)/(float)e->max_hp)*14)), pos_y+14, (((float)e->cur_hp/(float)e->max_hp)*28), 3, 0.0, 0.0, 1.0, 0.0, 1.0);
}

/*void VideoGameDrawWaveOSD(void)
{
    glPushMatrix();
    glTranslatef(32.0f,16.0f,0.0f);
    GLfloat vcoords[] = {
        0.0f, 0.0f,
        128.0f, 0.0f,
        128.0f, 32.0f,
        0.0f, 32.0f
    };
    glColor4d(1.0f,0.0f,0.0f,1.0f);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glPopMatrix();
}

void VideoGameDrawWave(gpointer data, gpointer user_data)
{
    Wave *w = (Wave*)data;
    glPushMatrix();
    glTranslatef((0.1*w->start)+32.0f,16.0f,0.0f);
    GLfloat vcoords[] = {
        1.0-w->blowup, 1.0-(w->blowup*0.25),
        127.0+w->blowup, 1.0-(w->blowup*0.25),
        127.0+w->blowup, 32.0+(w->blowup*0.25),
        1.0-w->blowup, 32.0+(w->blowup*0.25)
    };
    GLfloat tcoords[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };
    glColor4d(1.0f,1.0f,1.0f,1.0f-(0.05f*w->blowup));
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, w->tex.texid);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}*/

int VideoGameTextId;

void VideoGameDrawMessage(gpointer data, gpointer user_data)
{
    VideoGameTextId++;
    String *s = (String*)data;
    VideoGameDrawText(s,100, screen->h-100-(VideoGameTextId*20));
}

void VideoGameDrawText(String *s, int x, int y)
{
    int posx = x+(s->w/2);
    int posy = y+(s->h/2);
    VideoDrawTexturedQuadC(posx+1, posy+1, s->w, s->h, 0.0, &(s->tex), 0, 0.0, 0.0, 0.0, (float)s->alpha/100);
    VideoDrawTexturedQuad(posx, posy, s->w, s->h, 0.0, &(s->tex), 0,(float)s->alpha/100);
}

void VideoGameCoverEdges(void)
{
    int i;
    if ( LevelCamera[1] > 0 )
        VideoDrawColoredQuad(
                screen->w/2, 
                LevelCamera[1]/2, 
                screen->w, 
                LevelCamera[1], 
                0.0, 
                0.0, 0.0, 0.0, 1.0);
    if ( LevelCamera[1] + (Level.h*32) < screen->h )
        VideoDrawColoredQuad(
                screen->w/2,
                LevelCamera[1] + (Level.h*32)+ ((screen->h - LevelCamera[1] - (Level.h*32))/2),
                screen->w,
                (screen->h - LevelCamera[1] - (Level.h*32)),
                0.0,
                0.0, 0.0, 0.0, 1.0);
    if ( LevelCamera[0] > 0 )
        VideoDrawColoredQuad(
                LevelCamera[0]/2, 
                screen->h/2, 
                LevelCamera[0], 
                screen->h, 
                0.0, 
                0.0, 0.0, 0.0, 1.0);
    if ( LevelCamera[0] + (Level.w*32) < screen->w )
        VideoDrawColoredQuad(
                LevelCamera[0] + (Level.w*32)+ ((screen->w - LevelCamera[0] - (Level.w*32))/2), 
                screen->h/2, 
                (screen->w - LevelCamera[0] - (Level.w*32)), 
                screen->h, 
                0.0, 
                0.0, 0.0, 0.0, 1.0);
}

void VideoGameDrawTower(gpointer data, gpointer user_data)
{
    Tower *t = (Tower*)data;
    VideoDrawTexturedQuad(LevelCamera[0]+(t->x*32)-16, LevelCamera[1]+(t->y*32)-16, 32, 32, 0.0, t->tex, 0,1.0);
    VideoDrawTexturedQuad(LevelCamera[0]+(t->x*32)-16, LevelCamera[1]+(t->y*32)-16, 32, 32, t->rotation, t->tex, 1,1.0);
}

void VideoGameDrawToolbarTowerButton(gpointer key, gpointer value, gpointer user_data)
{
    float alpha = 1.0;
    gint *id = key;
    Tower *t = (Tower*)value;
    glTranslatef(64,0.0,0.0);

    if ( Gamedata.button_selected == *id ) alpha = 0.5;
    VideoDrawTexturedQuad(0.0, 0.0, 64.0, 64.0, 0.0, t->tex, 0, alpha);
    VideoDrawTexturedQuad(0.0, 0.0, 64.0, 64.0, 290.0, t->tex, 1, alpha);
}

void VideoGameDrawToolbar(void)
{
    float alpha = 1.0;
    glPushMatrix();
    glTranslatef(0.0,screen->h-64,0.0);

    VideoDrawColoredQuad((float)screen->w/2.0, 32.0, screen->w, 64.0, 0.0, 0.0, 0.0, 0.0, 0.5);

    if ( Gamedata.button_selected == 0 ) alpha = 0.5;
    VideoDrawTexturedQuad(32.0, 32.0, 64.0, 64.0, 0.0, VGIcons.pointer, 0,alpha);

    glTranslatef(32.0,32.0,0.0);
    g_hash_table_foreach(Gamedata.TowerTemplates,VideoGameDrawToolbarTowerButton,NULL);
    glPopMatrix();
}

void VideoGameDrawProjectiles(gpointer data, gpointer used_data)
{
    Projectile *p = (Projectile*)data;
    VideoDrawColoredQuad(LevelCamera[0]+(p->x), LevelCamera[1]+(p->y), 2, 8, p->rotation, 1.0, 0.0, 0.0, 1.0);
}

void VideoGameDrawParticles(gpointer data, gpointer user_data)
{
    int i;
    ParticleGroup *p = (ParticleGroup*)data;
    for (i=0;i<4;i++)
    VideoDrawTexturedQuadC(LevelCamera[0]+p->xpos+p->particles[i*2],LevelCamera[1]+p->ypos+p->particles[i*2+1],
                          p->size,p->size, 0.0,
                          VGIcons.point,0,
                          p->r,p->g,p->b,p->alpha);
}

void VideoGameDraw(void)
{
    VideoGameDrawLevel();
    g_slist_foreach(Gamedata.EnemyList,VideoGameDrawEnemy,NULL);
    g_slist_foreach(Gamedata.TowerList,VideoGameDrawTower,NULL);
    g_slist_foreach(Gamedata.ProjectileList,VideoGameDrawProjectiles,NULL);
    g_slist_foreach(Gamedata.ParticleList,VideoGameDrawParticles,NULL);
    VideoGameDrawCursor();
    VideoGameCoverEdges();
//    VideoGameDrawWaveOSD();
//    g_slist_foreach(Gamedata.WaveList,VideoGameDrawWave,NULL);
    VideoGameTextId = 0;
    g_slist_foreach(Gamedata.TextList,VideoGameDrawMessage,NULL);
    VideoGameDrawToolbar();
    VideoDrawNumber(screen->w-32,screen->h-61,Gamedata.money);
    VideoDrawNumber(screen->w-32,screen->h-31,Gamedata.score);
    VideoGameDrawText(VGIcons.tooltip,screen->w-VGIcons.tooltip->w-64,screen->h-100);
    VideoGameDrawIcons();
}
