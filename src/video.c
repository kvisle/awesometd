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
#else
#include <GL/gl.h>
#endif
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "video.h"
#include "video-game.h"

int VideoSetMode(int w, int h) {
    screen = SDL_SetVideoMode(w, h, 
                              VIDEOMODE_DEPTH, SDL_OPENGL|SDL_RESIZABLE);
    if (screen == NULL)
    {
        printf("Error when initializing video: %s\n", SDL_GetError());
        return -1;
    }
    SDL_WM_SetCaption("Awesome Tower Defense 0.4","");
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(  0.0, w,
                h, 0.0,
            -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 0;
}

String * VideoLoadText(char *string, SDL_Color fg)
{
    String *s = g_malloc(sizeof(String));
    GLuint tex;
    SDL_Surface *i = TTF_RenderUTF8_Blended(font,string,fg);
    if ( i )
    {
        s->w = i->w;
        s->h = i->h;
        glGenTextures(1,&s->texid);
        glBindTexture(GL_TEXTURE_2D,s->texid);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, i->w, i->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, i->pixels);
        SDL_FreeSurface(i);
        return s;
    }
    else
    {
        return s;
    }
}

GLuint VideoLoadTexture(char *filename)
{
    GLuint tex;
    SDL_Surface *i = IMG_Load(filename);
    if ( i )
    {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, i->w, i->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, i->pixels);
        SDL_FreeSurface(i);
        return tex;
    }
    else
    {
        printf("IMG_Load: %s\n",IMG_GetError());
        return 0;
    }
}

int VideoInit(void)
{
    TTF_Init();
#ifdef __APPLE__
    font = TTF_OpenFont("/Library/Fonts/Tahoma.ttf",16);
#else
    font = TTF_OpenFont("/usr/share/wine/fonts/tahoma.ttf",16);
#endif
    TTF_SetFontStyle(font,TTF_STYLE_BOLD);
    return VideoSetMode(VIDEOMODE_WIDTH,VIDEOMODE_HEIGHT);
}

void VideoDraw(void) {
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    VideoGameDraw();
    SDL_GL_SwapBuffers();
}
