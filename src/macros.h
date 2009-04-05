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

#ifndef __MACROS_H__
#define __MACROS_H__

#ifdef WIN32

#define DEFIMAGE(image) \
    extern char binary_ ## image ## _bmp_start; \
    extern char binary_ ## image ## _bmp_end;

#define IMAGE_BUF(image)    (&binary_ ## image ## _bmp_start)
#define IMAGE_SIZE(image)   (&binary_ ## image ## _bmp_end - &binary_ ## image ## _bmp_start)

#else /* ! WIN32 */

#define DEFIMAGE(image) \
    extern char _binary_ ## image ## _bmp_start; \
    extern char _binary_ ## image ## _bmp_end;

#define IMAGE_BUF(image)    (&_binary_ ## image ## _bmp_start)
#define IMAGE_SIZE(image)   (&_binary_ ## image ## _bmp_end - &_binary_ ## image ## _bmp_start)

#endif /* WIN32 */

#endif /* __MACROS_H__ */
