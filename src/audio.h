#ifndef __AUDIO_H__
#define __AUDIO_H__
#include <SDL_mixer.h>

struct audio {
    Mix_Chunk *laser[3];
};

struct audio aSetup();
void aDo(struct game *g, struct audio *a);

void aPlaySound(Mix_Chunk *a);

#endif /* __AUDIO_H__ */
