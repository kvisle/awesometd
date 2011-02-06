/* parser, a simple key->value parser.
 * Copyright (C) 2011  Trygve Vea <trygve.vea@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

#define PUSH_INT(param) \
    do { \
        if ( strcmp(key, # param) == 0 ) \
            data->param = strtol(value, NULL, 10); \
    } while(0)

#define PUSH_INT_ARRAY(param, max) \
    do { \
        if ( strcmp(key, # param) == 0 ) \
        { \
            int n = 0; \
            char *p = value; \
            while (*p != '\0' && n < max) \
            { \
                data->map[n] = strtol(p, &p, 10); \
                n++; \
                if ( *p != '\0' ) \
                    p++; \
            } \
        } \
    } while(0)

#define PUSH_STR(param) \
    do { \
        if ( strcmp(key, # param) == 0 ) \
            data->param = strdup(value); \
    } while(0)


void parse(char *fn, void parser(), void *data);
