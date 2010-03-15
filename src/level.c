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

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>

#include "level.h"
#include "game.h"

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
    GKeyFile *keyfile;
    GError *error = NULL;
    gsize c1, c2;
    gint i,y;
    gchar **groups, **keys;

    gint *map,w,h;
    gchar *mapname;

    EnemyFreeAll();

    keyfile = g_key_file_new();

    if ( !g_key_file_load_from_file(keyfile,filename,G_KEY_FILE_NONE,&error) )
    {
        printf("Unable to load level because: %s\n", error->message);
        return -1;
    }
    groups = g_key_file_get_groups(keyfile,&c1);

    for (i=0;i<c1;i++)
    {
//        printf("groupname: %s\n", groups[i]);
        keys = g_key_file_get_keys(keyfile,groups[i],&c2,&error);
        if ( g_pattern_match_simple("LevelHeader",groups[i]) )
        {
            gsize mapsize = 0;
            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("width",keys[y]) )
                    w = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("height",keys[y]) )
                    h = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("map",keys[y]) )
                    map = g_key_file_get_integer_list(keyfile,groups[i],keys[y],&mapsize,&error);
                if ( g_pattern_match_simple("name",keys[y]) )
                    mapname = g_key_file_get_string(keyfile,groups[i],keys[y],&error);
            }
//            printf("Width: %d, Height: %d\n",w,h);
//            printf("The map contained %d elements! (%d * %d = %d)\n",(int)mapsize,w,h,w*h);
//            printf("The map is named '%s'\n",mapname);
//          TODO: We shouldn't assume that the map is valid.
        }
        else if ( g_pattern_match_simple("Enemy_*",groups[i]) )
        {
            Enemy e;
            int id;
            sscanf(groups[i],"Enemy_%d",&id);
//            printf("Processing enemy-template (#%d)...\n",id);

            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("name",keys[y]) )
                    e.name = g_key_file_get_string(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("speed",keys[y]) )
                    e.speed = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("maxhp",keys[y]) )
                    e.max_hp = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("gfx",keys[y]) )
                    printf(""); 
                    // ^ At this point we will look up in a hashtable of loaded graphics. If we
                    // have a key corresponding to the name described here, we will use that.
                    // Otherwise, we will fall back to a default graphic.
            }
            EnemyTemplateAdd(id,&e);
        }
        else if ( g_pattern_match_simple("Wave_*",groups[i]) )
        {
            int id;
            gsize intervals, types;
            Wave *w = g_malloc(sizeof(Wave));
            sscanf(groups[i],"Wave_%d",&id);
            printf("Processing wave... (#%d)\n",id);
            w->blowup = 0;
            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("start",keys[y]) )
                    w->start = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("intervals",keys[y]) )
                    w->intervals = g_key_file_get_integer_list(keyfile,groups[i],keys[y],
                                                               &intervals,&error);
                if ( g_pattern_match_simple("types",keys[y]) )
                    w->types = g_key_file_get_integer_list(keyfile,groups[i],keys[y],
                                                           &types,&error);
            }
            w->enemies = types;
            printf("Wave starts at tick %d. Got %d intervals and %d types.\n",
                   w->start,intervals,types);
            WaveAdd(w);
        }
        g_strfreev(keys);
    }
    g_strfreev(groups);
    Level.map = map;
    Level.w = w;
    Level.h = h;
    LevelCamera[0] = 0;
    LevelCamera[1] = 64;
}
