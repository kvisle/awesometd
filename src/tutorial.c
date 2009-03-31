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

static int hoverchoice = 0;
static char* tutorials[] = {
    "basic gameplay",
    "upgrading towers",
    "targetting algorithm"
};
static int redraw = 1;
void show_tutorial(void) {
    current_screen = SCREEN_TUTORIAL;
    update_all();
}
void draw_tutorial(void) {
    int i;
    if ( redraw == 0 ) return;
    for (i=0;i<3;i++)
        if (hoverchoice != i) draw_text(screen,tutorials[i],16,160+(i*16));
        else draw_text_color(screen,tutorials[i],16,160+(i*16),255,255,0);

    update_all();
    switch(hoverchoice) {
        case 0:
                            //#################################
                            //#####################################################
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
            draw_text(screen,"you can control how the towers determine which target",200,160);
            draw_text(screen,"to aim for. when you click a tower, a row of buttons",200,176);
            draw_text(screen,"will appear on the top of the",360,192);
            draw_text(screen,"screen. the buttons let you pick",360,208);
            draw_text(screen,"between 5 algorithms. *",360,224);
        break;
    }
    redraw = 0;
    return;
}

void update_tutorial_selection(int x, int y) {
    if ( x >= 16 && x <= 176 && y >= 160 && y < (160+(16*3)) )
        hoverchoice = (y-160)/16;
    redraw = 1;
    return;
}
