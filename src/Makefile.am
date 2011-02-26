AM_CFLAGS = -Wall
bin_PROGRAMS = awesometd
awesometd_SOURCES = game.c input.c main.c pathfind.c video.c parser.c menu.c hiscore.c filepath.c #audio.c
awesometd_LDADD = $(GL_LIBS) \
                  $(SDL_LIBS) \
                  $(SDL_image_LIBS) \
                  $(sqlite3_LIBS)

EXTRA_DIST = game.h input.h pathfind.h video.h parser.h menu.h hiscore.h

#awesometd_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
awesometd_CFLAGS = -g $(GL_CFLAGS) $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(sqlite3_CFLAGS)
