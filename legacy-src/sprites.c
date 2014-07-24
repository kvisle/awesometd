/*
    Awesome Tower Defense
    Copyright (C) 2008-2009  Trygve Vea and contributors (read AUTHORS)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <SDL.h>

#include <math.h>

#include "render.h"
#include "sprites.h"
#include "level.h"
#include "settings.h"
#include "game.h"
#include "highscores.h"
#include "gfx_charmap.h"
#include "macros.h"

#define get_locx(__A) ((__A)/RECTSIZE_X)
#define get_locy(__A) ((__A)/RECTSIZE_Y)
#define get_cellx(__A) (((__A).pos_x)/RECTSIZE_X)
#define get_celly(__A) (((__A).pos_y)/RECTSIZE_Y)

static const int level_monster[1][25][40] = {
    {
        { 1,1,1,1,1,1,1,1,1,1 },
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 },
        { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 },
        { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4 },
        { 5,5,5,5,5,5,5,5,5,5 },
        { 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6 },
        { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 },
        { 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 },
        { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 },
        { 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11 },
        { 12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12 },
        { 13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13 },
        { 14,14,14,14,14,14,14,14,14,14,14,14,14,14,14 },
        { 15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 },
        { 16,16,16,16,16,16,16,16,16,16,16,16,16,16,16 },
        { 17,17,17,17,17,17,17,17,17,17,17,17,17,17,17 },
        { 18,18,18,18,18,18,18,18,18,18,18,18,18,18,18 },
        { 19,19,19,19,19,19,19,19,19,19,19,19,19,19,19 },
        { 20,20,20,20,20,20,20,20,20,20,20,20,20,20,20 },
        { 21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21 },
        { 22,22,22,22,22,22,22,22,22,22,22,22,22,22,22 },
        { 23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23 },
        { 24,24,24,24,24,24,24,24,24,24,24,24,24,24,24 },
        { 25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25 }
    }
};

static const int monster_batches[1][25] = {
    {
        10, 15, 30, 15, 10, 15, 20, 20, 15, 20,
        20, 25, 20, 15, 20, 15, 15, 15, 15, 15,
        20, 15, 20, 15, 20
    }
};

static const int level_batches[1] = {
    25
};

static const struct monster monster_definitions[25] = {
    { 0,0,0,0,15,15,0,4,DIRECTION_S,0,0,10,1,0,1,1,-1 },
    { 0,0,4,0,20,20,0,5,DIRECTION_S,0,0,20,2,0,1,1,-1 },
    { 0,0,0,0,50,50,0,4,DIRECTION_S,0,0,35,3,0,1,1,-1 },
    { 0,0,0,0,100,100,0,3,DIRECTION_S,0,0,50,4,0,1,1,-1 },
    { 0,0,4,0,150,150,0,5,DIRECTION_S,0,0,100,5,0,1,1,-1 },
    { 0,0,0,0,200,200,0,6,DIRECTION_S,0,0,150,6,0,1,1,-1 },
    { 0,0,2,0,300,300,0,7,DIRECTION_S,0,0,200,7,0,1,1,-1 },
    { 0,0,0,0,400,400,0,6,DIRECTION_S,0,0,250,8,0,1,1,-1 },
    { 0,0,0,0,600,600,0,6,DIRECTION_S,0,0,300,9,0,1,1,-1 },
    { 0,0,0,0,700,700,0,6,DIRECTION_S,0,0,350,10,0,1,1,-1 },
    { 0,0,0,0,800,800,0,6,DIRECTION_S,0,0,400,11,0,1,1,-1 },
    { 0,0,0,0,1000,1000,0,5,DIRECTION_S,0,0,450,12,0,1,1,-1 },
    { 0,0,0,0,1200,1200,0,6,DIRECTION_S,0,0,500,13,0,1,1,-1 },
    { 0,0,0,0,1400,1400,0,7,DIRECTION_S,0,0,550,14,0,1,1,-1 },
    { 0,0,0,0,1600,1600,0,6,DIRECTION_S,0,0,600,15,0,1,1,-1 },
    { 0,0,0,0,1800,1800,0,6,DIRECTION_S,0,0,650,16,0,1,1,-1 },
    { 0,0,0,0,2000,2000,0,8,DIRECTION_S,0,0,700,17,0,1,1,-1 },
    { 0,0,0,0,2250,2250,0,7,DIRECTION_S,0,0,750,18,0,1,1,-1 },
    { 0,0,0,0,2500,2500,0,7,DIRECTION_S,0,0,800,19,0,1,1,-1 },
    { 0,0,0,0,2750,2750,0,7,DIRECTION_S,0,0,850,20,0,1,1,-1 },
    { 0,0,0,0,3000,3000,0,7,DIRECTION_S,0,0,900,21,0,1,1,-1 },
    { 0,0,0,0,3250,3250,0,7,DIRECTION_S,0,0,950,22,0,1,1,-1 },
    { 0,0,0,0,3500,3500,0,7,DIRECTION_S,0,0,1000,23,0,1,1,-1 },
    { 0,0,0,0,3750,3750,0,7,DIRECTION_S,0,0,1100,24,0,1,1,-1 },
    { 0,0,0,0,4000,4000,0,7,DIRECTION_S,0,0,1250,25,0,1,1,-1 }
};

const struct tower tower_definitions[9] = {
    { 0,0,1,0,1,5,0,0,100,0,80,0,ALGORITHM_TRAVELLED_FARTHEST,0,10,1,0 },
    { 0,0,6,0,1,4,0,0,200,0,125,1,ALGORITHM_TRAVELLED_FARTHEST,0,20,2,0 },
    { 0,0,9,0,1,3,0,0,400,0,200,2,ALGORITHM_TRAVELLED_FARTHEST,0,0,0,0 },
    { 0,0,3,0,1,40,0,0,300,0,75,3,ALGORITHM_TRAVELLED_FARTHEST,0,10,4,0 },
    { 0,0,7,0,1,30,0,0,600,0,90,4,ALGORITHM_TRAVELLED_FARTHEST,0,20,5,0 },
    { 0,0,10,0,1,20,0,0,900,0,130,5,ALGORITHM_TRAVELLED_FARTHEST,0,0,0,0 },
    { 0,0,5,0,1,10,0,0,150,0,100,6,ALGORITHM_FASTEST,0,10,7,0 },
    { 0,0,8,0,1,7,0,0,300,0,125,7,ALGORITHM_FASTEST,0,20,8,0 },
    { 0,0,11,0,1,5,0,0,450,0,150,8,ALGORITHM_FASTEST,0,0,0,0 }
};

static struct projectile projectile_definitions[9] = {
    { 0,0,1,0, 7,0, 5, 0, 0,0,0 },
    { 0,0,1,0, 7,0, 10, 0, 0,0,0 },
    { 0,0,1,0, 7,0, 25, 0, 0,0,0 },
    { 0,0,1,0, 1,0, 90, 50, 0,0,0 },
    { 0,0,1,0, 2,0, 180, 50, 0,0,0 },
    { 0,0,1,0, 2,0, 360, 50, 0,0,0 },
    { 0,0,1,0, 3,0, 5, 0, -0.25,0,0 },
    { 0,0,1,0, 3,0, 7, 0, -0.30,0,0 },
    { 0,0,1,0, 3,0, 10, 0, -0.40,0,0 }
};

static int level = 0;
int batch = 0;
static int monster = 0;

int selected_tower = -1;

static struct monster monsters[MAX_MONSTERS];
struct tower towers[MAX_TOWERS];
static struct projectile projectiles[MAX_PROJECTILES];
static struct sprites sprites;
static SDL_Surface *gfx_pause;

DEFIMAGE(enemy1)
DEFIMAGE(tower1)
DEFIMAGE(enemy2)
DEFIMAGE(tower2)
DEFIMAGE(enemy3)
DEFIMAGE(tower3)
DEFIMAGE(tower1_2)
DEFIMAGE(tower2_2)
DEFIMAGE(tower3_2)
DEFIMAGE(tower1_3)
DEFIMAGE(tower2_3)
DEFIMAGE(tower3_3)
DEFIMAGE(minipause)

void reset_everything(void) {
    level = 0;
    batch = 0;
    monster = 0;
    selected_tower = -1;
    memset(&monsters, 0x00, sizeof(struct monster)*MAX_MONSTERS);
    memset(&towers, 0x00, sizeof(struct tower)*MAX_TOWERS);
    memset(&projectiles, 0x00, sizeof(struct projectile)*MAX_PROJECTILES);
}

void toggle_shooting(int tower) {
    if ( towers[tower].paused == 1 ) towers[tower].paused = 0;
    else towers[tower].paused = 1;
    updaterect(14,0);
    updaterect(towers[tower].loc_x,towers[tower].loc_y);
//    printf("toggling for %d: (currently: %d)\n", tower, towers[tower].paused);
    return;
}

static void load_sprite_from_pic(char *filename, int width, int height, int frames, int memsize) {
    int i,d;
    sprites.sprites[sprites.spritecount] = SDL_LoadBMP_RW(SDL_RWFromMem(filename,memsize),0);
    if ( sprites.sprites[sprites.spritecount] != NULL ) {
        sprites.frames[sprites.spritecount] = frames;
        SDL_SetColorKey(sprites.sprites[sprites.spritecount],
                        SDL_SRCCOLORKEY,
                        SDL_MapRGB(
                            sprites.sprites[sprites.spritecount]->format,
                            255,0,255)
                        );
        for (i=0;i<frames;i++) {
            for (d=0;d<MAX_DIRECTIONS;d++) {
                sprites.slices[sprites.spritecount][d][i].x = 0 + (i*width);
                sprites.slices[sprites.spritecount][d][i].y = 0 + (d*height);
                sprites.slices[sprites.spritecount][d][i].w = width;
                sprites.slices[sprites.spritecount][d][i].h = height;
            }
        }
        printf("* Finished adding sprite %d!\n", sprites.spritecount);
        sprites.spritecount++;
    } else {
        printf("Big bad error while loading file: \"%s\": %s\n", filename, SDL_GetError());
    }
}

void init_sprites(void) {
    load_sprite_from_pic(IMAGE_BUF(enemy1),32,32,2,IMAGE_SIZE(enemy1));
    load_sprite_from_pic(IMAGE_BUF(tower1),32,32,1,IMAGE_SIZE(tower1));
    load_sprite_from_pic(IMAGE_BUF(enemy2),32,32,2,IMAGE_SIZE(enemy2));
    load_sprite_from_pic(IMAGE_BUF(tower2),32,32,8,IMAGE_SIZE(tower2));
    load_sprite_from_pic(IMAGE_BUF(enemy3),32,32,2,IMAGE_SIZE(enemy3));
    load_sprite_from_pic(IMAGE_BUF(tower3),32,32,1,IMAGE_SIZE(tower3));
    load_sprite_from_pic(IMAGE_BUF(tower1_2),32,32,1,IMAGE_SIZE(tower1_2));
    load_sprite_from_pic(IMAGE_BUF(tower2_2),32,32,8,IMAGE_SIZE(tower2_2));
    load_sprite_from_pic(IMAGE_BUF(tower3_2),32,32,1,IMAGE_SIZE(tower3_2));
    load_sprite_from_pic(IMAGE_BUF(tower1_3),32,32,1,IMAGE_SIZE(tower1_3));
    load_sprite_from_pic(IMAGE_BUF(tower2_3),32,32,8,IMAGE_SIZE(tower2_3));
    load_sprite_from_pic(IMAGE_BUF(tower3_3),32,32,1,IMAGE_SIZE(tower3_3));
    gfx_pause = SDL_LoadBMP_RW(SDL_RWFromMem(IMAGE_BUF(minipause),IMAGE_SIZE(minipause)),0);
    SDL_SetColorKey(gfx_pause, SDL_SRCCOLORKEY, SDL_MapRGB(gfx_pause->format, 255,0,255));

    projectile_definitions[0].color = SDL_MapRGB(screen->format, 128, 128, 128);
    projectile_definitions[1].color = SDL_MapRGB(screen->format, 192, 192, 192);
    projectile_definitions[2].color = SDL_MapRGB(screen->format, 255, 255, 255);
    projectile_definitions[3].color = SDL_MapRGB(screen->format, 128, 0, 0);
    projectile_definitions[4].color = SDL_MapRGB(screen->format, 192, 0, 0);
    projectile_definitions[5].color = SDL_MapRGB(screen->format, 255, 0, 0);
    projectile_definitions[6].color = SDL_MapRGB(screen->format, 0, 0, 128);
    projectile_definitions[7].color = SDL_MapRGB(screen->format, 0, 0, 192);
    projectile_definitions[8].color = SDL_MapRGB(screen->format, 0, 0, 255);
}

static void draw_sprite(SDL_Surface *s, int spid, int fid, int rot, int x, int y) {
    int ax, ay;
    SDL_Rect dst = { x, y, RECTSIZE_X, RECTSIZE_Y };
    if ( spid < sprites.spritecount ) {
        if ( fid < sprites.frames[spid] ) {
            if ( x < VIDEOMODE_WIDTH && y < VIDEOMODE_HEIGHT ) {
                dst.w = sprites.slices[spid][rot][fid].w;
                dst.h = sprites.slices[spid][rot][fid].h;
                for ( ax = (x/RECTSIZE_X); ax <= ((dst.x+dst.w-1)/RECTSIZE_X); ax++) {
                    for ( ay=(y/RECTSIZE_Y);ay <= ((dst.y+dst.h-1)/RECTSIZE_Y); ay++) {
                        updaterect(ax, ay);
                    }
                }
                SDL_BlitSurface(sprites.sprites[spid], &sprites.slices[spid][rot][fid], s, &dst);
            } else {
                printf("ERR -- draw_sprite(): Attempt to draw outside the screen.\n");
            }
        } else {
            printf("ERR -- draw_sprite(): Frame doesn't exist.\n");
        }
    } else {
        printf("ERR -- draw_sprite(): Sprite doesn't exist.\n");
    }
}

void select_tower(int x, int y) {
    int i;
    for (i=0;i<MAX_TOWERS;i++) {
        if ( towers[i].active == 1 ) {
            if ( towers[i].loc_x == x && towers[i].loc_y == y ) {
                selected_tower = i;
                updaterect(8,0);
                return;
            }
        }
    }
}

void sell_tower(int tid) {
    towers[tid].active = 0;
    update_money((towers[tid].price/2));
    selected_tower = -1;
    updaterect(towers[tid].loc_x, towers[tid].loc_y);
    updaterect(8,0);
}

void tower_algorithm(int tid, int aid) {
    updaterect(8,0);
    towers[tid].target_algorithm = aid;
}

int spawn_monster(void) {
    int i;

    if ( monster_batches[level][batch] == monster ) {
        for (i = 0; i < MAX_MONSTERS; i++) {
            if ( monsters[i].cur_hp > 0 ) {
//                printf("Not spawning: Stuff is still alive.\n");
//   Not spawning any new monsters,
//   because we still have live monsters on the field.
                return 0;
            }
        }
        monster = 0;
        batch++;
        if ( batch == level_batches[level] ) {
            show_highscores(SCORE_WONMESSAGE);
            return 0;
        }
        draw_numbers();
        return 0;
    }

    for (i=0;i<MAX_MONSTERS;i++) {
        if ( monsters[i].cur_hp == 0 ) {
            monsters[i] = monster_definitions[level_monster[level][batch][monster]-1];
            monsters[i].pos_x = 3*RECTSIZE_X;
            monsters[i].pos_y = 0*RECTSIZE_Y;
            monsters[i].direction = DIRECTION_S;
            monster++;
            return 0;
        }
    }
    return 0;
}

static void upgrade_tower(int tid) {
    struct tower tmptower = towers[tid];
    towers[tid] = tower_definitions[tmptower.next_tower];
    towers[tid].loc_x = tmptower.loc_x;
    towers[tid].loc_y = tmptower.loc_y;
    towers[tid].target_algorithm = tmptower.target_algorithm;
}

void move_projectile(void) {
    int i, ax, ay;
    float x, y/*, angle*/;
    SDL_Rect projrect = { 0,0, 4, 4 };

    for (i=0;i<MAX_PROJECTILES;i++) {
        if ( monsters[projectiles[i].targetmonster].cur_hp <= 0 && projectiles[i].damage > 0 ) {
            for ( ay = get_celly( projectiles[i] ); ay <= get_locy( projectiles[i].pos_y + projrect.h ); ay++ ) {
                for ( ax = get_cellx( projectiles[i] ); ax <= get_locx( projectiles[i].pos_x + projrect.w ); ax++ ) {
                    updaterect(ax,ay);
                }
            }
            projectiles[i].damage = 0;
        } else if ( projectiles[i].damage > 0 ) {

            /*angle = atan2(monsters[projectiles[i].targetmonster].pos_y+16 - projectiles[i].pos_y, monsters[projectiles[i].targetmonster].pos_x+16 - projectiles[i].pos_x);
            x = cos(angle);
            y = sin(angle);*/

            // Calculate direction without trigonometric functions. Should be faster
            float dx = monsters[projectiles[i].targetmonster].pos_x+16 - projectiles[i].pos_x;
            float dy = monsters[projectiles[i].targetmonster].pos_y+16 - projectiles[i].pos_y;
            float d = sqrt(dx*dx + dy*dy);
            x = dx/d;
            y = dy/d;

            for ( ay = get_celly( projectiles[i] ); ay <= get_locy(projectiles[i].pos_y + projrect.h); ay++ ) {
                for ( ax = get_cellx( projectiles[i] ); ax <= get_locx(projectiles[i].pos_x + projrect.w); ax++ ) {
                    updaterect(ax,ay);
                }
            }

            projrect.x = (projectiles[i].pos_y += y * projectiles[i].speed);
            projrect.y = (projectiles[i].pos_x += x * projectiles[i].speed);

            for ( ay = get_celly( projectiles[i] ); ay <= get_locy(projectiles[i].pos_y + projrect.h); ay++ ) {
                for ( ax = get_cellx( projectiles[i] ); ax <= get_locx(projectiles[i].pos_x + projrect.w); ax++ ) {
                    updaterect(ax,ay);
                }
            }


            if (
                projectiles[i].pos_x > monsters[projectiles[i].targetmonster].pos_x &&
                projectiles[i].pos_x < (monsters[projectiles[i].targetmonster].pos_x + RECTSIZE_X) &&
                projectiles[i].pos_y > monsters[projectiles[i].targetmonster].pos_y &&
                projectiles[i].pos_y < (monsters[projectiles[i].targetmonster].pos_y + RECTSIZE_Y)
                ) {
                    monsters[projectiles[i].targetmonster].cur_hp -= projectiles[i].damage;
                    if ( towers[projectiles[i].owner].max_exp > 0 ) monsters[projectiles[i].targetmonster].exp_gainer = projectiles[i].owner;

                    if ( projectiles[i].splash > 0 )
                    {
                        int m, k1, k2, h;
                        float dmg, modifier, finaldmg;
                        for (m=0;m<MAX_MONSTERS;m++)
                        {
                            if ( monsters[m].cur_hp > 0 && m != projectiles[i].targetmonster ) {
                                k1 = (projectiles[i].pos_x+2)-(monsters[m].pos_x+16);
                                k2 = (projectiles[i].pos_y+2)-(monsters[m].pos_y+16);

                                h = (k1*k1)+(k2*k2);
                                if ( h <= (projectiles[i].splash*projectiles[i].splash) ) {
                                    dmg = 0.5f * projectiles[i].damage;
                                    modifier = projectiles[i].splash*projectiles[i].splash/(float)h;
                                    finaldmg = dmg * modifier;
                                    monsters[m].cur_hp -= finaldmg;
                                    if ( towers[projectiles[i].owner].max_exp > 0 ) monsters[m].exp_gainer = projectiles[i].owner;

                                    if ( monsters[m].cur_hp <= 0 ) {
                                        monsters[m].cur_hp = 0;
                                        for ( ax=(monsters[m].pos_x/RECTSIZE_X);ax<=((monsters[m].pos_x+RECTSIZE_X)/RECTSIZE_X);ax++) {
                                            for ( ay=(monsters[m].pos_y/RECTSIZE_Y);ay<=((monsters[m].pos_y+RECTSIZE_Y)/RECTSIZE_Y);ay++) {
                                                updaterect(ax,ay);
                                            }
                                        }
                                        // rewarding the player for early kills.
                                        update_score(monsters[m].score-(monsters[m].score*(monsters[m].travelled/1728.0f)));
                                        if ( monsters[m].exp_gainer > -1 ) {
                                            if ( towers[monsters[m].exp_gainer].next_tower > 0 ) towers[monsters[m].exp_gainer].exp++;
                                            if ( towers[monsters[m].exp_gainer].exp >= towers[monsters[m].exp_gainer].max_exp && towers[monsters[m].exp_gainer].next_tower > 0 ) {
                                                upgrade_tower(monsters[m].exp_gainer);
                                            }
                                        }
                                        update_money(monsters[m].money);
                                        draw_numbers();
                                    }
                                }
                            }
                        }
                    }


                    if ( projectiles[i].effect_speed < 1 ) {
                        if ( monsters[projectiles[i].targetmonster].effect_speed == 1 )
                            monsters[projectiles[i].targetmonster].effect_speed += projectiles[i].effect_speed;
                        monsters[projectiles[i].targetmonster].last_ice_shot = SDL_GetTicks();
                    }
                    monsters[projectiles[i].targetmonster].upcoming_damage -= projectiles[i].damage;
                    monsters[projectiles[i].targetmonster].upcoming_effect_speed -= projectiles[i].effect_speed;
                    projectiles[i].damage = 0;
                    if ( monsters[projectiles[i].targetmonster].cur_hp <= 0 ) {
                        monsters[projectiles[i].targetmonster].cur_hp = 0;

                        for ( ax=(monsters[projectiles[i].targetmonster].pos_x/RECTSIZE_X);ax<=((monsters[projectiles[i].targetmonster].pos_x+RECTSIZE_X)/RECTSIZE_X);ax++) {
                            for ( ay=(monsters[projectiles[i].targetmonster].pos_y/RECTSIZE_Y);ay<=((monsters[projectiles[i].targetmonster].pos_y+RECTSIZE_Y)/RECTSIZE_Y);ay++) {
                                updaterect(ax,ay);
                            }
                        }

                        update_score(monsters[projectiles[i].targetmonster].score-(monsters[projectiles[i].targetmonster].score*(monsters[projectiles[i].targetmonster].travelled/1728.0f)));
                        if ( monsters[projectiles[i].targetmonster].exp_gainer > -1 ) {
                            if ( towers[monsters[projectiles[i].targetmonster].exp_gainer].next_tower > 0 ) towers[monsters[projectiles[i].targetmonster].exp_gainer].exp++;
                            if ( towers[monsters[projectiles[i].targetmonster].exp_gainer].exp >= towers[monsters[projectiles[i].targetmonster].exp_gainer].max_exp && towers[monsters[projectiles[i].targetmonster].exp_gainer].next_tower > 0) {
                                upgrade_tower(monsters[projectiles[i].targetmonster].exp_gainer);
                            }
                        }
                        update_money(monsters[projectiles[i].targetmonster].money);
                        draw_numbers();
                    }

            } else {
//                SDL_FillRect(screen,&projrect, SDL_MapRGB(screen->format, 0,0,255));
            }
        }
    }
}

