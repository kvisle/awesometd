AM_CFLAGS = -Wall
bin_PROGRAMS = awesometd
awesometd_SOURCES = game.c input.c main.c pathfind.c video.c parser.c menu.c #audio.c
awesometd_LDADD = $(GL_LIBS) \
                  $(SDL_LIBS) \
                  $(SDL_image_LIBS)

EXTRA_DIST = game.h input.h video.h

#awesometd_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
awesometd_CFLAGS = -g $(GL_CFLAGS) $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS)
