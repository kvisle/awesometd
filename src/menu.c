#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "menu.h"

static struct charelement * mPopulateLevelList(void)
{
    char *p;
    struct charelement *te = calloc('\0', sizeof(struct charelement));
    struct charelement *ce = te;
    struct dirent *ent;
    DIR *dir;

    dir = opendir("share/level");
    if (!dir)
    {
        printf("Error: Can't find directory with levels.!\n");
        exit(1);
    }
    while ((ent = readdir(dir)) != NULL) {
        if ( ent->d_type == DT_REG )
        {
            p = strstr(ent->d_name, ".lvl");
            if ( p )
            {
                if ( ce->text )
                {
                    ce->next = calloc('\0', sizeof(struct charelement));
                    ce = ce->next;
                }
                *p = '\0';
                ce->text = strdup(ent->d_name);
            }
        }
    }
    closedir(dir);

    if ( !te->text )
    {
        free(te);
        return NULL;
    }
    return te;
}

struct menu mSetup(void)
{
    struct menu m;
    m.hovering = -1;
    m.quit = 0;
    m.mainmenu[0] = "new game";
    m.mainmenu[1] = "leaderboards";
    m.mainmenu[2] = "tutorial";
    m.mainmenu[3] = "visit our homepage";
    m.mainmenu[4] = "quit";
    m.levels = mPopulateLevelList();

    return m;
}

void mDo(struct menu *m)
{
    return;
}