void move_monster(void) {
    int i, old_x, old_y;

    for (i=0;i<MAX_MONSTERS;i++) {

        if ( monsters[i].cur_hp > 0 ) {
            old_x = monsters[i].pos_x;
            old_y = monsters[i].pos_y;
            if ( get_cellx(monsters[i]) == 19 && get_celly(monsters[i]) == 2 ) {
                // TODO: This is a hardcoded way for me to detect if the monster has arrived at its end path
                // This should be fixed to not put stupid limitations on the game.
                update_lives(-1);
                if ( current_screen != SCREEN_INGAME ) return;
                draw_numbers();
                monsters[i].cur_hp = 0;
            }
            monsters[i].progress += (monsters[i].speed*monsters[i].effect_speed);
            if ( (monsters[i].last_ice_shot + 3000 ) < SDL_GetTicks() ) monsters[i].effect_speed = 1;

            while ( monsters[i].progress > 10 ) {
                monsters[i].travelled++;
                switch(monsters[i].direction) {
                    case DIRECTION_N:
                        if ( is_path( get_cellx(monsters[i]), ((monsters[i].pos_y-1)/RECTSIZE_Y)) ) monsters[i].pos_y--;
                        else if ( is_path(((monsters[i].pos_x)/RECTSIZE_X)+1,(get_celly(monsters[i]))) ) {
                            monsters[i].pos_x++;
                            monsters[i].direction = DIRECTION_E;
                        }
                        else if ( is_path(((monsters[i].pos_x-1)/RECTSIZE_X),(get_celly(monsters[i]))) ) {
                            monsters[i].pos_x--;
                            monsters[i].direction = DIRECTION_W;
                        }
                    break;
                    case DIRECTION_E:
                        if ( is_path(((monsters[i].pos_x)/RECTSIZE_X)+1,get_celly(monsters[i])) ) monsters[i].pos_x++;
                        else if ( is_path(get_cellx(monsters[i]), ((monsters[i].pos_y)/RECTSIZE_Y)+1) ) {
                            monsters[i].pos_y++;
                            monsters[i].direction = DIRECTION_S;
                        }
                        else if ( is_path( get_cellx(monsters[i]),((monsters[i].pos_y)/RECTSIZE_Y)-1) ) {
                            monsters[i].pos_y--;
                            monsters[i].direction = DIRECTION_N;
                        }
                    break;
                    case DIRECTION_S:
                        if ( is_path(get_cellx(monsters[i]),((monsters[i].pos_y)/RECTSIZE_Y)+1) ) monsters[i].pos_y++;
                        else if ( is_path(((monsters[i].pos_x)/RECTSIZE_X)+1,(get_celly(monsters[i]))) ) {
                            monsters[i].pos_x++;
                            monsters[i].direction = DIRECTION_E;
                        }
                        else if ( is_path(((monsters[i].pos_x-1)/RECTSIZE_X),(get_celly(monsters[i])))) {
                            monsters[i].pos_x--;
                            monsters[i].direction = DIRECTION_W;
                        }
                    break;
                    case DIRECTION_W:
                        if ( is_path(((monsters[i].pos_x-1)/RECTSIZE_X),(get_celly(monsters[i]))) ) monsters[i].pos_x--;
                        else if ( is_path( get_cellx(monsters[i]),((monsters[i].pos_y)/RECTSIZE_Y)-1) ) {
                            monsters[i].pos_y--;
                            monsters[i].direction = DIRECTION_N;
                        }
                        else if ( is_path(get_cellx(monsters[i]), ((monsters[i].pos_y)/RECTSIZE_Y)+1) ) {
                            monsters[i].pos_y++;
                            monsters[i].direction = DIRECTION_S;
                        }
                    break;
                }
                monsters[i].progress = monsters[i].progress - 10;
            }
            updaterect(old_x/RECTSIZE_X, old_y/RECTSIZE_Y);
            updaterect((old_x+31)/RECTSIZE_X,old_y/RECTSIZE_Y);
            updaterect((old_x+31)/RECTSIZE_X,(old_y+31)/RECTSIZE_Y);
            updaterect(old_x/RECTSIZE_X,(old_y+31)/RECTSIZE_Y);
            updaterect(get_cellx(monsters[i]), get_celly(monsters[i]));
            updaterect((monsters[i].pos_x+31)/RECTSIZE_X,get_celly(monsters[i]));
            updaterect((monsters[i].pos_x+31)/RECTSIZE_X,(monsters[i].pos_y+31)/RECTSIZE_Y);
            updaterect(get_cellx(monsters[i]),(monsters[i].pos_y+31)/RECTSIZE_Y);
        }
    }
}

