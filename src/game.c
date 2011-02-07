#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "game.h"
#include "parser.h"

int gPathNext(struct game *g, int x, int y)
{
    if ( x > 0 &&
         y > 0 &&
         g->grid[y][x-1] == 0 &&
         g->grid[y-1][x] == 0 &&
         g->grid[y-1][x-1] == 0 &&
         g->path[y-1][x-1] < g->path[y][x]-1
       )
    {
        return G_PATH_NEXT_NW;
    }
    else if ( x < G_WIDTH &&
              y > 0 &&
              g->grid[y][x+1] == 0 &&
              g->grid[y-1][x] == 0 &&
              g->grid[y-1][x+1] == 0 &&
              g->path[y-1][x+1] < g->path[y][x]-1
            )
    {
        return G_PATH_NEXT_NE;
    }
    else if ( x < G_WIDTH &&
              y < G_HEIGHT &&
              g->grid[y][x+1] == 0 &&
              g->grid[y+1][x] == 0 &&
              g->grid[y+1][x+1] == 0 &&
              g->path[y+1][x+1] < g->path[y][x]-1
            )
    {
        return G_PATH_NEXT_SE;
    }
    else if ( x > 0 &&
              y < G_HEIGHT &&
              g->grid[y][x-1] == 0 &&
              g->grid[y+1][x] == 0 &&
              g->grid[y+1][x-1] == 0 &&
              g->path[y+1][x-1] < g->path[y][x]-1
            )
    {
        return G_PATH_NEXT_SW;
    }
    else if ( x > 0 &&
              g->path[y][x-1] < g->path[y][x] &&
              g->path[y][x-1] > 0 )
    {
        return G_PATH_NEXT_W;
    }
    else if ( x < G_WIDTH &&
              g->path[y][x+1] < g->path[y][x] &&
              g->path[y][x+1] > 0 )
    {
        return G_PATH_NEXT_E;
    }
    else if ( y > 0 &&
              g->path[y-1][x] < g->path[y][x] &&
              g->path[y-1][x] > 0 )
    {
        return G_PATH_NEXT_N;
    }
    else if ( y < G_HEIGHT &&
              g->path[y+1][x] < g->path[y][x] &&
              g->path[y+1][x] > 0 )
    {
        return G_PATH_NEXT_S;
    }

    return G_PATH_NEXT_U;
}

static void sNew(struct game *g, struct tower *t, struct enemy *e)
{
    struct shot *p;
    struct shot *s = calloc(1, sizeof(struct shot));
    s->x = t->x*32+16;
    s->y = t->y*32+16;
    s->dx = e->x+16 - s->x;
    s->dy = e->y+16 - s->y;
    s->damage = t->shot_template.damage;
    s->speed = t->shot_template.speed;
    s->type = t->shot_template.type;
    s->debuff = t->shot_template.debuff;
    s->video = t->shot_template.video;
    s->rot = ((float)(atan2((int)e->y-((int)t->y*32),(int)e->x-((int)t->x*32))*180)/M_PI);

//    g->st.laser++;

    if ( s->type == GS_TYPE_DIRECT )
    {
        s->x = e->x+16;
        s->y = e->y+16;
        s->tx = t->x*32+16;
        s->ty = t->y*32+16;
    }

    if (!g->shot)
    {
        g->shot = s;
        return;
    }
    p = g->shot;
    while (p->next)
    {
        p = p->next;
    }
    p->next = s;
}

static void sMove(struct game *g, struct shot *s)
{
    struct enemy *e = g->enemy;

    float dx = s->dx;
    float dy = s->dy;
    float d = 1.0f/sqrt(dx*dx + dy*dy);
    float x = dx*d;
    float y = dy*d;

    if ( s->type != GS_TYPE_DIRECT )
    {
        s->x += x * (float)s->speed;
        s->y += y * (float)s->speed;
    }

    while(e)
    {
        if ( e->timeleft == 0 &&
             e->x <= s->x &&
             e->x+32 >= s->x &&
             e->y <= s->y &&
             e->y+32 >= s->y &&
             e->hp > 0 )
        {
            e->hp -= s->damage;
            if ( s->debuff.type )
            {
                if ( e->debuffs[s->debuff.type].time_left == 0 )
                {
                    e->debuffs[s->debuff.type] = s->debuff;
                }
            }
            if ( e->hp < 1 )
            {
                g->money += e->price;
            }
            s->x = -1024;
            s->y = -1024;
            return;
        }
        e = e->next;
    }
    if ( s->type == GS_TYPE_DIRECT )
    {
        s->x = -1024;
        s->y = -1024;
    }
}

