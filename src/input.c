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

static void iMouseButtonEvent(SDL_MouseButtonEvent b, struct input *i, struct game *g)
{
    int n;
    struct tower *s;
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
int iEventLoop(struct input *i, struct game *g)
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
				iMouseButtonEvent(event.button, i, g);
				break;
			default:
				break;
		}
	}
	return 0;
}
