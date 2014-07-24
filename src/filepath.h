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
#ifndef __FILEPATH_H__
#define __FILEPATH_H__

typedef struct {
    char *filename;
    char *fullname;
    const char *path;
    void *meta;
} FilePathElement_t;

typedef struct {
    FilePathElement_t *e;
    int e_n;
    int e_max;
} FilePath_t;

FilePath_t *
fpEnumerate(const char *paths[], void* meta(char * fn));

void
fpFreeFilePath(FilePath_t *fp, void metafree(void * m));

#endif /* __FILEPATH_H__ */