static int teDistance(struct tower *t, struct enemy *e)
{
    int k1 = (t->x*32) - e->x;
    int k2 = (t->y*32) - e->y;

    int h = sqrt((k1*k1)+(k2*k2));

    return h;
}

static struct enemy * tFindWeakest(struct game *g, struct tower *t)
{
    struct enemy *best = NULL;
    struct enemy *p = g->enemy;
    while(p)
    {
        if ( best )
        {
            if ( best->hp > p->hp && t->range >= teDistance(t,p) && p->hp > 0 && p->timeleft == 0 )
            {
                best = p;
            }
        }
        else if ( t->range >= teDistance(t,p) && p->hp > 0 && p->timeleft == 0 )
            best = p;
        p = p->next;
    }
    return best;
}

static void tMove(struct game *g, struct tower *t)
{
    t->progress += t->speed;
    struct enemy *e = tFindWeakest(g, t);
    if ( e )
    {
        float angle = (float)(atan2((int)e->y-((int)t->y*32),(int)e->x-((int)t->x*32))*180)/M_PI;;
        t->rot = angle + 90;
        if ( t->progress >= 100 )
        {
            sNew(g, t, e);
            t->progress = 0;
        }
    }
}

static void tNew(struct game *g, struct tower *template, int cx, int cy)
{
    printf("tNew!\n");
    struct tower *p;
    struct tower *t = calloc(1, sizeof(struct tower));
    memcpy(t, template, sizeof(struct tower));
    t->x = cx;
    t->y = cy;
    t->progress = 0;
    t->rot = rand() % 360;
    t->next = NULL;
    if ( g->tower )
    {
        p = g->tower;
        while ( p->next )
        {
            p = p->next;
        }
        p->next = t;
    }
    else
        g->tower = t;
}

void gClickCell(struct game *g, int cx, int cy)
{
    int old;
    struct enemy *e = g->enemy;
    while(e)
    {
        if ( e->x/32 == cx || (e->x+31)/32 == cx )
        {
            if ( e->y/32 == cy || (e->y+31)/32 == cy )
            {
                printf("Cell contains at least one enemy. Returning.\n");
                // TODO - open up information about this enemy (?)
                return;
            }
        }
        e = e->next;
    }

    if (!g->towerS)
    {
        printf("No tower-template selected.\n");
        return;
    }

    if ( g->grid[cy][cx] )
    {
        printf("Inhabitable landscape. Returning\n");
        return;
    }

    if ( g->towerS->price > g->money )
    {
        printf("Not enough money, returning.\n");
        return;
    }

    old = g->grid[cy][cx];
    g->grid[cy][cx] = 2;
    if ( pPathfind(g, 1) > 0 )
    {
        g->grid[cy][cx] = old;
        return;
    }
    
    g->money -= g->towerS->price;
    tNew(g, g->towerS, cx, cy);
    g->needpath++;
}

static void sFreeUsed(struct game *g)
{
    struct shot *p, *o;
    if ( !g->shot ) return;

    p = g->shot;
    while ( p )
    {
        while ( p->next && ( p->next->x < 0 || p->next->x > 512 || p->next->y < 0 || p->next->y > 480 ) )
        {
            o = p->next;
            p->next = o->next;
            free(o);
        }
        p = p->next;
    }
    if ( g->shot && ( g->shot->x < 0 || g->shot->x > 512 || g->shot->y < 0 || g->shot->y > 480 ) )
    {
        o = g->shot;
        g->shot = g->shot->next;
        free(o);
    }
}

