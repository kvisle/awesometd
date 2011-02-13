#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <SDL.h>
#include <SDL_opengl.h>

#define FPS 60

#define SFULL   0
#define S32X32  1

#include "game.h"
#include "menu.h"

struct texture {
    GLuint texid;
    int w, h;
};

struct video {
	SDL_Surface *s;
    struct texture logo;

    struct texture terrain;
    struct texture enemy1;
    struct texture enemy2;
    struct texture tower1;
};

struct video vSetup(void);

void vDraw(struct video *v, struct game *g, struct menu *m);

#endif /* __VIDEO_H__ */
