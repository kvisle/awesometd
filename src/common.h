#ifndef __COMMON_H__
#define __COMMON_H__

#include <glib.h>

#ifdef DEFBASEDIRS
static char *BaseDirectories[] = {
    DATADIR,
    "../share",
    "./share",
    ".",
    "NULL"
};
#endif

GHashTable *TextureTable, *FontTable, *LevelTable;

#endif /* __COMMON_H__ */
