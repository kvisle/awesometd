#ifndef __SPRITES_H__
#define __SPRITES_H__

#include <SDL.h>

#define MAX_SPRITES 8
#define MAX_FRAMES 8
#define MAX_DIRECTIONS 4
#define MAX_MONSTERS 50
#define MAX_TOWERS 100
#define MAX_PROJECTILES 128

#define DIRECTION_N 0
#define DIRECTION_E 1
#define DIRECTION_S 2
#define DIRECTION_W 3

struct monster {
    int pos_x, pos_y; // Exact coordinates, not the cell-coordinates...
                    // We need the exact ones, because we want smooth movements.
    int spid; // Sprite ID
    int frameno;
    int cur_hp, max_hp;
    int speed;
    int direction;
    int progress;
    int travelled;
    int score;
    int money;
};

struct tower {
    int loc_x, loc_y; // The grid location ... these doesn't move around.
    int spid; // Sprite ID
    int frameno;
    int active;
    int reload;
    int damage;
    int splash;
    int price;
    int reloadtimeleft;
    int range;
    int projectile;
};

struct sprites {
    SDL_Surface *sprites[MAX_SPRITES];
    int frames[MAX_SPRITES];
    SDL_Rect slices[MAX_SPRITES][MAX_DIRECTIONS][MAX_FRAMES];
    int spritecount;
};

struct projectile {
    float pos_x, pos_y; // The exact location.
    int spid; // Sprite ID
    int frameno;
    int speed;
    int targetmonster;
    int damage; // If damage is 0, it's considered as inactive.
    int splash;
};

void draw_sprite(SDL_Surface *s, int spid, int fid, int rot, int x, int y);
void spawn_monster(void);
void move_monster(void);
void move_projectile(void);
void animate_sprites(void);
void init_sprites(void);
void draw_towers(void);
void draw_health(SDL_Surface *s, int x, int y, int cur, int max);
void add_tower(int x, int y, int type);
int has_tower(int x, int y);
void shoot_towers(void);
void draw_tower(int x, int y);
void draw_enemy(int x, int y);
void draw_projectile(int x, int y);

const struct tower tower_definitions[2];

#endif /* __SPRITES_H__ */
