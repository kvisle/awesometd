#include "render.h"
#include "highscores.h"
#include "gfx_charmap.h"

int messageno = 0;

char messages[3][50] = {
    "",
    "you won the game",
    "you lost the game"
};

void show_highscores(int message) {
    messageno = message;
    current_screen = SCREEN_HIGHSCORES;
    update_all();
}

void draw_highscores(void) {
    draw_text(screen,"player                    level  batch  score",104,136);

    draw_text(screen,"this is still not implemented",200,200);
    draw_text(screen,messages[messageno],200,250);
}
