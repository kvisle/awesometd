#include <stdio.h>

#include <SDL.h>

#include "video.h"
#include "input.h"
#include "game.h"

struct main {
	int last_draw;
	int f;
};

struct all {
	struct video v;
	struct input i;
	struct game g;

	struct main m;
};

int main(int argc, char *argv[])
{
	struct all all;

	// Is there any way I can make sure that all is 0 without doing this manually?
	memset(&all, '\0', sizeof(struct all));

	all.v = vSetup();
	all.g = gNew();

    while ( !iEventLoop(&all.i, &all.g) ) // Inputhandling is done here.
    {
		// Moving the game forward is done here.
		gDo(&all.g);

		// Drawing is done here.
		vDraw(&all.v, &all.g);

		all.m.f++;
		if ( all.i.f.debug )
			printf("Frame: %d\n", all.m.f);

        int t = (1000/FPS)-(SDL_GetTicks()-all.m.last_draw);
        if ( t > 0 && t < 1000 ) SDL_Delay(t);
        all.m.last_draw = SDL_GetTicks();
    }

	return 0;
}