static void eFreeDead(struct game *g)
{
    struct enemy *p, *o;
    if ( !g->enemy ) return;
    while ( g->enemy && g->enemy->hp < 1 )
    {
        p = g->enemy->next;
        free(g->enemy);
        g->enemy = p;
        printf("Freed the top enemy.\n");
    }
    p = g->enemy;
    while( p && p->next )
    {
        if ( p->next->hp < 1 )
        {
            o = p->next;
            p->next = o->next;
            free(o);
            printf("Freed subenemy.\n");
        }
        p = p->next;
    }
}

static void edMove(struct game *g, struct enemy *e)
{
    int i;
    for (i=0;i<G_DEBUFF_MAX;i++)
    {
        if ( e->debuffs[i].time_left )
        {
            e->debuffs[i].counter++;
            switch(e->debuffs[i].type)
            {
                case GDB_TYPE_DOT:
                    if ( e->debuffs[i].counter == e->debuffs[i].interval )
                    {
                        e->hp -= e->debuffs[i].damage;
                        e->debuffs[i].counter = 0;
                        if ( e->hp <= 0 )
                        {
                            g->money += e->price;
                            return;
                        }
                    }
                    break;
                case GDB_TYPE_SLOW:
                    e->progress -= (float)e->speed * e->debuffs[i].speed_mod;
                    break;
            }
            e->debuffs[i].time_left--;
        }
    }
}

// Move enemy.
static void eMove(struct game *g, struct enemy *e)
{
    int i;
    if ( e->hp <= 0 ) return;
    e->progress += e->speed;

    edMove(g,e);

    while ( e->progress >= 100 )
    {
        e->progress -= 100;
        e->frame++;
        while ( e->frame >= 4 )
            e->frame -= 4;

        for (i=0;i<2;i++)
        {
            if ( e->rot_goal != e->rot )
            {
                if ( e->rot_goal - e->rot > 0 &&
                     e->rot_goal - e->rot <= 180 )
                    e->rot += 5;
                else
                    e->rot -= 5;
            }
            if ( e->rot < 0 )
                e->rot += 360;
    
            if ( e->rot >= 360 )
                e->rot -= 360;
        }

        if ( e->moveleft == 0 )
        {
            e->moveX = 0;
            e->moveY = 0;
            int x = e->x / 32;
            int y = e->y / 32;
            e->moveleft = 32;

            switch(gPathNext(g,x,y))
            {
                case G_PATH_NEXT_NW:
                    e->moveX = -1;
                    e->moveY = -1;
                    e->rot_goal = 315;
                    break;
                case G_PATH_NEXT_NE:
                    e->moveX = 1;
                    e->moveY = -1;
                    e->rot_goal = 45;
                    break;
                case G_PATH_NEXT_SE:
                    e->moveX = 1;
                    e->moveY = 1;
                    e->rot_goal = 135;
                    break;
                case G_PATH_NEXT_SW:
                    e->moveX = -1;
                    e->moveY = 1;
                    e->rot_goal = 225;
                    break;
                case G_PATH_NEXT_W:
                    e->moveX = -1;
                    e->rot_goal = 270;
                    break;
                case G_PATH_NEXT_E:
                    e->moveX = 1;
                    e->rot_goal = 90;
                    break;
                case G_PATH_NEXT_N:
                    e->moveY = -1;
                    e->rot_goal = 0;
                    break;
                case G_PATH_NEXT_S:
                    e->moveY = 1;
                    e->rot_goal = 180;
                    break;
                default:
                    e->moveleft = 0;
                    e->hp = 0;
                    g->lives--;
                    e->progress = 0;
                    break;
            }
        }

        if ( e->moveleft )
        {
            e->x += e->moveX;
            e->y += e->moveY;
            e->moveleft--;
        }
    }
}

