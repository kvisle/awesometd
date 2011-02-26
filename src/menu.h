#ifndef __MENU_H__
#define __MENU_H__

#define MENU_MAINMENU       0
#define MENU_LEVELSELECT    1
#define MENU_LEADERBOARDS   2
#define MENU_TUTORIAL       3

#define MAINMENU_ELEMENTS   5

#include "filepath.h"

struct menuelement {
    char *text;
    int score;
    struct menuelement *next;
};

struct menu {
    int currentmenu;
    int hovering;
    int quit;
    char *mainmenu[MAINMENU_ELEMENTS];
    FilePath_t *levels;
//    struct menuelement *levels;
    int level_p, level_h;
};

void mDo(struct menu *m);
struct menu mSetup();

#endif /* __MENU_H__ */
