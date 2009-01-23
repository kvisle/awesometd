PLATFORM = LINUX

OBJS = main.o render.o eventloop.o timers.o level.o sprites.o cursor.o gfx_charmap.o gfx_buttons.o game.o

CFLAGS = -ggdb -Wall -pg

ifeq ($(PLATFORM),LINUX)
	TARGET = atd
	CC = gcc
	CFLAGS += $(shell sdl-config --cflags)
	LDFLAGS += -lm $(shell sdl-config --libs)
endif
ifeq ($(PLATFORM),WIN32)
	TARGET = atd.exe
	CC = i586-mingw32msvc-gcc
	CFLAGS += $(shell /home/trygve/sdl/SDL-1.2.13/bin/sdl-config --cflags)
	LDFLAGS += $(shell /home/trygve/sdl/SDL-1.2.13/bin/sdl-config --libs)
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

all: $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)
