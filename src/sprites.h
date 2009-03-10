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

#define ALGORITHM_TRAVELLED_SHORTEST 1
#define ALGORITHM_TRAVELLED_FARTHEST 2
#define ALGORITHM_MOST_HP 3
#define ALGORITHM_LEAST_HP 4
#define ALGORITHM_FASTEST 5

struct monster {
    int pos_x, pos_y; // Exact coordinates, not the cell-coordinates...
                    // We need the exact ones, because we want smooth movements.
    int spid; // Sprite ID
    int frameno;
    int cur_hp, max_hp;
    int upcoming_damage;
    int speed;
    int direction;
    int progress;
    int travelled;
    int score;
    int money;
    Uint32 last_ice_shot;
    float effect_speed;
    float upcoming_effect_speed;
    int exp_gainer;
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
    int target_algorithm;
    int exp;
    int max_exp;
    int next_tower;
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
    float effect_speed;
    int owner;
};

void reset_everything(void);
void draw_sprite(SDL_Surface *s, int spid, int fid, int rot, int x, int y);
int spawn_monster(void);
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
void select_tower(int x, int y);
void sell_tower(int tid);
void tower_algorithm(int tid, int aid);
const struct tower tower_definitions[9];
struct tower towers[MAX_TOWERS];
int selected_tower;
void upgrade_tower(int tid);

#endif /* __SPRITES_H__ */
