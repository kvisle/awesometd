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

#include "video.h"
#include "video-game.h"
#include "game.h"
#include "level.h"

#define SNAP_CAMERA_AT_W    128
#define SNAP_CAMERA_AT_H    128

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
        GLfloat vcoords[] = {
             0.0, 0.0,
            CELL_SIZE_W, 0.0,
            CELL_SIZE_W,CELL_SIZE_H,
             0.0,CELL_SIZE_H
        };
        switch(Level.map[i-1])
        {
            case 0: glColor4d( 0.0f, 1.0f, 0.0f, 1.0f); break;
            case 1: glColor4d( 0.5f,0.25f, 0.0f, 1.0f); break;
            case 2: glColor4d( 1.0f, 0.0f, 0.0f, 1.0f); break;
            case 3: glColor4d( 0.0f, 0.0f, 1.0f, 1.0f); break;
        }
        glVertexPointer(2, GL_FLOAT, 0, vcoords);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
        glTranslatef(LevelCamera[0]+(((mx-LevelCamera[0])/CELL_SIZE_W)*CELL_SIZE_W),
                     LevelCamera[1]+(((my-LevelCamera[1])/CELL_SIZE_H)*CELL_SIZE_H),
                     0.0);
        GLfloat vcoords[] = {
             0.0, 0.0,
            CELL_SIZE_W, 0.0,
            CELL_SIZE_W,CELL_SIZE_H,
             0.0,CELL_SIZE_H
        };
        glColor4d(1.0f,1.0f,1.0f,0.5f);
        glVertexPointer(2, GL_FLOAT, 0, vcoords);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    glPopMatrix();
}

void VideoGameDrawEnemy(gpointer data, gpointer user_data)
{
    Enemy *e = (Enemy*)data;
    glPushMatrix();
    glTranslatef(LevelCamera[0]-16,LevelCamera[1]-16,0.0);
    glTranslatef(e->x,e->y,0.0);
    GLfloat vcoords[] = {
        -16.0f, -16.0f,
        16.0f, -16.0f,
        16.0f, 16.0f,
        -16.0f, 16.0f
    };
    GLfloat tcoords[] = {
        e->frame*(1.0/(float)e->tex->frames), 0.0,
        e->frame*(1.0/(float)e->tex->frames)+(1.0/(float)e->tex->frames), 0.0,
        e->frame*(1.0/(float)e->tex->frames)+(1.0/(float)e->tex->frames), 1.0,
        e->frame*(1.0/(float)e->tex->frames), 1.0
    };
    glColor4d(1.0f,1.0f,0.0f,1.0f);
    glRotated(e->rotation,0.0,0.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,e->tex->texid);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void VideoGameDrawWaveOSD(void)
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
}

int VideoGameTextId;

void VideoGameDrawText(gpointer data, gpointer user_data)
{
    VideoGameTextId++;
    String *s = (String*)data;
    glPushMatrix();
    glTranslatef(100.0,screen->h-100-(VideoGameTextId*20),0.0);
    GLfloat vcoords[] = {
        0.0, 0.0,
        s->w, 0.0,
        s->w, s->h,
        0.0, s->h
    };
    GLfloat tcoords[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };
    glColor4d(1.0,1.0,1.0,(double)s->alpha/100);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,s->texid);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void VideoGameCoverEdges(void)
{
    int i;
    glPushMatrix();
    glTranslatef(LevelCamera[0],LevelCamera[1],0.0);
    GLfloat vcoords[] = {
        -64.0, -64.0,
        64.0+(32*Level.w), -64.0,
        64.0+(32*Level.w), 0.0,
        -64.0,0.0
    };
    GLfloat vcoords2[] = {
        -64.0, Level.h*32,
        64.0+(32*Level.w), Level.h*32,
        64.0+(32*Level.w), Level.h*32+64.0,
        -64.0,Level.h*32+64.0
    };
    GLfloat vcoords3[] = {
        -64.0, 0.0,
        0.0, 0.0,
        0.0, Level.h*32,
        -64.0, Level.h*32
    };
    GLfloat vcoords4[] = {
        Level.w*32, 0.0,
        Level.w*32+64, 0.0,
        Level.w*32+64, Level.h*32,
        Level.w*32, Level.h*32
    };
    glColor4d( 0.0f, 0.0f, 0.0f, 1.0f);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glVertexPointer(2, GL_FLOAT, 0, vcoords2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glVertexPointer(2, GL_FLOAT, 0, vcoords3);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glVertexPointer(2, GL_FLOAT, 0, vcoords4);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
}

void VideoGameDrawTower(gpointer data, gpointer user_data)
{
    Tower *t = (Tower*)data;
    glPushMatrix();
    glTranslatef(LevelCamera[0]+(t->x*32)-16,LevelCamera[1]+(t->y*32)-16,0.0);
    GLfloat vcoords[] = {
        -16.0, -16.0,
        16.0, -16.0,
        16.0, 16.0,
        -16.0, 16.0,
    };
    GLfloat tcoords[] = {
        0.0, 0.0,
        0.5, 0.0,
        0.5, 1.0,
        0.0, 1.0
    };
    GLfloat tcoords_gun[] = {
        0.5, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.5, 1.0,
    };
    glColor4d(1.0,1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,t->tex->texid);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glRotated(t->rotation,0.0,0.0,1.0);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords_gun);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void VideoGameDrawToolbarTowerButton(gpointer key, gpointer value, gpointer user_data)
{
    gint *id = key;
    Tower *t = (Tower*)value;
    glPushMatrix();
    glTranslatef(50.0+(*id*64)+32,screen->h-32,0.0);
    GLfloat vcoords[] = {
        -32.0, -32.0,
        32.0, -32.0,
        32.0, 32.0,
        -32.0, 32.0
    };
    GLfloat tcoords[] = {
        0.0, 0.0,
        0.5, 0.0,
        0.5, 1.0,
        0.0, 1.0
    };
    GLfloat tcoords_gun[] = {
        0.5, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.5, 1.0
    };
    glColor4d(1.0,1.0,1.0,1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,t->tex->texid);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords_gun);
    glRotated(290,0.0,0.0,1.0);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void VideoGameDrawToolbar(void)
{
    glPushMatrix();
    glTranslatef(0.0,screen->h-64,0.0);
    GLfloat vcoords[] = {
        0.0, 0.0,
        screen->w, 0.0,
        screen->w, 64.0,
        0.0, 64.0
    };
    glColor4d(0.0f,0.0f,0.0f,0.5f);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
    g_hash_table_foreach(Gamedata.TowerTemplates,VideoGameDrawToolbarTowerButton,NULL);
}

void VideoGameDraw(void)
{
    VideoGameDrawLevel();
    g_slist_foreach(Gamedata.EnemyList,VideoGameDrawEnemy,NULL);
    g_slist_foreach(Gamedata.TowerList,VideoGameDrawTower,NULL);
    VideoGameCoverEdges();
    VideoGameDrawWaveOSD();
    g_slist_foreach(Gamedata.WaveList,VideoGameDrawWave,NULL);
    VideoGameTextId = 0;
    g_slist_foreach(Gamedata.TextList,VideoGameDrawText,NULL);
    VideoGameDrawCursor();
    VideoGameDrawToolbar();
}