// Allocate count template enemies and push them to g's list.
static void eNew(struct game *g, struct enemy template, int count)
{
    struct enemy *e;
    struct enemy *p;

    template.x = g->start[0][0]*32;
    template.y = g->start[0][1]*32;

    switch(gPathNext(g,template.x/32,template.y/32))
    {
        case G_PATH_NEXT_NW:
            template.rot = template.rot_goal = 315;
            break;
        case G_PATH_NEXT_NE:
            template.rot = template.rot_goal = 45;
            break;
        case G_PATH_NEXT_SE:
            template.rot = template.rot_goal = 135;
            break;
        case G_PATH_NEXT_SW:
            template.rot = template.rot_goal = 225;
            break;
        case G_PATH_NEXT_W:
            template.rot = template.rot_goal = 270;
            break;
        case G_PATH_NEXT_E:
            template.rot = template.rot_goal = 90;
            break;
        case G_PATH_NEXT_N:
            template.rot = template.rot_goal = 0;
            break;
        case G_PATH_NEXT_S:
            template.rot = template.rot_goal = 180;
            break;
    }


    if ( !g->enemy )
    {
        e = malloc(sizeof(struct enemy));
        memcpy(e, &template, sizeof(struct enemy));
        e->moveX = e->moveY = 0;
        e->progress = 0;
        g->enemy = e;
        e->next = NULL;
        count--;
    }

    p = g->enemy;

    while(count)
    {
        e = malloc(sizeof(struct enemy));
        memcpy(e, &template, sizeof(struct enemy));
        e->next = NULL;

        while (p->next) p = p->next;

        p->next = e;

        count--;
    }
}

// Allocate new wave w/template and return.
// TODO: Values are currently hardcoded.
void wNew(struct game *g, struct wave *template)
{
    struct wave *p;
    puts("wNew()");
    struct wave *w = calloc(1,sizeof(struct wave));

    w->timeleft = template->timeleft;
    w->enemy_template.hp = w->enemy_template.hp_max = template->enemy_template.hp;
    w->enemy_template.speed = template->enemy_template.speed;
    w->enemy_template.timeleft = template->enemy_template.timeleft;
    w->enemy_template.price = template->enemy_template.price;
    w->enemies = template->enemies;

    if ( !g->wave )
    {
        g->wave = w;
        return;
    }

    p = g->wave;

    while (p->next)
        p = p->next;

    p->next = w;
    return;
}

// Spawn wave. -> Create enemies and free the wave.
static void wSpawn(struct game *g)
{
    struct wave *w = g->wave;
    g->wave = w->next;

    eNew(g, w->enemy_template, w->enemies);
    puts("Spawned wave");

    free(w);
}

static void gFindHotspots(struct game *g)
{
    int x,y;
    g->startN = 0;
    for(x=0;x<G_WIDTH;x++)
    {
        for(y=0;y<G_HEIGHT;y++)
        {
            if (g->grid[y][x] == 254)
            {
                g->start[g->startN][0] = x;
                g->start[g->startN][1] = y;
                g->startN++;
            }
            else if (g->grid[y][x] == 255)
            {
                g->exit[g->exitN][0] = x;
                g->exit[g->exitN][1] = y;
                g->exitN++;
            }
        }
    }
}

static void ttNew(struct game *g, struct tower tn)
{
    struct tower *t = malloc(sizeof(struct tower));
    struct tower *p;

    memcpy(t,&tn,sizeof(struct tower));

    if ( g->towerT == NULL )
    {
        g->towerT = t;
        return;
    }

    p = g->towerT;
    while ( p->next )
        p = p->next;

    p->next = t;
    
}

