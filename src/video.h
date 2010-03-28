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

typedef struct texture {
    GLuint texid;
    gchar *filename;
    int frames;
}Texture;

typedef struct string {
    GLuint texid;
    int w,h;
    int timeleft;
    int alpha;
}String;

int VideoSetMode(int w, int h);
int VideoInit(void);
void VideoDraw(void);
Texture VideoLoadTexture(char *filename);
String * VideoLoadText(char *string, SDL_Color fg,int ft);
void VideoFreeText(String *ptr);
void VideoDrawNumber(int x, int y, int val);
void VideoDrawCircle(float r);

SDL_Surface *screen;
TTF_Font *font, *monofont;
GHashTable *TextureTable;

#endif /* __VIDEO_H__ */
