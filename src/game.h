#ifndef __GAME_H__
#define __GAME_H__

#define VM_GREEN		1
#define VM_BLUE			2

#define G_WIDTH      	16
#define G_HEIGHT     	15
#define G_SIZE       	G_WIDTH*G_HEIGHT
#define G_START_MAX  	8
#define G_EXITS_MAX  	8

#define G_PATH_NEXT_N   1
#define G_PATH_NEXT_E   2
#define G_PATH_NEXT_S   3
#define G_PATH_NEXT_W   4
#define G_PATH_NEXT_NE  5
#define G_PATH_NEXT_NW  6
#define G_PATH_NEXT_SE  7
#define G_PATH_NEXT_SW  8
#define G_PATH_NEXT_U   -1

#define G_TOWERS     	5
#define G_DEBUFF_MAX	3
#define G_WAVES			4

#define GS_TYPE_IMPACT	1
#define GS_TYPE_POISON	2
#define GS_TYPE_SPLASH	3
#define GS_TYPE_DIRECT	4
#define GS_TYPE_HOMING	5

#define GDB_TYPE_DOT	1
#define GDB_TYPE_SLOW	2


#define GS_VIDEO_LASER_RED	1

struct debuff {
	int type;
	int damage;
	int interval;
	float speed_mod;
	int time_left;
	int counter;   // Just a counter that is 
	int video_mod; // if we should apply a video effect on the enemy affected by this
};

struct shot {
    float x, y;
    float dx, dy;
	float tx, ty;
    int type;
    int damage;
    float speed;
	struct debuff debuff;
	int video;
	int rot;
    struct shot *next;
};

struct tower {
    int x, y;
    char *name;
    int type;
    int price;
    int speed;
    int progress;
	int range;
    int damage;
    int rot;
	struct shot shot_template;
    struct tower *next;
};

struct enemy {
    int x, y;
    int hp, hp_max;
    int speed;
    int timeleft;
    int moveX, moveY;
    int moveleft;
    int progress;
    int price;
    int frame;
    int rot;
    int rot_goal;
	struct debuff debuffs[G_DEBUFF_MAX];
    struct enemy *next;
};

struct wave {
    int timeleft;
    struct enemy enemy_template;
    int enemies; // The amount of enemies to spawn.
    struct wave *next;
};

//struct soundtable {
//    int laser;
//    int laser_last;
//};

struct game {
    struct tower *towerT;
    struct tower *towerS;
    int grid[G_HEIGHT][G_WIDTH];
    int path[G_HEIGHT][G_WIDTH];
    int time, waveN, lives, money, score;
    int startN, exitN, needpath;
    int start[G_START_MAX][2], exit[G_EXITS_MAX][2];
    struct tower *tower;
    struct enemy *enemy;
    struct wave *wave;
    struct shot *shot;
//    struct soundtable st;
};

void gDo(struct game *g);
struct game gNew(void);
void gClickCell(struct game *g, int cx, int cy);

// pathfind.c
int pPathfind(struct game *g, int checkonly);


#endif /* __GAME_H__ */
