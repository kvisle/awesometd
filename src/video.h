#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <SDL.h>
#include <SDL_opengl.h>

#define FPS 60

#define S32X32  0

#include "game.h"

struct texture {
    GLuint texid;
    int w, h;
};

struct video {
	SDL_Surface *s;
    struct texture terrain;
    struct texture enemy1;
    struct texture enemy2;
    struct texture tower1;
};

struct video vSetup(void);

void vDraw(struct video *v, struct game *g);

#endif /* __VIDEO_H__ */
