#include <SDL.h>

#include <math.h>

#include "render.h"
#include "sprites.h"
#include "level.h"
#include "settings.h"
#include "game.h"

#define get_locy(__A) ((__A)/RECTSIZE_Y)
#define get_locx(__A) ((__A)/RECTSIZE_X)
#define get_cellx(__A) (((__A).pos_x)/RECTSIZE_X)
#define get_celly(__A) (((__A).pos_y)/RECTSIZE_Y)

static const int level_monster[1][10][40] = {
    {
        { 1,1,1,1,1,1,1,1,1,1 },
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 },
        { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 },
        { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4 },
        { 5,5,5,5,5,5,5,5,5,5 },
        { 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6 },
        { 7 }
    }
};

static const int monster_batches[1][10] = {
    { 10, 15, 30, 15, 10, 15, 1, 0, 0, 0 }
};

static const int level_batches[1] = {
    10
};

static const struct monster monster_definitions[7] = {
    { 0,0,0,0,15,15,0,4,DIRECTION_S,0,0,10,3 },
    { 0,0,4,0,20,20,0,5,DIRECTION_S,0,0,20,6 },
    { 0,0,0,0,50,50,0,4,DIRECTION_S,0,0,35,10 },
    { 0,0,0,0,100,100,0,3,DIRECTION_S,0,0,50,20 },
    { 0,0,4,0,150,150,0,5,DIRECTION_S,0,0,100,30 },
    { 0,0,0,0,200,200,0,6,DIRECTION_S,0,0,100,60 },
    { 0,0,2,0,2000,2000,0,7,DIRECTION_S,0,0,100000,1000 }
};

const struct tower tower_definitions[2] = {
    { 0,0,1,0,1,5,5,0,50,0,100,0 },
    { 0,0,3,0,1,50,30,10,100,0,50,1 }
};

static const struct projectile projectile_definitions[2] = {
    { 0,0,1,0,10,0,3,0 },
    { 0,0,1,0,1,0,100,0 }
};

static int level = 0;
static int batch = 0;
static int monster = 0;


struct monster monsters[MAX_MONSTERS];
struct tower towers[MAX_TOWERS];
struct projectile projectiles[MAX_PROJECTILES];
struct sprites sprites;


void load_sprite_from_pic(char *filename, int width, int height, int frames) {
    int i,d;
    sprites.sprites[sprites.spritecount] = SDL_LoadBMP(filename);
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
    load_sprite_from_pic("enemy1.bmp",32,32,2);
    load_sprite_from_pic("tower1.bmp",32,32,1);
    load_sprite_from_pic("enemy2.bmp",32,32,2);
    load_sprite_from_pic("tower2.bmp",32,32,8);
    load_sprite_from_pic("enemy3.bmp",32,32,2);
}

