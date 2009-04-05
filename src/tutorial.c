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
#include "render.h"
#include "tutorial.h"
#include "gfx_charmap.h"
#include "gfx_buttons.h"
#include "macros.h"

static int hoverchoice = 0;
static char* tutorials[] = {
    "basic gameplay",
    "upgrading towers",
    "targetting algorithm",
    "hotkeys"
};
static int redraw = 1;
void show_tutorial(void) {
    current_screen = SCREEN_TUTORIAL;
    redraw = 1;
    update_all();
}

static int sprites_loaded = 0;

static SDL_Surface *tut_basic;
static SDL_Rect tut_basic_rect = { 200,160,144,128 };
static SDL_Surface *tut_upgrading;
static SDL_Rect tut_upgrading_rect = { 480,160,144,128 };

static SDL_Rect tower_dst = { 200,232,24,24 };
static SDL_Rect tower_src = { 0,0,24,24 };

DEFIMAGE(tut_basic)
DEFIMAGE(tut_upgrading)

static void load_sprites(void) {
    tut_basic = SDL_LoadBMP_RW(SDL_RWFromMem(IMAGE_BUF(tut_basic),IMAGE_SIZE(tut_basic)),0);
    tut_upgrading = SDL_LoadBMP_RW(SDL_RWFromMem(IMAGE_BUF(tut_upgrading),IMAGE_SIZE(tut_upgrading)),0);
    printf("sprites loaded!\n");
    sprites_loaded = 1;
}

void draw_tutorial(void) {
    int i;
    if ( sprites_loaded == 0 ) load_sprites();
    if ( redraw == 0 ) return;
    for (i=0;i<ARRAY_SIZE(tutorials);i++)
        if (hoverchoice != i) draw_text(screen,tutorials[i],16,160+(i*16));
        else draw_text_color(screen,tutorials[i],16,160+(i*16),255,255,0);

    update_all();
    switch(hoverchoice) {
        case 0:
                            //#################################
                            //#####################################################
            SDL_BlitSurface(tut_basic,NULL,screen,&tut_basic_rect);
            draw_text(screen,"enemies will move down a road, if",360,160);
            draw_text(screen,"they don't get killed before they",360,176);
            draw_text(screen,"get to the end you lose a life.",360,192);
            draw_text(screen,"the goal is to survive all the",360,208);
            draw_text(screen,"upcoming waves of enemies. when",360,224);
            draw_text(screen,"you kill an enemy, you recieve",360,240);
            draw_text(screen,"money and points for each kill.",360,256);
            draw_text(screen,"the amount of money you get only",360,272);
            draw_text(screen,"depend on the enemy you kill.",360,288);
            draw_text(screen,"the amount of points you get depend on how far the",200,304);
            draw_text(screen,"enemy have travelled along the route. earlier kill",200,320);
            draw_text(screen,"means more points.",200,336);
        break;
        case 1:
                            //#################################
                            //#####################################################
            SDL_BlitSurface(tut_upgrading,NULL,screen,&tut_upgrading_rect);
            draw_text(screen,"when a tower makes a killing blow",200,160);
            draw_text(screen,"it earns experience. the yellow",200,176);
            draw_text(screen,"bar indicates how far it is from",200,192);
            draw_text(screen,"an upgrade. when a tower is fully",200,208);
            draw_text(screen,"upgraded it will not gain more",200,224);
            draw_text(screen,"experience. the killing blow goes",200,240);
            draw_text(screen,"to the last upgradeable tower who",200,256);
            draw_text(screen,"made a hit on the enemy. the",200,272);
            draw_text(screen,"trick is to position the towers",200,288);
            draw_text(screen,"in a way that makes sure that as little experience",200,304);
            draw_text(screen,"as possible are wasted.",200,320);
        break;
        case 2:
                            //#################################
                            //#####################################################
//            SDL_Rect tower_dst = { 200,232,24,24 };
//            SDL_Rect tower_src = { 0,0,24,24 };
            draw_text(screen,"you can control how the towers determine which target",200,160);
            draw_text(screen,"to aim for. when you click a tower, a row of buttons",200,176);
            draw_text(screen,"will appear on the top of the screen. the buttons let",200,192);
            draw_text(screen,"you pick between 5 algorithms.",200,208);

            tower_dst.y = 232; tower_src.y = 24;
            SDL_BlitSurface(buttonsurface2,&tower_src,screen,&tower_dst);
            draw_text(screen,"shoot enemy travelled the least",232,240);

            tower_dst.y = tower_dst.y+32; tower_src.y = tower_src.y +24;
            SDL_BlitSurface(buttonsurface2,&tower_src,screen,&tower_dst);
            draw_text(screen,"shoot enemy travelled the farthest",232,272);

            tower_dst.y = tower_dst.y+32; tower_src.y = tower_src.y +24;
            SDL_BlitSurface(buttonsurface2,&tower_src,screen,&tower_dst);
            draw_text(screen,"shoot enemy with most hp",232,304);

            tower_dst.y = tower_dst.y+32; tower_src.y = tower_src.y +24;
            SDL_BlitSurface(buttonsurface2,&tower_src,screen,&tower_dst);
            draw_text(screen,"shoot enemy with least hp",232,336);

            tower_dst.y = tower_dst.y+32; tower_src.y = tower_src.y +24;
            SDL_BlitSurface(buttonsurface2,&tower_src,screen,&tower_dst);
            draw_text(screen,"shoot the fastest enemy",232,368);
        break;
        case 3:
                draw_text_color(screen,"1",232,160,0,255,255);
            draw_text(screen,"build pillbox",280,160);
                draw_text_color(screen,"2",232,176,0,255,255);
            draw_text(screen,"build artificial volcano",280,176);
                draw_text_color(screen,"3",232,192,0,255,255);
            draw_text(screen,"build freezer",280,192);
                draw_text_color(screen,"s",232,208,0,255,255);
            draw_text(screen,"sell tower",280,208);
                draw_text_color(screen,"q",232,224,0,255,255);
            draw_text(screen,"shoot enemy travelled the least",280,224);
                draw_text_color(screen,"w",232,240,0,255,255);
            draw_text(screen,"shoot enemy travelled the farthest",280,240);
                draw_text_color(screen,"e",232,256,0,255,255);
            draw_text(screen,"shoot enemy with most hp",280,256);
                draw_text_color(screen,"r",232,272,0,255,255);
            draw_text(screen,"shoot enemy with least hp",280,272);
                draw_text_color(screen,"t",232,288,0,255,255);
            draw_text(screen,"shoot the fastest enemy",280,288);
                draw_text_color(screen,"p",232,304,0,255,255);
            draw_text(screen,"pause tower",280,304);
                draw_text_color(screen,"space",232,320,0,255,255);
            draw_text(screen,"pause the game",280,320);
                draw_text_color(screen,"esc",232,336,0,255,255);
            draw_text(screen,"quit to menu",280,336);
        break;
    }
    redraw = 0;
    return;
}

void update_tutorial_selection(int x, int y) {
    if ( x >= 16 && x <= 176 && y >= 160 && y < (160+(16*4)) )
        hoverchoice = (y-160)/16;
    redraw = 1;
    return;
}
