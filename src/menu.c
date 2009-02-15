#include "gfx_charmap.h"
#include "render.h"
#include "game.h"
#include "menu.h"


#define MAX_OPTIONS 4

char options[4][12] = {
    "new game",
    "high scores",
    "credits",
    "quit"
};

int selected = 0;

void draw_menu(void) {
    int i;
    int ypos = 200;
    for (i=0;i<MAX_OPTIONS;i++) {
        if ( i == selected )
            draw_text_color(screen,options[i],400,ypos,255,255,0);
        else
            draw_text_color(screen,options[i],400,ypos,255,255,255);
        ypos += 16;
    }
}

void click_menu(void) {
    switch (selected) {
        case 0:
            new_game();
        break;
        case 1:
            printf("high scores is unimplemented...\n");
        break;
        case 2:
            printf("credits is unimplemented...\n");
        break;
        case 3:
            printf("QUIT?! :(( \n");
        break;
    }
}

void update_selection(int x, int y) {
    if ( x >= 400 && x <= 488 && y >= 200 && y <= (200+(MAX_OPTIONS*16)) )
        selected = (y-200)/16;
}
