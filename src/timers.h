/*
    Awesome Tower Defense
    Copyright (C) 2008-2009  Trygve Vea and contributors (read AUTHORS)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <stdint.h>

#define TIMER_REPORTFPS         1
#define TIMER_GAMECYCLE         2
#define TIMER_RENDER            3

void init_timers(void);
uint32_t timeduserevent(uint32_t interval, void *param);
uint32_t timedrenderevent(uint32_t interval, void *param);
void mark_rendered(void);

#endif
