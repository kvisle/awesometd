#include <SDL.h>

#include "level.h"
#include "cursor.h"
#include "render.h"
#include "game.h"

static int mouse_x, mouse_y;
static int cell_x, cell_y;

static int colorspopped = 0;

static Uint32 colors[2];

void update_cursor(int x, int y) {
    if ( cell_y > 12 ) {
        draw_numbers();
    }
    updaterect(cell_x,cell_y);
    mouse_x = x;
    mouse_y = y;
    cell_x = x / RECTSIZE_X;
    cell_y = y / RECTSIZE_Y;
    updaterect(cell_x,cell_y);
}

void get_cursor_location(int *x, int *y) {
    *x = cell_x;
    *y = cell_y;
}

void draw_cursor(SDL_Surface *s) {
    Uint32 color = colors[0];
    if ( colorspopped == 0 ) {
        colors[0] = SDL_MapRGB(s->format, 255,255,255); // "OK" color
        colors[1] = SDL_MapRGB(s->format, 255,0,0); // "Not OK" color
        colorspopped++;
    }
    if ( cell_x > 0 && cell_x < 19 && cell_y > 0 && cell_y < 13 ) {
        SDL_Rect rect = { cell_x*RECTSIZE_X, cell_y*RECTSIZE_Y, RECTSIZE_X, RECTSIZE_Y };
        if ( is_path(cell_x,cell_y) ) color = colors[1];
        updaterect(cell_x,cell_y);
        SDL_FillRect(s, &rect, color);
    }
}
