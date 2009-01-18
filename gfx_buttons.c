#include <SDL.h>

#include "render.h"
#include "gfx_charmap.h"

SDL_Surface *buttonsurface;
static SDL_Rect button_rects[2] = {
    { 260, 430, 48, 48 },
    { 308, 430, 48, 48 }
};

static const int button_count = 2;
static int button_pressed[2] = { 1, 0 };
static const char buttonhint[2][40] = {
    "build archer tower",
    "build cannon tower"
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
        if ( x < (button_rects[i].x + button_rects[i].w) && x > button_rects[i].x && y > button_rects[i].y && y < (button_rects[i].y + button_rects[i].h) ) {
            button_pressed[i] = 1;
            for (n=0;n<button_count;n++) { if (n != i ) button_pressed[n] = 0; }
            draw_buttons(get_screen());
        }
    }
}

void draw_hint_text(SDL_Surface *s) {
    int i;
    for (i=0;i<button_count;i++) {
        if ( button_pressed[i] == 1 ) draw_text(s, buttonhint[i], 320, 416);
    }
}
