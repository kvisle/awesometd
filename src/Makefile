#    Awesome Tower Defense
#    Copyright (C) 2008-2009  Trygve Vea and contributors (read AUTHORS)
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

PLATFORM = LINUX

OBJS = main.o render.o eventloop.o timers.o level.o sprites.o cursor.o gfx_charmap.o gfx_buttons.o game.o menu.o credits.o highscores.o

CFLAGS = -Wall -pipe
ifdef RELEASE
	CFLAGS += -march=native -O2 -fomit-frame-pointer -funroll-loops
else
	CFLAGS += -pg -ggdb -O0
endif

ifeq ($(PLATFORM),LINUX)
	TARGET = awesometd
	CC = gcc
	CFLAGS += $(shell sdl-config --cflags)
	LDFLAGS += -lm $(shell sdl-config --libs)
endif
ifeq ($(PLATFORM),WIN32)
	TARGET = awesometd.exe
	CC = i586-mingw32msvc-gcc
	CFLAGS += $(shell /home/trygve/sdl/SDL-1.2.13/bin/sdl-config --cflags)
	LDFLAGS += $(shell /home/trygve/sdl/SDL-1.2.13/bin/sdl-config --libs)
endif

all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
