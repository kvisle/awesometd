#include <SDL.h>

#include "render.h"
#include "gfx_charmap.h"
#include "settings.h"
#include "game.h"

static int money = 200;
static int score = 0;
static int lives = 20;

void update_money(int amount) {
    char moneytext[20];
    money += amount;
    if ( money < 0 ) money = 0;
    sprintf(moneytext, "money %6d", money);
    draw_text(screen,moneytext,3*32,420);
}
void update_score(int amount) {
    char scoretext[20];
    score += amount;
    if ( score < 0 ) score = 0;
    sprintf(scoretext, "score %6d", score);
    draw_text(screen,scoretext,3*32,430);
}
void update_lives(int amount) {
    char lifetext[20];
    lives += amount;
    if ( lives < 0 ) lives = 0;
    sprintf(lifetext, "lives %6d", lives);
    draw_text(screen,lifetext,3*32,440);
}
int have_money(int amount) {
    if ( money >= amount ) return 1;
    return 0;
}
void draw_numbers(void) {
    update_lives(0);
    update_score(0);
    update_money(0);
}
