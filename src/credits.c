#include "render.h"
#include "credits.h"
#include "gfx_charmap.h"

char credits[10][25] = {
    "code",
    "",
    "trygve vea",
    "henrik nordvik",
    "martin mikkelsen",
    "",
    "graphics",
    "",
    "trygve vea",
    "lars erik thorsplass"
};

void show_credits(void) {
    current_screen = SCREEN_CREDITS;
    update_all();
}

void draw_credits(void) {
    int i;
    for (i=0;i<10;i++) {
        draw_text(screen,credits[i],200,(200+(i*16)));
    }
}
