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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#include "parser.h"

void parse(char *fn, void parser(), void *data)
{
    char buffer[BUFFER_LEN];
    char *kp, *vp, *wp;
    int line = 0;

    FILE *cf = fopen(fn, "r");

    assert(cf);

    while ( fgets((char *)&buffer, BUFFER_LEN, cf) )
    {
        line++;
        kp = vp = wp = (char *) &buffer;

        // Line is empty, or a comment.
        if ( *wp == '#' ||
             *wp == '\0' ) continue;

        while(*wp != '\0')
        {
            switch(*wp)
            {
            case '\n':
                *wp = '\0';
                break;
            case '=':
                if ( kp == vp )
                {
                    vp = wp+1;
                    *wp = '\0';
                }
                break;
            }
            wp++;
        }
        if ( kp != vp )
        {
            parser(data, kp, vp);
        }
        else {
            fprintf(stderr, "Didn't understand line %d in %s\n", line, fn);
        }
    }

    fclose(cf);
}
