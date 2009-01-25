#include <SDL.h>

#include "settings.h"

#include "render.h"
#include "sprites.h"
#include "cursor.h"
#include "gfx_charmap.h"
#include "gfx_buttons.h"
#include "level.h"
#include "game.h"
#include "timers.h"

SDL_Surface *screen;
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

    init_sprites();
    init_buttons();

    SDL_FillRect(background, NULL, SDL_MapRGB(background->format,0,0,0));
    load_level(background, 0);

    update_score(0);
    update_money(0);
    update_lives(0);
    draw_hint_text(screen);
    draw_buttons(screen);

    return 0;
}


// DRAW STUFF!
void render(void) {
//    draw_text(screen, fpstext, 520, 0);

    sprintf(recttext, "%11d rectupdates", updatefield.count);
//    draw_text(screen, recttext, 0, 0);

    draw_stuff_on_top();

    if ( updatefield.count > 0 ) {
        memset(&field, 0x00, sizeof(int)*(VIDEOMODE_WIDTH/RECTSIZE_X)*(VIDEOMODE_HEIGHT/RECTSIZE_Y));
        SDL_UpdateRects(screen, updatefield.count, (SDL_Rect *) &updatefield.rects);
        updatefield.count = 0;
    }
    mark_rendered();
    rendercount++;
}

// Reports FPS
int getrenders(void) {
    int i = rendercount;
    sprintf(fpstext, "%11d fps", i);
    draw_text(screen, fpstext, 520, 0);
    rendercount = 0;
    return i;
}

// Marks a cell in the grid as one who needs to be updated.
// This function should be called before we draw any sprites in the cell, since
// it will blit from the background surface if we have one.
void updaterect(int x, int y) {
    if ( x < 20 && y < 15 ) {
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
}

//10x13 = help-text

void draw_stuff_on_top(void) {
    int x,y;

    int m_x, m_y;

    int drawn_helptext = 0;
    int drawn_buttons = 0;
    int drawn_numbers = 0;

    get_cursor_location(&m_x, &m_y);

    draw_cursor(screen);

    for (x=0;x<20;x++) {
        for (y=0;y<15;y++) {
            if ( x >= 10 && y >= 13 && drawn_helptext == 0 && field[x][y] == 1 ) {
                drawn_helptext = 1;
                draw_hint_text(screen);
                drawn_buttons = 1;
                draw_buttons(screen);
            }
            if ( x >= 8 && y >= 13 && x <= 10 && y <= 14 && drawn_buttons == 0 && field[x][y] == 1 ) {
                drawn_buttons = 1;
                draw_buttons(screen);
            }
            if ( field[x][y] == 1 ) {
                draw_tower(x,y);
                draw_enemy(x,y);
                draw_projectile(x,y);
            }
            if ( x >= 3 && x <= 9 && y >=13 && field[x][y] == 1 && drawn_numbers == 0 ) {
                drawn_numbers = 1;
                draw_numbers();
            }
        }
    }
    if ( field[3][0] == 1 ) {
        SDL_Rect rect = { 3*RECTSIZE_X, 0*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
    }
    if ( field[19][2] == 1 ) {
        SDL_Rect rect = { 19*RECTSIZE_X, 2*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
    }
    if ( field[17][0] == 1 || field[18][0] == 1 || field[19][0] == 1) {
        draw_text(screen, fpstext, 520, 0);
    }
}
