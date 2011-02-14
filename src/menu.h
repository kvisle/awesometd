#ifndef __MENU_H__
#define __MENU_H__

#define MENU_MAINMENU       0
#define MENU_LEVELSELECT    1
#define MENU_LEADERBOARDS   2
#define MENU_TUTORIAL       3

#define MAINMENU_ELEMENTS   5

struct charelement {
    char *text;
    struct charelement *next;
};

struct menu {
    int currentmenu;
    int hovering;
    int quit;
    char *mainmenu[MAINMENU_ELEMENTS];
    struct charelement *levels;
};

void mDo(struct menu *m);
struct menu mSetup();

#endif /* __MENU_H__ */
