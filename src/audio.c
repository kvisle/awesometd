#include <stdlib.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include "game.h"
#include "audio.h"

struct audio aSetup()
{
    struct audio a;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(2);
    }

    a.laser[0] = Mix_LoadWAV("share/sfx/wp_rifle_fire-01.wav");
    a.laser[1] = Mix_LoadWAV("share/sfx/wp_rifle_fire-02.wav");
    a.laser[2] = Mix_LoadWAV("share/sfx/wp_rifle_fire-03.wav");

    Mix_AllocateChannels(64);

    return a;
}

void aDo(struct game *g, struct audio *a)
{
    if ( g->st.laser && SDL_GetTicks() > g->st.laser_last + 90 )
    {
        aPlaySound(a->laser[rand() % 3]);
        g->st.laser = 0;
        g->st.laser_last = SDL_GetTicks();
    }
}

void aPlaySound(Mix_Chunk *a)
{
    if ( a )
    {
        if ( Mix_PlayChannel(-1, a, 0) == -1 )
        {
            printf("Mix_PlayChannel: %s\n", Mix_GetError());
        }

    }
}