static void levelParser(struct game *data, char *key, char *value)
{
    PUSH_INT(lives);
    PUSH_INT(money);
    PUSH_INT_ARRAY(map,G_WIDTH*G_HEIGHT);

    if ( strcmp(key, "tower") == 0 )
    {
        char *wp, *sn;
        struct tower t;

        wp = sn = value;

        t.id = strtol(wp,&wp,10);

        if ( *wp != '\0' ) wp++;
        if ( *wp != '"' )
        {
            printf("Invalid formatting on tower.\n");
            return;
        }
        wp++;
        sn = wp;
        while ( *wp != '"' && *wp != '\0' )
            wp++;

        if ( *wp == '"' )
        {
            *wp = '\0';
            wp += 2;
            t.name = strdup(sn);
        }
        else
        {
            printf("Invalid formatting on tower.\n");
            return;
        }

        t.price = strtol(wp,&wp,10); wp++;
        t.speed = strtol(wp,&wp,10); wp++;
        t.range = strtol(wp,&wp,10); wp++;

        if ( strncmp(wp, "IMP", 3) == 0 )
            t.shot_template.type = GS_TYPE_IMPACT;
        else if ( strncmp(wp, "DIR", 3) == 0 )
            t.shot_template.type = GS_TYPE_DIRECT;
        else if ( strncmp(wp, "SPL", 3) == 0 )
            t.shot_template.type = GS_TYPE_SPLASH;
        else
        {
            free(t.name);
            printf("Invalid formatting on tower.\n");
            return;
        }
        wp += 4;

        t.shot_template.damage = strtol(wp,&wp,10); wp++;
        t.shot_template.speed = strtol(wp,&wp,10); wp++;

        if ( strncmp(wp, "NOT", 3) == 0 )
        {
            t.shot_template.debuff.type = 0;
        }
        else if ( strncmp(wp, "DOT", 3) == 0 )
        {
            wp += 4;
            t.shot_template.debuff.type = GDB_TYPE_DOT;
            t.shot_template.debuff.damage = strtol(wp,&wp,10); wp++;
            t.shot_template.debuff.interval = strtol(wp,&wp,10); wp++;
            t.shot_template.debuff.time_left = strtol(wp,&wp,10); wp++;
            t.shot_template.debuff.counter = strtol(wp,&wp,10); wp++;
        }
        else if ( strncmp(wp, "SLW", 3) == 0 )
        {
            wp += 4;
            t.shot_template.debuff.type = GDB_TYPE_SLOW;
            t.shot_template.debuff.speed_mod = strtod(wp, &wp);
            wp++;
            t.shot_template.debuff.time_left = strtol(wp,&wp,10);
            wp++;
        }
        else
        {
            free(t.name);
            printf("Invalid formatting on tower.\n");
            return;
        }
        
        ttNew(data, t);
    }
    if ( strcmp(key, "wave") == 0 )
    {
        struct wave w;
        memset(&w, '\0', sizeof(struct wave));

        char *wp = value;

        w.timeleft = strtol(wp,&wp,10); wp++;
        w.enemies = strtol(wp,&wp,10); wp++;
        w.enemy_template.hp = w.enemy_template.hp_max = strtol(wp,&wp,10); wp++;
        w.enemy_template.speed = strtol(wp,&wp,10); wp++;
        w.enemy_template.price = strtol(wp,&wp,10); wp++;
        w.enemy_template.timeleft = strtol(wp,&wp,10); wp++;
        wNew(data, &w);
    }

    printf("key: %s, value: %s\n", key, value);
}

// New game.
struct game gNew(char *level)
{
    int i;
    printf("gNew(%s)\n", level);

    struct game g;
    memset(&g, '\0', sizeof(struct game));

    parse(level, levelParser, &g);

    for(i=0;i<15*16;i++)
        g.grid[i/16][i%16] = g.map[i];

    g.needpath = 1;

    gFindHotspots(&g);
    return g;
}

// Move game forward.
void gDo(struct game *g)
{
    struct enemy *e = g->enemy;
    struct tower *t = g->tower;
    struct shot *s = g->shot;

    if ( g->lives == 0 )
    {
        return;
    }

    g->time++;

    if ( g->needpath )
    {
        g->needpath = pPathfind(g, 0);
    }

// Iterate enemylist and move them around.
    while (e)
    {
        if ( e->timeleft == 0 )
        {
            eMove(g, e);
            e = e->next;
        }
        else
        {
            e->timeleft--;
            e = NULL;
        }
    }
    while (s)
    {
        sMove(g, s);
        s = s->next;
    }
    sFreeUsed(g);
    eFreeDead(g);
    while (t)
    {
        tMove(g, t);
        t = t->next;
    }


    if ( !g->wave )
    {
        if ( !g->enemy )
        {
//            puts("No more waves or enemies left... The game is definitly over.");
        }
        return;
    }
    
    g->wave->timeleft--;

    if ( !g->wave->timeleft )
    {
        wSpawn(g);
    }
}