static void draw_health(SDL_Surface *s, int x, int y, int cur, int max) {
    SDL_Rect green = { x+2,y,(28*cur/max),3 };
    SDL_Rect red = { x+2+green.w,y,28-green.w,3 };
    SDL_FillRect(s, &green, SDL_MapRGB(s->format,0,255,0));
    SDL_FillRect(s, &red, SDL_MapRGB(s->format, 255,0,0));
}
static void draw_reload(SDL_Surface *s, int x, int y, int cur, int max) {
    SDL_Rect gray = { x+2,y+28,(28*cur/max),3 };
    SDL_FillRect(s, &gray, SDL_MapRGB(s->format,200,200,200));
}
static void draw_exp(SDL_Surface *s, int x, int y, int cur, int max) {
    SDL_Rect yellow = { x+2,y+25,(28*cur/max),3 };
    SDL_FillRect(s, &yellow, SDL_MapRGB(s->format,255,255,0));
}

void draw_enemy(int x, int y) {
    int i;
    for (i=0;i<MAX_MONSTERS;i++) {
        if ( monsters[i].cur_hp > 0 && (get_cellx(monsters[i])) == x && (get_celly(monsters[i])) == y ) {
            draw_sprite(screen,monsters[i].spid, monsters[i].frameno,monsters[i].direction, monsters[i].pos_x, monsters[i].pos_y);
            draw_health(screen,monsters[i].pos_x, monsters[i].pos_y, monsters[i].cur_hp, monsters[i].max_hp);
        }
    }
}

