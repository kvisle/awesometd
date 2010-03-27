AM_CFLAGS = -Wall
bin_PROGRAMS = awesometd
awesometd_SOURCES = main.c eventloop.c video.c video-game.c game.c level.c
awesometd_LDADD = $(GL_LIBS) \
                  $(GLU_LIBS) \
                  $(SDL_LIBS) \
                  $(SDL_image_LIBS) \
                  $(freetype_LIBS) \
                  $(SDL_ttf_LIBS) \
                  $(GLIB_LIBS) 

#awesometd_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
awesometd_CFLAGS = -g $(GL_CFLAGS) $(GLU_CFLAGS) $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_ttf_CFLAGS) $(GLIB_CFLAGS)
