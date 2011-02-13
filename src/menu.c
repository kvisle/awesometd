#include <stdlib.h>
#include "menu.h"

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
    
    return m;
}

void mDo(struct menu *m)
{
    return;
}