void draw_projectile(int x, int y) {
    int i, x2, y2;
    for (i=0;i<MAX_PROJECTILES;i++) {
        x2 = get_cellx(projectiles[i]);
        y2 = get_celly(projectiles[i]);
        if ( projectiles[i].damage > 0 && x2 == x && y2 == y ) {
            SDL_Rect projrect = { projectiles[i].pos_x, projectiles[i].pos_y, 4, 4 };
            SDL_FillRect(screen,&projrect, projectiles[i].color);
        }
    }
}

void draw_tower(int x, int y) {
    int i;
    for (i=0;i<MAX_TOWERS;i++) {
        if ( towers[i].active == 1 ) {
            if ( towers[i].loc_x == x && towers[i].loc_y == y ) {
                SDL_Rect pause = { (towers[i].loc_x*32)+24, (towers[i].loc_y*32)+24, 8, 8 };
                updaterect(x,y);
                if ( selected_tower == i ) {
                    SDL_Rect trect = { towers[i].loc_x*32, towers[i].loc_y*32,32,32 };
                    SDL_FillRect(screen, &trect, SDL_MapRGB(screen->format, 0,0,255) );
                }
                draw_sprite(screen,towers[i].spid, towers[i].frameno, DIRECTION_N,towers[i].loc_x*RECTSIZE_X,towers[i].loc_y*RECTSIZE_Y);
                draw_reload(screen,towers[i].loc_x*RECTSIZE_X, towers[i].loc_y*RECTSIZE_Y, towers[i].reloadtimeleft, towers[i].reload);
                if ( towers[i].max_exp > 0 )
                    draw_exp(screen,towers[i].loc_x*RECTSIZE_X, towers[i].loc_y*RECTSIZE_Y, towers[i].exp, towers[i].max_exp);
                if ( towers[i].paused == 1 ) SDL_BlitSurface(gfx_pause, NULL, screen, &pause);
                return;
            }
        }
    }
}

