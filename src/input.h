#ifndef __INPUT_H__
#define __INPUT_H__

#include "game.h"
#include "menu.h"

// This feels wrong.
struct flags {
	int debug;
};

struct input {
	struct flags f;
	int pushCX, pushCY;
    int pushTID;
};

int iEventLoop(struct input *i, struct game *g, struct menu *m);

#endif /* __INPUT_H__ */
