#include <SDL.h>

#include "render.h"
#include "gfx_charmap.h"
#include "settings.h"
#include "game.h"

static int money = 200;
static int score = 0;
static int lives = 1;
char moneytext[20], scoretext[20], lifetext[20];

void update_money(int amount) {
    money += amount;
    if ( money < 0 ) money = 0;
    sprintf(moneytext, "money %6d", money);
    updaterect(3,13);
}
void update_score(int amount) {
    score += amount;
    if ( score < 0 ) score = 0;
    sprintf(scoretext, "score %6d", score);
    updaterect(3,13);
}
void update_lives(int amount) {
    lives += amount;
    if ( lives < 0 ) {
        lives = 0;
        game_over();
    }
    sprintf(lifetext, "lives %6d", lives);
    updaterect(3,13);
}
int have_money(int amount) {
    if ( money >= amount ) return 1;
    return 0;
}
void draw_numbers(void) {
    draw_text(screen,moneytext,3*32,420);
    draw_text(screen,scoretext,3*32,430);
    draw_text(screen,lifetext,3*32,440);
}
SDL_Event go = { SDL_QUIT };
void game_over(void) {
    printf("GAME OVER!!!!\n");
    SDL_PushEvent(&go);
}
