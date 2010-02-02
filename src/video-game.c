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

#include "video-game.h"
#include "game.h"
#include "level.h"

void VideoGameDrawLevel(void)
{
    int i;
    glPushMatrix();
    glTranslatef(LevelCamera[0],LevelCamera[1],0.0);
    for (i=1;i<=(Level.w*Level.h);++i)
    {
        GLfloat vcoords[] = {
             0.0, 0.0,
            32.0, 0.0,
            32.0,32.0,
             0.0,32.0
        };
        switch(Level.map[i-1])
        {
            case 'X': glColor4d( 0.0f, 1.0f, 0.0f, 1.0f); break;
            case '.': glColor4d( 0.5f,0.25f, 0.0f, 1.0f); break;
            case 's': glColor4d( 1.0f, 0.0f, 0.0f, 1.0f); break;
            case 'e': glColor4d( 0.0f, 0.0f, 1.0f, 1.0f); break;
        }
        glVertexPointer(2, GL_FLOAT, 0, vcoords);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glTranslatef(32.0f,0.0f,0.0f);
        if ( i % Level.w == 0 && i > 0 ) 
            glTranslatef(-32.0f*Level.w,32.0f,0.0f);
    }
    glPopMatrix();
}

void VideoGameDraw(void)
{
    VideoGameDrawLevel();
}
