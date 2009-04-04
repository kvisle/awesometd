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

PLATFORM = WIN32

OBJS = main.o render.o eventloop.o timers.o level.o sprites.o cursor.o gfx_charmap.o gfx_buttons.o game.o menu.o credits.o highscores.o tutorial.o
OBJS += buttonbar2.o buttonbar.o enemy1.o enemy2.o enemy3.o leveltiles.o menuscreen.o minipause.o tower1-2.o tower1-3.o tower1.o tower2-2.o tower2-3.o tower2.o tower3-2.o tower3-3.o tower3.o tower4.o tut_basic.o tut_upgrading.o


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
