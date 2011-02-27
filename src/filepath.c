/* filepath, a simple 'path' searcher.
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
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "filepath.h"

#define CHUNK_SIZE 32

/*
 *  Crawls a path for files, and stores the information in the structure.
 *  Will dynamically reallocate memory to make sure there's room.
 */
static void
fpEnumeratePath(FilePath_t *fp, const char *path)
{
    struct dirent *entry;
    DIR *dp;
    int pl = strlen(path);
    int len;

    /*
     *  Open the directory.
     */
    dp = opendir(path);
    if (!dp)
    {
        fprintf(stderr, "Error: Unable to open %s\n", path);
        return;
    }

    /*
     *  Iterate the directory contents.
     */
    while ((entry = readdir(dp)))
    {
        /*
         *  We don't consider directories valid resources.  We could probably
         *  strip other things as well, such as character devices, socket
         *  files, and whatnot.
         */
#ifndef WIN32
// dirent on Windows is crap / nonexistent.  TODO: Fix this.
        if ( entry->d_type == DT_DIR )
            continue;
#else
        if ( entry->d_name[0] == '.' )
            continue;
#endif

        /*
         *  If we've filled our buffer, we double its size.
         */
        if ( fp->e_n == fp->e_max ) {
            fp->e_max *= 2;
            fp->e = realloc(fp->e, sizeof(FilePathElement_t) * fp->e_max);
        }

        /*
         *  Store information somewhere.
         */
        len = strlen(entry->d_name) + pl + 2;
        fp->e[fp->e_n].path = path;
        fp->e[fp->e_n].filename = strdup(entry->d_name);
        fp->e[fp->e_n].fullname = malloc(len);
        snprintf(fp->e[fp->e_n].fullname, len, "%s/%s", path, entry->d_name);
        
        fp->e_n++;
    }

    /*
     *  Close the directory.
     */
    closedir(dp);
}


/*
 *  This code removes duplicates.  The most low-hanging fruits for optimization
 *  resides here.
 */
static void
fpStripDups(FilePath_t *fp)
{
    int i, y;
    for (i=0; i<fp->e_n; i++)
    {
        for (y=i+1; y<fp->e_n; y++)
        {
            /*
             *  If the path-pointers point to the same place, we consider the
             *  files to be different.
             */
            if ( fp->e[i].path == fp->e[y].path )
                continue;
            /*
             *  Are the file names the same?
             */
            if ( 
                strcmp(fp->e[i].filename, fp->e[y].filename) == 0 )
            {
                /*
                 *  Free the element, and cover the hole in the array.
                 *  It may be better to do this in one run at the very end.
                 */
                free(fp->e[y].filename);
                free(fp->e[y].fullname);
                memmove(
                        &(fp->e[y]), 
                        &(fp->e[y+1]), 
                        sizeof(FilePathElement_t)*(fp->e_max-y)
                    );
                fp->e_n--;
                y--;
            }
        }
    }
}

/*
 *  If the meta-function is provided, we want to populate the structure with
 *  metadata.
 */
static void
fpPopulateMeta(FilePath_t *fp, void* meta(char * fn))
{
    int i;
    for (i=0; i<fp->e_n; i++)
    {
        fp->e[i].meta = meta(fp->e[i].fullname);
    }
}


/*
 *  A simple string-sorting function for qsort.  Nothing special.
 */
static int
fpSortFileName(const void *p1, const void *p2)
{
    const FilePathElement_t *fp1 = p1;
    const FilePathElement_t *fp2 = p2;
    return strcmp(fp1->filename, fp2->filename);
}

/*
 *  Returns a pointer to a FilePath_t structure.
 *
 *  This structure should be freed with fpFreeFilePath()
 *
 *  Takes a list of paths as its argument, and optionally a meta-function.
 */
FilePath_t *
fpEnumerate(const char *paths[], void* meta(char * fn))
{
    FilePath_t * fp = calloc(1, sizeof(FilePath_t));
    int i = 0;
    const char *p = paths[i];

    fp->e_max = CHUNK_SIZE;
    fp->e = malloc(sizeof(FilePathElement_t)*fp->e_max);
    fp->e_n = 0;

    /*
     *  Foreach path, enumerate path.
     */
    while ( p )
    {
        fpEnumeratePath(fp, p);
        i++;
        p = paths[i];
    }

    /*
     *  Remove duplicate file names.
     */
    fpStripDups(fp);

    /*
     *  If meta-function is provided, populate metadata.
     */
    if ( meta )
        fpPopulateMeta(fp, meta);

    /*
     *  Shrink the structure size to the amount of elements in the array.
     */
    fp->e_max = fp->e_n;
    fp->e = realloc(fp->e, sizeof(FilePathElement_t)*fp->e_max);

    /*
     *  Sort the array alphabetically based on file name.
     */
    qsort(fp->e, fp->e_n, sizeof(FilePathElement_t), fpSortFileName);

    return fp;
}

/*
 *  Frees the FilePath_t, and everything that has been allocated with it.
 *
 *  If you supply a metafree-function, it will be called with each meta-value
 *  as a parameter.
 */
void
fpFreeFilePath(FilePath_t *fp, void metafree(void * m))
{
    int i;
    for(i=0; i<fp->e_n; i++)
    {
        if ( metafree )
            metafree(fp->e[i].meta);
        free(fp->e[i].filename);
        free(fp->e[i].fullname);
    }
    free(fp->e);
    free(fp);
}

