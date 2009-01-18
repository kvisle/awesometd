#include <SDL.h>

#include "settings.h"

#include "render.h"
#include "sprites.h"
#include "cursor.h"
#include "gfx_charmap.h"
#include "gfx_buttons.h"
#include "level.h"
#include "game.h"

static SDL_Surface *screen;
static SDL_Surface *background;

// An array of integers representing the playfield grid ( X x Y ) ...
// If set to 1, indicates that it should be updated. Usually set by
// updaterect, which also makes sure that the background is blit first.
static int field[VIDEOMODE_WIDTH/RECTSIZE_X][VIDEOMODE_HEIGHT/RECTSIZE_Y];
static int rendercount = 0;
static char fpstext[16] = "";
static char recttext[24] = "";

static struct updatefield updatefield;

// Some stuff we wanna do before we start drawing stuff.
int init_video(void) {
    screen = SDL_SetVideoMode(VIDEOMODE_WIDTH, VIDEOMODE_HEIGHT, VIDEOMODE_DEPTH, SDL_HWSURFACE);
    background = SDL_CreateRGBSurface(SDL_HWSURFACE, VIDEOMODE_WIDTH, VIDEOMODE_HEIGHT, VIDEOMODE_DEPTH, RMASK, GMASK, BMASK, AMASK);


    memset(&updatefield, 0x00, sizeof(struct updatefield));
    memset(&field, 0x00, sizeof(int)*(VIDEOMODE_WIDTH/RECTSIZE_X)*(VIDEOMODE_HEIGHT/RECTSIZE_Y));

    init_sprites();
    init_buttons();

    SDL_FillRect(background, NULL, SDL_MapRGB(background->format,0,0,0));
    load_level(background, 0);
    SDL_SaveBMP(background, "level.bmp");

    update_score(0);
    update_money(0);
    update_lives(0);
    draw_hint_text(screen);
    draw_buttons(screen);

    return 0;
}


// DRAW STUFF!
void render(void) {
    // TODO : Remove unnecessary drawing from this function ... if we stick to only
    // drawing stuff when we need to, we will save a lot of time.

    SDL_Rect rect = { 3*RECTSIZE_X, 0*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
//    draw_text(screen, fpstext, 520, 0);
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));

    sprintf(recttext, "%11d rectupdates", updatefield.count);
//    draw_text(screen, recttext, 0, 0);
    rect.x = 19*RECTSIZE_X;
    rect.y = 2*RECTSIZE_Y;

    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
    draw_stuff_on_top();

    if ( updatefield.count > 0 ) {
        memset(&field, 0x00, sizeof(int)*(VIDEOMODE_WIDTH/RECTSIZE_X)*(VIDEOMODE_HEIGHT/RECTSIZE_Y));
        SDL_UpdateRects(screen, updatefield.count, (SDL_Rect *) &updatefield.rects);
        updatefield.count = 0;
    }
    rendercount++;
}

// Reports FPS
int getrenders(void) {
    int i = rendercount;
    sprintf(fpstext, "%11d fps", i);
    rendercount = 0;
    return i;
}

// Marks a cell in the grid as one who needs to be updated.
// This function should be called before we draw any sprites in the cell, since
// it will blit from the background surface if we have one.
void updaterect(int x, int y) {
    if ( field[x][y] == 0 ) {
        field[x][y] = 1;
        updatefield.rects[updatefield.count].x = x * RECTSIZE_X;
        updatefield.rects[updatefield.count].y = y * RECTSIZE_Y;
        updatefield.rects[updatefield.count].w = RECTSIZE_X;
        updatefield.rects[updatefield.count].h = RECTSIZE_Y;
        SDL_BlitSurface(background, &updatefield.rects[updatefield.count], screen, &updatefield.rects[updatefield.count]);

        updatefield.count++;
    }
}

SDL_Surface *get_screen(void) {
    return screen;
}


//10x13 = help-text

void draw_stuff_on_top(void) {
    int x,y;

    int m_x, m_y;

    int drawn_helptext = 0;
    int drawn_mouse = 0;
    int drawn_buttons = 0;

    get_cursor_location(&m_x, &m_y);

    for (x=0;x<20;x++) {
        for (y=0;y<15;y++) {
            if ( x >= 10 && y >= 13 && drawn_helptext == 0 && field[x][y] == 1 ) {
                drawn_helptext = 1;
                draw_hint_text(screen);
                drawn_buttons = 1;
                draw_buttons(screen);
            }
            if ( x == m_x && y == m_y && drawn_mouse == 0 && field[x][y] == 1 ) {
                drawn_mouse = 1;
                draw_cursor(screen);
            }
            if ( x >= 8 && y >= 13 && x <= 10 && y <= 14 && drawn_buttons == 0 && field[x][y] == 1 ) {
                drawn_buttons = 1;
                draw_buttons(screen);
            }
            if ( has_tower(x,y) == 1 && field[x][y] == 1 ) {
                draw_tower(x,y);
            }
        }
    }
}
