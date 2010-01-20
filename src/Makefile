#    Awesome Tower Defense
#    Copyright (C) 2008-2010  Trygve Vea and contributors (read AUTHORS)
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

SOURCES = main.c video.c eventloop.c

OBJS = $(SOURCES:%.c=%.o)

CFLAGS = -Wall -pipe

ifdef RELEASE
	CFLAGS += -O2 -fomit-frame-pointer
else
	CFLAGS += -pg -ggdb -O0
endif

TARGET = awesometd
CC = gcc
CFLAGS += $(shell sdl-config --cflags)
LDFLAGS += -lm $(shell sdl-config --libs) -lGL -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