void draw_sprite(SDL_Surface *s, int spid, int fid, int rot, int x, int y) {
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
        return 0;
    }

    if ( batch == level_batches[level] ) {
        printf("You won the level, grats! I haven't made a scoreboard or anything yet.\n");
        return 1;
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



void move_projectile(void) {
    int i, ax, ay;
    float x, y, angle;
    SDL_Rect projrect = { 0,0, 4, 4 };

    for (i=0;i<MAX_PROJECTILES;i++) {
        if ( monsters[projectiles[i].targetmonster].cur_hp <= 0 && projectiles[i].damage > 0 ) {
            for ( ay = get_celly( projectiles[i] ); ay <= get_locy( projectiles[i].pos_y+projrect.h ); ay++ ) {
                for ( ax = get_cellx( projectiles[i] ); ax <= get_locx( projectiles[i].pos_x+projrect.w ); ax++ ) {
                    updaterect(ax,ay);
                }
            }
            projectiles[i].damage = 0;
        } else if ( projectiles[i].damage > 0 ) {

            angle = atan2(monsters[projectiles[i].targetmonster].pos_y+16 - projectiles[i].pos_y, monsters[projectiles[i].targetmonster].pos_x+16 - projectiles[i].pos_x);
            x = cos(angle);
            y = sin(angle);

            for ( ay = get_celly( projectiles[i] ); ay <= get_locy(projectiles[i].pos_y + projrect.h); ay++ ) {
                for ( ax = get_cellx( projectiles[i] ); ax <= get_locx(projectiles[i].pos_x + projrect.w); ax++ ) {
                    updaterect(ax,ay);
                }
            }

            projectiles[i].pos_y += y * projectiles[i].speed;
            projectiles[i].pos_x += x * projectiles[i].speed;
            projrect.x = projectiles[i].pos_x;
            projrect.y = projectiles[i].pos_y;

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
                    monsters[projectiles[i].targetmonster].cur_hp = monsters[projectiles[i].targetmonster].cur_hp-projectiles[i].damage;
                    monsters[projectiles[i].targetmonster].upcoming_damage -= projectiles[i].damage;
                    projectiles[i].damage = 0;
                    if ( monsters[projectiles[i].targetmonster].cur_hp <= 0 ) {
                        monsters[projectiles[i].targetmonster].cur_hp = 0;

                        for ( ax=(monsters[projectiles[i].targetmonster].pos_x/RECTSIZE_X);ax<=((monsters[projectiles[i].targetmonster].pos_x+RECTSIZE_X)/RECTSIZE_X);ax++) {
                            for ( ay=(monsters[projectiles[i].targetmonster].pos_y/RECTSIZE_Y);ay<=((monsters[projectiles[i].targetmonster].pos_y+RECTSIZE_Y)/RECTSIZE_Y);ay++) {
                                updaterect(ax,ay);
                            }
                        }

                        update_score(monsters[projectiles[i].targetmonster].score);
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
                draw_numbers();
                monsters[i].cur_hp = 0;
            }
            monsters[i].progress = monsters[i].progress + monsters[i].speed;

            while ( monsters[i].progress > 10 ) {
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
}

void draw_health(SDL_Surface *s, int x, int y, int cur, int max) {
    SDL_Rect green = { x+2,y,(28*cur/max),3 };
    SDL_Rect red = { x+2+green.w,y,28-green.w,3 };
    SDL_FillRect(s, &green, SDL_MapRGB(s->format,0,255,0));
    SDL_FillRect(s, &red, SDL_MapRGB(s->format, 255,0,0));
}
void draw_reload(SDL_Surface *s, int x, int y, int cur, int max) {
    SDL_Rect gray = { x+2,y+28,(28*cur/max),3 };
    SDL_FillRect(s, &gray, SDL_MapRGB(s->format,200,200,200));
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
            SDL_FillRect(screen,&projrect, SDL_MapRGB(screen->format, 0,0,255));
        }
    }
}


void draw_towers(void) {
    int i;
    for (i=0;i<MAX_TOWERS;i++) {
        if ( towers[i].active == 1 ) {
            updaterect(towers[i].loc_x, towers[i].loc_y);
            draw_sprite(screen,towers[i].spid, towers[i].frameno,DIRECTION_N,towers[i].loc_x*RECTSIZE_X,towers[i].loc_y*RECTSIZE_Y);
            draw_reload(screen, towers[i].loc_x*RECTSIZE_X, towers[i].loc_y*RECTSIZE_Y, towers[i].reloadtimeleft, towers[i].reload);
        }
    }
}

void draw_tower(int x, int y) {
    int i;
    for (i=0;i<MAX_TOWERS;i++) {
        if ( towers[i].active == 1 ) {
            if ( towers[i].loc_x == x && towers[i].loc_y == y ) {
                updaterect(x,y);
                draw_sprite(screen,towers[i].spid, towers[i].frameno, DIRECTION_N,towers[i].loc_x*RECTSIZE_X,towers[i].loc_y*RECTSIZE_Y);
                draw_reload(screen,towers[i].loc_x*RECTSIZE_X, towers[i].loc_y*RECTSIZE_Y, towers[i].reloadtimeleft, towers[i].reload);
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

void add_projectile(int pid, int mid, int tid) {
    int i;
    for (i=0;i<MAX_PROJECTILES;i++) {
        if ( projectiles[i].damage == 0 ) {
            projectiles[i] = projectile_definitions[pid];
            projectiles[i].pos_x = (towers[tid].loc_x * RECTSIZE_X) + (RECTSIZE_X / 2);
            projectiles[i].pos_y = (towers[tid].loc_y * RECTSIZE_Y) + (RECTSIZE_Y / 2);
            projectiles[i].targetmonster = mid;
            monsters[mid].upcoming_damage += projectiles[i].damage;
            return;
        }
    }
}

void shoot_towers(void) {
    int i,y;
    double k1,k2,h;
    double shortest = 0;

    int target;

    for (i=0;i<MAX_TOWERS;i++) {
        if (towers[i].active == 1) {
            if (towers[i].reloadtimeleft == 0) {
                target = -1;
                shortest = 0;
                for (y=0;y<MAX_MONSTERS;y++) {
                    if (monsters[y].cur_hp > 0 && monsters[y].cur_hp > monsters[y].upcoming_damage ) {
                        k1 = ((towers[i].loc_x*32)+16)-(monsters[y].pos_x+16);
                        k2 = ((towers[i].loc_y*32)+16)-(monsters[y].pos_y+16);

                        h = (k1*k1) + (k2*k2);

                        if ( h < towers[i].range*towers[i].range ) {
                            if ( shortest == 0 || shortest > h ) {
                                shortest = h;
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
