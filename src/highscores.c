#include "render.h"
#include "highscores.h"
#include "gfx_charmap.h"

void show_highscores(void) {
    current_screen = SCREEN_HIGHSCORES;
    update_all();
}

void draw_highscores(void) {
    draw_text(screen,"player                    level  batch  score",104,136);

    draw_text(screen,"this is still not implemented",200,200);
}
