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

#include "video.h"
#include "level.h"
#include "game.h"

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
                if ( g_pattern_match_simple("money",keys[y]) )
                    Gamedata.money = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
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
                if ( g_pattern_match_simple("score",keys[y]) )
                    e.score = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("money",keys[y]) )
                    e.money = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("gfx",keys[y]) )
                {
                    gchar *tname = g_key_file_get_string(keyfile,groups[i],keys[y],&error);
                    Texture *t = g_hash_table_lookup(TextureTable,tname);
                    g_free(tname);
                    if ( t )
                    {
                        if ( t->texid == 0 )
                        {
                            *t = VideoLoadTexture(t->filename);
                        }
                        e.tex = t;
                    }
                }
            }
            e.frame = 0;
            e.progress = 0;
            e.rotation = 0;
            e.moved = 0;
            EnemyTemplateAdd(id,&e);
        }
        else if ( g_pattern_match_simple("Projectile_*",groups[i]) )
        {
            Projectile p;
            int id;
            sscanf(groups[i],"Projectile_%d",&id);

            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("name",keys[y]) )
                    p.name = g_key_file_get_string(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("speed",keys[y]) )
                    p.speed = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("damage",keys[y]) )
                    p.damage = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
            }
            ProjectileTemplateAdd(id,&p);
        }
        else if ( g_pattern_match_simple("Tower_*",groups[i]) )
        {
            Tower t;
            int id;
            sscanf(groups[i],"Tower_%d",&id);
            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("name",keys[y]) )
                    t.name = g_key_file_get_string(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("reloadtime",keys[y]) )
                    t.reloadtime = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("range",keys[y]) )
                    t.range = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("price",keys[y]) )
                    t.price = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("projectile",keys[y]) )
                {
                    gint z = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                    t.projectile = g_hash_table_lookup(Gamedata.ProjectileTemplates,&z);
                }
                if ( g_pattern_match_simple("gfx",keys[y]) )
                {
                    gchar *tname = g_key_file_get_string(keyfile,groups[i],keys[y],&error);
                    Texture *tt = g_hash_table_lookup(TextureTable,tname);
                    g_free(tname);
                    if ( tt )
                    {
                        if ( tt->texid == 0 )
                        {
                            *tt = VideoLoadTexture(tt->filename);
                        }
                        t.tex = tt;
                    }
                }
            }
            t.frame = 0;
            t.reloadtimeleft = 0;
            t.rotation = 0;
            TowerTemplateAdd(id,&t);
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
                if ( g_pattern_match_simple("sp",keys[y]) )
                    w->sp = g_key_file_get_integer_list(keyfile,groups[i],keys[y],
                                                        &types,&error);
            }
            w->enemies = types;
            printf("Wave starts at tick %d. Got %d intervals and %d types.\n",
                   w->start,intervals,types);
            w->tex = VideoLoadTexture("../share/gfx/wave.png");
            WaveAdd(w);
        }
        else if ( g_pattern_match_simple("Start_*",groups[i]) )
        {
            gint *id = g_malloc(sizeof(gint));
            StartPosition *sp = g_malloc(sizeof(StartPosition));
            sscanf(groups[i],"Start_%d",id);
            printf("Processing start position... (#%d)\n",*id);
            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("x",keys[y]) )
                    sp->x = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("y",keys[y]) )
                    sp->y = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("dir",keys[y]) )
                    sp->dir = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
            }
            g_hash_table_insert(Level.st,id,sp);
        }
        else if ( g_pattern_match_simple("End_*",groups[i]) )
        {
            EndPosition *ep = g_malloc(sizeof(EndPosition));
            for (y=0;y<c2;y++)
            {
                if ( g_pattern_match_simple("x",keys[y]) )
                    ep->x = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
                if ( g_pattern_match_simple("y",keys[y]) )
                    ep->y = g_key_file_get_integer(keyfile,groups[i],keys[y],&error);
            }
            Level.ep = g_slist_insert(Level.ep,ep,-1);
        }
        g_strfreev(keys);
    }
    g_strfreev(groups);
    Level.map = map;
    Level.w = w;
    Level.h = h;
    LevelCamera[0] = 64;
    LevelCamera[1] = 64;
}
