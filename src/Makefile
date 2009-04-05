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

SOURCES = credits.c game.c highscores.c menu.c timers.c cursor.c gfx_buttons.c level.c render.c tutorial.c eventloop.c gfx_charmap.c main.c sprites.c
BMPS = buttonbar.bmp enemy3.bmp tower1-2.bmp tower2-3.bmp tower3.bmp buttonbar2.bmp leveltiles.bmp tower1-3.bmp tower2.bmp enemy1.bmp menuscreen.bmp tower1.bmp tower3-2.bmp tut_basic.bmp enemy2.bmp minipause.bmp tower2-2.bmp tower3-3.bmp tut_upgrading.bmp

OBJS = $(SOURCES:%.c=%.o)
OBJS += $(BMPS:%.bmp=%.o)

CFLAGS = -Wall -pipe
ifdef RELEASE
	CFLAGS += -O2 -fomit-frame-pointer
else
	CFLAGS += -pg -ggdb -O0
endif

ifeq ($(PLATFORM),LINUX)
	TARGET = awesometd
	CC = gcc
	CFLAGS += $(shell sdl-config --cflags)
	LDFLAGS += -lm $(shell sdl-config --libs)
	OBJCOPY += objcopy -I binary -O elf32-i386 -B i386
endif
ifeq ($(PLATFORM),WIN32)
	TARGET = awesometd.exe
	CC = i586-mingw32msvc-gcc
	WINDRES := i586-mingw32msvc-windres
	CFLAGS += $(shell /home/trygve/sdl/SDL-1.2.13/bin/sdl-config --cflags)
	LDFLAGS += $(shell /home/trygve/sdl/SDL-1.2.13/bin/sdl-config --libs)
	OBJCOPY += i586-mingw32msvc-objcopy -I binary -O pe-i386 -B i386
	OBJS += res.o
endif

all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.bmp
	$(OBJCOPY) $< $@

res.o: res.rc icon.ico
	$(WINDRES) -o $@ res.rc

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
