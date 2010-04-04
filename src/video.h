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

#ifndef __VIDEO_H__
#define __VIDEO_H__

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>

#include <glib.h>

#include "settings.h"

#define FPS 60

#define TEXTURE_SIZE_48x64      0
#define TEXTURE_SIZE_32x32      1
#define TEXTURE_SIZE_64x64      2
#define TEXTURE_SIZE_128x128    3

typedef struct texture {
    GLuint texid;
    gchar *filename;
    int frames;
    int rows;
    int columns;
}Texture;

typedef struct string {
    Texture tex;
//    GLuint texid;
    int w,h;
    int timeleft;
    int alpha;
}String;

int VideoSetMode(int w, int h);
int VideoInit(void);
void VideoDraw(void);
Texture VideoLoadTexture(char *filename, int size);
String * VideoLoadText(char *string, SDL_Color fg,int ft);
void VideoFreeText(String *ptr);
void VideoDrawNumber(int x, int y, int val);
void VideoDrawCircle(float r);
void VideoDrawColoredQuad(float x, float y, float w, float h, float rot, float r, float g, float b, float a);

#define VideoDrawTexturedQuad(x,y,w,h,rot,tex,sliceid,alpha) VideoDrawTexturedQuadC(x,y,w,h,rot,tex,sliceid,1.0,1.0,1.0,alpha)
void VideoDrawTexturedQuadC(float x, float y, float w, float h, float rot, Texture *tex, int sliceid, float r, float g, float b, float alpha);

SDL_Surface *screen;
TTF_Font *font, *monofont;

#endif /* __VIDEO_H__ */
