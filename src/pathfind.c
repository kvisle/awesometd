#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game.h"

#define HEAD(q) (q->e[q->head])
#define TAIL(q) (q->e[q->tail])

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

struct grid_t {
    int w, h;
    int *ar;
    int *path;
    int *marked;
    size_t size;
};

struct queue_element_t {
    int id;
    int next;
};

struct queue_t {
    int head;
    int tail;
    struct queue_element_t *e;
};

static void push(struct queue_t *queue, struct grid_t *grid, const
                 int id, const int next);


static void
checkfield(struct queue_t *q, struct grid_t *g)
{
    int cursor = HEAD(q).id;
    // Write the amount of steps from the goal to this cell.
    g->path[cursor] = HEAD(q).next;

    // Check if the cell is a walkable area, return if it isn't.
    if ((g->ar[cursor] != 0) && (g->ar[cursor] != 255) && g->ar[cursor] != 254) {
//        if (g->ar[cursor] == 1||g->ar[cursor] == )
            g->path[cursor] = -1;
        return;
    }

    // Check the cell to the right.
    if (g->marked[cursor+1] == 0 &&
         (cursor+1) % g->w != 0 &&
         (cursor+1) < (signed int)g->size )
        push(q, g, cursor+1, HEAD(q).next + 1);

    // Check the cell to the left.
    if (g->marked[cursor-1] == 0 &&
         (cursor) % g->w != 0 &&
         (cursor-1) >= 0)
        push(q, g, cursor-1, HEAD(q).next + 1);

    // Check the cell below.
    if (g->marked[cursor + g->w] == 0 &&
         (cursor + g->w) < (signed int)g->size)
        push(q, g, cursor + g->w, HEAD(q).next + 1);

    // Check the cell above.
    if (g->marked[cursor - g->w] == 0 &&
        (cursor - g->w) >= 0)
        push(q, g, cursor - g->w, HEAD(q).next + 1);
}

// Pushing information about a cell to be checked to our queue.
static void
push(struct queue_t *queue, 
     struct grid_t *grid, 
     const int id, 
     const int next)
{
    queue->e[queue->tail].id = id;
    queue->e[queue->tail].next = next;
    grid->marked[id]++;
    queue->tail++;
}

/* 
 * Parse the grid input and allocate a matching memory segment
 *
 * XXX: Cheapo-error handling
 * XXX: Now that reset_grid exists, calloc is redundant.
 */
static struct grid_t *
copy_grid(struct game *g)
{
    struct grid_t *grid;
    int x,y,i;

    grid = malloc(sizeof(struct grid_t));

    grid->size = G_SIZE;
	grid->w = G_WIDTH;
	grid->h = G_HEIGHT;

    grid->ar = calloc(grid->size, sizeof(int));
    grid->path = calloc(grid->size, sizeof(int));
    grid->marked = calloc(grid->size, sizeof(int));

    // Reads in the level from stdin.
    for(y=0; y < G_HEIGHT; y++) {
		for(x=0; x < G_WIDTH; x++) {
			grid->ar[i] = g->grid[y][x];
			i++;
		}
    }
    return grid;
}

/*
 * Free grid
 */
static void
free_grid(struct grid_t *g)
{
    free(g->ar);
    free(g->path);
    free(g->marked);
    free(g);
}

/*
 * Reset the queue, as if it was empty.
 */
static void
reset_queue(struct queue_t *q)
{
    q->tail = 0;
    q->head = 0;
    q->e[0].id = 0;
    q->e[0].next = 0;
}

/*
 * Allocate and initialize the queue.
 */
static struct queue_t *
alloc_queue(const struct grid_t *grid)
{
    struct queue_t *queue;
    queue = calloc(1, sizeof(struct queue_t));
    queue->e = calloc(grid->size, sizeof(struct queue_element_t));

    assert(queue->e);
    reset_queue(queue);
    return queue;
}

static void
free_queue(struct queue_t *q)
{
    free(q->e);
    free(q);
}

int pPathfind(struct game *g, int checkonly)
{
    int i = 0, x, y;
    struct queue_t *queue;
	struct grid_t *grid;

    grid = copy_grid(g);
    queue = alloc_queue(grid);

/*    for(i=0;i< (signed int)grid->size;i++)
    {
        if ( i % grid->w == 0 ) printf("\n");
        switch(grid->ar[i]) {
            case 0:     printf("%3d",grid->path[i]); break;
            case 1:     printf("###"); break;
            case 2:     printf("XXX"); break;
            case 254:   printf("SSS"); break;
            case 255:   printf("EEE"); break;

        }
    }
    printf("\n\n");*/


    for(i=0; i < (signed int)grid->size; i++) {
        if (grid->ar[i] == 255) {
            push(queue, grid, i, 1);
        }
    }

    while (grid->marked[HEAD(queue).id] != 0) {
        checkfield(queue, grid);
        queue->head++;
		if ( queue->head >= (signed int)grid->size ) break;
//		printf("Gonna check: %d (%d)\n", HEAD(queue).id, queue->head);
    }

/*    for(i=0;i< (signed int)grid->size;i++)
    {
        if ( i % grid->w == 0 ) printf("\n");
        switch(grid->ar[i]) {
            case 0:     printf("%3d",grid->path[i]); break;
            case 1:     printf("###"); break;
			case 2:     printf("XXX"); break;
            case 254:   printf("SSS"); break;
            case 255:   printf("EEE"); break;

        }
    }
    printf("\n\n");*/


	if ( checkonly )
	{
		int ret = 0;
//		puts("We need to check if any of the entrances are blocked.");
		for(i=0;i<g->startN;i++)
		{
			if ( grid->path[g->start[i][0]+(g->start[i][1]*G_WIDTH)] > 0 )
			{
				continue;
			}
//			printf("Start position no: %d, cell %d x %d, is value %d\n", i, g->start[i][0], g->start[i][1]*G_WIDTH, grid->path[g->start[i][0]+(g->start[i][1]*G_WIDTH)]);
			ret++;
		}
		free_queue(queue);
		free_grid(grid);
		return ret;
	}

	i = 0;
    for(y=0; y < G_HEIGHT; y++) {
        for(x=0; x < G_WIDTH; x++) {
            g->path[y][x] = grid->path[i];
            i++;
        }
    }

    free_queue(queue);
    free_grid(grid);
    return 0;
}