int has_tower(int x, int y) {
    int i;
    for (i=0;i<MAX_TOWERS;i++) {
        if ( towers[i].active == 1 ) {
            if ( towers[i].loc_x == x && towers[i].loc_y == y ) {
                return 1;
            }
        }
    }
    return 0;
}

void add_tower(int x, int y, int type) {
    int i;
    if ( have_money(tower_definitions[type].price) == 1 ) {
        if ( has_tower(x, y) == 0 ) {
            for (i=0;i<MAX_TOWERS;i++) {
                if ( towers[i].active == 0 ) {
                    towers[i] = tower_definitions[type];
                    towers[i].loc_x = x;
                    towers[i].loc_y = y;
                    update_money(tower_definitions[type].price*-1);
                    draw_numbers();
                    return;
                }
            }
        }
    }
}

void animate_sprites(void) {
    int i;
    for (i=0;i<MAX_MONSTERS;i++) {
        if (monsters[i].cur_hp > 0) {
            if ( (monsters[i].frameno+1) < sprites.frames[monsters[i].spid] ) monsters[i].frameno++;
            else monsters[i].frameno = 0;
        }
    }
    for (i=0;i<MAX_TOWERS;i++) {
        if (towers[i].active == 1 ) {
            if ( (towers[i].frameno+1) < sprites.frames[towers[i].spid] ) towers[i].frameno++;
            else towers[i].frameno = 0;
            updaterect(towers[i].loc_x,towers[i].loc_y);
        }
    }
}

