#include <SDL.h>

#include "render.h"
#include "sprites.h"
#include "gfx_charmap.h"

SDL_Surface *buttonsurface;
static SDL_Rect button_rects[3] = {
    { 260, 430, 48, 48 },
    { 308, 430, 48, 48 },
    { 356, 430, 48, 48 }
};

static const int button_count = 3;
int towerbutton = 0;
static int button_pressed[3] = { 1, 0, 0 };
static const char buttonhint[3][40] = {
    "build pillbox",
    "build artificial volcano",
    "build freezer"
};

void init_buttons(void) {
    buttonsurface = SDL_LoadBMP("buttonbar.bmp");
}

void draw_buttons(SDL_Surface *s) {
    int i,ax,ay;
    SDL_Rect rect = { 0,0,48,48 };
    for (i=0;i<button_count;i++) {
        if ( button_pressed[i] == 1 ) rect.x = 48;
        else rect.x = 0;

        rect.y = i*48;
        for (ay=(button_rects[i].y/RECTSIZE_Y);ay<=((button_rects[i].y+button_rects[i].w+1)/RECTSIZE_Y);ay++) {
            for (ax=(button_rects[i].x/RECTSIZE_Y);ax<=((button_rects[i].x+button_rects[i].x+1)/RECTSIZE_X);ax++) {
                updaterect(ax,ay);
            }
        }
        SDL_BlitSurface(buttonsurface, &rect, s, &button_rects[i]);
    }
}

void press_button(int x, int y) {
    int i, n;
    for (i=0;i<button_count;i++) {
        if ( x <= (button_rects[i].x + button_rects[i].w) && x >= button_rects[i].x && y >= button_rects[i].y && y <= (button_rects[i].y + button_rects[i].h) ) {
            button_pressed[i] = 1;
            towerbutton = i;
            sprintf(costtext, "%6d", tower_definitions[i].price);
            for (n=0;n<button_count;n++) { if (n != i ) button_pressed[n] = 0; }
            draw_buttons(screen);
            return;
        }
    }
}

void draw_hint_text(SDL_Surface *s) {
    int i;
    for (i=0;i<button_count;i++) {
        if ( button_pressed[i] == 1 ) draw_text(s, buttonhint[i], 320, 416);
    }
}
