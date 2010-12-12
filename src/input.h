#ifndef __INPUT_H__
#define __INPUT_H__

#include "game.h"

// This feels wrong.
struct flags {
	int debug;
};

struct input {
	struct flags f;
	int pushCX, pushCY;
    int pushTID;
};

int iEventLoop(struct input *i, struct game *g);

#endif /* __INPUT_H__ */
