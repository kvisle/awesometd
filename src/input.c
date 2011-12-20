#include <SDL.h>

#include "input.h"

#include "game.h"

// Keyboard event handling.
static int iKeyEvent(SDL_KeyboardEvent k, struct input *i)
{
// This currently doesn't make any sense...
	if ( k.keysym.sym == 'd' )
		i->f.debug = !i->f.debug;
	return 0;
}

static void iMouseMotionEvent(SDL_MouseMotionEvent e, struct input *i, struct game *g, struct menu *m)
{
    if ( g->state == GAMESTATE_MENU )
    {
        switch(m->currentmenu)
        {
            case MENU_MAINMENU:
                if ( e.x >= 60 && e.y >= 170 && e.x <= 204 && e.y <= 266 )
                    m->hovering = (e.y - 170) / 16;
                else
                    m->hovering = -1;
                break;
            case MENU_LEVELSELECT:
                if ( e.x >= 80 && e.y >= 150 && e.x <= 319 && e.y <= 406 )
                {
                    if ( ((e.y-150)/16)+(32*m->level_p) < m->levels->e_n )
                        m->level_h = ((e.y-150)/16)+(32*m->level_p);
                }
                if ( e.x >= 340 && e.y >= 150 && e.x <= 579 && e.y <= 406 )
                {
                    if ( ((e.y-150)/16)+(32*m->level_p)+16 < m->levels->e_n )
                        m->level_h = ((e.y-150)/16)+(32*m->level_p)+16;
                }
                if ( e.x >= 70 && e.y >= 440 && e.x <= 590 && e.y <= 450 )
                {
                    if ( (e.x-70) / 20 <= m->levels->e_n/32 )
                        m->level_p = (e.x-70) / 20;
                }
                break;
        }

        return;
    }
}

static void iMouseButtonEvent(SDL_MouseButtonEvent b, struct input *i, struct game *g, struct menu *m)
{
    int n;
    struct tower *s;

    if ( g->state == GAMESTATE_MENU )
    {
        if ( b.type != SDL_MOUSEBUTTONDOWN )
            return;
        if ( m->currentmenu != MENU_MAINMENU && b.button == SDL_BUTTON_RIGHT )
            m->currentmenu = MENU_MAINMENU;
        else if ( m->currentmenu == MENU_LEVELSELECT && b.button == SDL_BUTTON_LEFT )
        {
            if ( b.x >= 80 && b.y >= 150 && b.x <= 579 && b.y <= 406 )
            {
                *g = gNew(m->levels->e[m->level_h].fullname);
                g->state = GAMESTATE_INGAME;
            }
        }
        else if ( m->currentmenu == MENU_MAINMENU && m->hovering == 0 )
        {
//            *g = gNew("share/level/level1.lvl");
//            g->state = GAMESTATE_INGAME;
            m->currentmenu = MENU_LEVELSELECT;
        }
        else if ( m->currentmenu == MENU_MAINMENU && m->hovering == 3 )
            system("xdg-open http://trygvevea.com/awesometd");
        else if ( m->currentmenu == MENU_MAINMENU && m->hovering == 4 )
            m->quit = 1;

        m->hovering = -1;
        return;
    }


	if ( b.x >= 512 )
	{
        if ( b.type == SDL_MOUSEBUTTONDOWN )
        {
            n = 0;
            s = g->towerT;
            while(s)
            {
                if ( b.x >= 528 &&
                     b.x <= 628 &&
                     b.y >= 128+(n*24) &&
                     b.y <= 148+(n*24) )
                {
                    i->pushTID = n;
                    return;
                }
                n++;
                s = s->next;
            }
        }
        else if ( b.type == SDL_MOUSEBUTTONUP &&
                  b.x >= 528 &&
                  b.x <= 628 &&
                  b.y >= 128+(i->pushTID*24) &&
                  b.y <= 148+(i->pushTID*24) )
        {
            s = g->towerT;
            for (n=0;n<i->pushTID;n++)
            {
                if ( s == NULL ) break;
                s = s->next;
            }
            g->towerS = s;
        }
		puts("Clicked sidebar");
		return;
	}
	if ( b.type == SDL_MOUSEBUTTONUP && b.x / 32 == i->pushCX && b.y / 32 == i->pushCY)
	{
		gClickCell(g, b.x/32, b.y/32);
//		printf("Clicked cell: %d x %d\n", b.x/32, b.y/32);
		i->pushCX = i->pushCY = -1;
	}
	if ( b.type == SDL_MOUSEBUTTONDOWN )
	{
		i->pushCX = b.x / 32;
		i->pushCY = b.y / 32;
	}
}

// The main event loop.
int iEventLoop(struct input *i, struct game *g, struct menu *m)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				return 1;
				break;
			case SDL_KEYDOWN:
				iKeyEvent(event.key, i);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				iMouseButtonEvent(event.button, i, g, m);
				break;
            case SDL_MOUSEMOTION:
                iMouseMotionEvent(event.motion, i, g, m);
                break;
			default:
				break;
		}
	}
	return 0;
}
