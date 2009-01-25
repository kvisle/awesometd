#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL.h>
#include "settings.h"

#define RECTSIZE_X  32
#define RECTSIZE_Y  32

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif

struct updatefield {
    int count;
    SDL_Rect rects[((VIDEOMODE_WIDTH/RECTSIZE_X)*(VIDEOMODE_HEIGHT/RECTSIZE_Y))];
};

int init_video(void);
void render(void);
void updaterect(int x, int y);
int getrenders(void);
SDL_Surface *screen;
void draw_stuff_on_top(void);

#endif /* __RENDER_H__ */