static void add_projectile(int pid, int mid, int tid) {
    int i;
    for (i=0;i<MAX_PROJECTILES;i++) {
        if ( projectiles[i].damage == 0 ) {
            projectiles[i] = projectile_definitions[pid];
            projectiles[i].pos_x = (towers[tid].loc_x * RECTSIZE_X) + (RECTSIZE_X / 2);
            projectiles[i].pos_y = (towers[tid].loc_y * RECTSIZE_Y) + (RECTSIZE_Y / 2);
            projectiles[i].targetmonster = mid;
            projectiles[i].owner = tid;
            monsters[mid].upcoming_damage += projectiles[i].damage;
            monsters[mid].upcoming_effect_speed += projectiles[i].effect_speed;
            return;
        }
    }
}

void shoot_towers(void) {
    int i,y;
    double k1,k2,h;

    int target;

    for (i=0;i<MAX_TOWERS;i++) {
        if (towers[i].active == 1 && towers[i].paused == 0 ) {
            if (towers[i].reloadtimeleft == 0) {
                target = -1;
                for (y=0;y<MAX_MONSTERS;y++) {
                    if (monsters[y].cur_hp > 0 && monsters[y].cur_hp > monsters[y].upcoming_damage ) {
                        k1 = ((towers[i].loc_x*32)+16)-(monsters[y].pos_x+16);
                        k2 = ((towers[i].loc_y*32)+16)-(monsters[y].pos_y+16);

                        h = (k1*k1) + (k2*k2);

                        if ( h < towers[i].range*towers[i].range ) {
                            if ( target > -1 ) {
                                switch(towers[i].target_algorithm) {
                                    case ALGORITHM_MOST_HP:
                                        if ( monsters[y].cur_hp > monsters[target].cur_hp ) target = y;
                                    break;
                                    case ALGORITHM_LEAST_HP:
                                        if ( monsters[y].cur_hp < monsters[target].cur_hp ) target = y;
                                    break;
                                    case ALGORITHM_FASTEST:
                                        if (
                                            (monsters[y].effect_speed*(monsters[y].speed*monsters[y].upcoming_effect_speed)) >
                                            (monsters[target].effect_speed*(monsters[target].speed*monsters[target].upcoming_effect_speed))
                                           ) target = y;
                                    break;
                                    case ALGORITHM_TRAVELLED_SHORTEST:
                                        if ( monsters[y].travelled < monsters[target].travelled ) target = y;
                                    break;
                                    case ALGORITHM_TRAVELLED_FARTHEST:
                                        if ( monsters[y].travelled > monsters[target].travelled ) target = y;
                                    break;
                                }
                            } else {
                                target = y;
                            }
                        }
                    }
                }
                if ( target > -1 ) {
                    add_projectile(towers[i].projectile,target,i);
                    towers[i].reloadtimeleft = towers[i].reload;
                    updaterect(towers[i].loc_x, towers[i].loc_y);
                }
            } else {
                towers[i].reloadtimeleft--;
                updaterect(towers[i].loc_x, towers[i].loc_y);
            }
        }
    }
}

void draw_numbers2(void) {
    char batchtext[20];
    sprintf(batchtext, "batch  %2d-%2d", batch+1, level_batches[level] );
    draw_text(screen,batchtext,3*32,450);
}
