AM_CFLAGS = -Wall
bin_PROGRAMS = awesometd
awesometd_SOURCES = main.c eventloop.c video.c video-game.c game.c
awesometd_LDADD = -lGL $(SDL_LIBS)

awesometd_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
awesometd_CFLAGS = $(SDL_CFLAGS)
