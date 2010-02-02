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

#include "video.h"
#include "video-game.h"

SDL_Surface *screen;

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
    return 0;
}

int VideoInit(void)
{
	return VideoSetMode(VIDEOMODE_WIDTH,VIDEOMODE_HEIGHT);
}

void VideoDraw(void) {
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	VideoGameDraw();
	SDL_GL_SwapBuffers();
}
