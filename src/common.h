#ifndef __COMMON_H__
#define __COMMON_H__

#include <glib.h>

static char *BaseDirectories[] = {
    DATADIR,
    "../share",
    "./share",
    ".",
    "NULL"
};

GHashTable *TextureTable, *FontTable, *LevelTable;

#endif /* __COMMON_H__ */
