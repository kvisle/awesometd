/*
    Awesome Tower Defense
    Copyright (C) 2008-2010  Trygve Vea and contributors (read AUTHORS)

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

#include <stdlib.h>
#include <stdio.h>

#include "level.h"

char *MapMalloc(int size)
{
	return malloc(size);
}

void MapFree(char *ptr)
{
	free(ptr);
}

int LevelLoad(char *filename)
{
	int n,i = 0;
	char *t_map;
	FILE *f = fopen(filename, "r");
	if ( fscanf(f,"%dx%d",&Level.w,&Level.h) != 2 ) return -1;

	printf("Brettet er %d x %d stort!\n",Level.w, Level.h);
	t_map = MapMalloc(Level.w*Level.h);

	while ( fscanf(f,"%d",&n) == 1 )
	{
        if ( i % (Level.w * Level.h ) == 0 && i > 0) {
            printf("brettet er for stort\n");
			MapFree(t_map);
            return -1;
		}
		switch(n)
		{
			case 0: t_map[i] = 'X'; printf("X"); break;
			case 1: t_map[i] = '.'; printf("."); break;
			case 2: t_map[i] = 's'; printf("s"); break;
			case 3: t_map[i] = 'e'; printf("e"); break;
		}
		i++;
		if ( i % Level.w == 0 ) printf("\n");
	}
	if ( i % (Level.w * Level.h) == 0 )
	{
		printf("brettet er passe!\n");
		Level.map = t_map;
/*		int x,y;
		for (y=0;y<Level.h;y++)
		{
			for (x=0;x<Level.w;x++)
			{
				printf("%c",t_map[x+(y*Level.w)]);
			}
			printf("\n");
		}*/
		return 0;
	}
	else
	{
		printf("brettet er for lite!\n");
		MapFree(t_map);
		return -1;
	}
	fclose(f);
}
