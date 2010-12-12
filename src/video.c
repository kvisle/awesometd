#include "video.h"

#include <SDL_image.h>

// FIXME: I haven't managed to get this to work without an alignment of 4.
//        That's just stupid.
static const GLubyte charmap[256][32] = {
    ['\'']= { 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 
              0x00, 0, 0, 0, 0x08, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, },
    ['*'] = { 0x00, 0, 0, 0, 0x10, 0, 0, 0, 0x54, 0, 0, 0, 0x38, 0, 0, 0, 
              0xfe, 0, 0, 0, 0x38, 0, 0, 0, 0x54, 0, 0, 0, 0x10, 0, 0, 0, },
    [','] = { 0x30, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x00, 0, 0, 0, 
              0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, },
    ['-'] = { 0x00, 0, 0, 0, 0x80, 0, 0, 0, 0xc0, 0, 0, 0, 0x60, 0, 0, 0, 
              0x30, 0, 0, 0, 0x18, 0, 0, 0, 0x0c, 0, 0, 0, 0x06, 0, 0, 0, },
    ['.'] = { 0x00, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x00, 0, 0, 0, 
              0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, },
    ['0'] = { 0x00, 0, 0, 0, 0x38, 0, 0, 0, 0x64, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x4c, 0, 0, 0, 0x38, 0, 0, 0, },
    ['1'] = { 0x00, 0, 0, 0, 0xfc, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0, 
              0x30, 0, 0, 0, 0x30, 0, 0, 0, 0x70, 0, 0, 0, 0x30, 0, 0, 0, },
    ['2'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xe0, 0, 0, 0, 0x78, 0, 0, 0, 
              0x3c, 0, 0, 0, 0x06, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['3'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0x06, 0, 0, 0, 
              0x3c, 0, 0, 0, 0x18, 0, 0, 0, 0x0c, 0, 0, 0, 0x7e, 0, 0, 0, },
    ['4'] = { 0x00, 0, 0, 0, 0x0c, 0, 0, 0, 0x0c, 0, 0, 0, 0xfe, 0, 0, 0, 
              0xcc, 0, 0, 0, 0x6c, 0, 0, 0, 0x3c, 0, 0, 0, 0x1c, 0, 0, 0, },
    ['5'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0x06, 0, 0, 0, 
              0x06, 0, 0, 0, 0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['6'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0x60, 0, 0, 0, 0x3c, 0, 0, 0, },
    ['7'] = { 0x00, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0, 
              0x18, 0, 0, 0, 0x0c, 0, 0, 0, 0xc6, 0, 0, 0, 0xfe, 0, 0, 0, },
    ['8'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['9'] = { 0x00, 0, 0, 0, 0x78, 0, 0, 0, 0x0c, 0, 0, 0, 0x06, 0, 0, 0, 
              0x7e, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['a'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfe, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x6c, 0, 0, 0, 0x38, 0, 0, 0, },
    ['b'] = { 0x00, 0, 0, 0, 0xfc, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xfc, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['c'] = { 0x00, 0, 0, 0, 0x3c, 0, 0, 0, 0x66, 0, 0, 0, 0xc0, 0, 0, 0, 
              0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0x66, 0, 0, 0, 0x3c, 0, 0, 0, },
    ['d'] = { 0x00, 0, 0, 0, 0xf8, 0, 0, 0, 0xcc, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xcc, 0, 0, 0, 0xf8, 0, 0, 0, },
    ['e'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 
              0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xfe, 0, 0, 0, },
    ['f'] = { 0x00, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 
              0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xfe, 0, 0, 0, },
    ['g'] = { 0x00, 0, 0, 0, 0x3e, 0, 0, 0, 0x66, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xce, 0, 0, 0, 0xc0, 0, 0, 0, 0x60, 0, 0, 0, 0x3e, 0, 0, 0, },
    ['h'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xfe, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['i'] = { 0x00, 0, 0, 0, 0x7e, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 
              0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x7e, 0, 0, 0, },
    ['j'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0x06, 0, 0, 0, 0x06, 0, 0, 0, 0x06, 0, 0, 0, 0x1e, 0, 0, 0, },
    ['k'] = { 0x00, 0, 0, 0, 0xce, 0, 0, 0, 0xdc, 0, 0, 0, 0xf8, 0, 0, 0, 
              0xf0, 0, 0, 0, 0xd8, 0, 0, 0, 0xcc, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['l'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 
              0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, },
    ['m'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xd6, 0, 0, 0, 
              0xfe, 0, 0, 0, 0xfe, 0, 0, 0, 0xee, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['n'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xce, 0, 0, 0, 0xde, 0, 0, 0, 
              0xfe, 0, 0, 0, 0xf6, 0, 0, 0, 0xe6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['o'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['p'] = { 0x00, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xfc, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['q'] = { 0x00, 0, 0, 0, 0x7a, 0, 0, 0, 0xcc, 0, 0, 0, 0xde, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['r'] = { 0x00, 0, 0, 0, 0xce, 0, 0, 0, 0xdc, 0, 0, 0, 0xf8, 0, 0, 0, 
              0xce, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['s'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0x06, 0, 0, 0, 
              0x7c, 0, 0, 0, 0xc0, 0, 0, 0, 0xcc, 0, 0, 0, 0x78, 0, 0, 0, },
    ['t'] = { 0x00, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 
              0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x7e, 0, 0, 0, },
    ['u'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['v'] = { 0x00, 0, 0, 0, 0x10, 0, 0, 0, 0x38, 0, 0, 0, 0x7c, 0, 0, 0, 
              0xee, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['w'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xee, 0, 0, 0, 0xfe, 0, 0, 0, 
              0xfe, 0, 0, 0, 0xd6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['x'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xee, 0, 0, 0, 0x7c, 0, 0, 0, 
              0x38, 0, 0, 0, 0x7c, 0, 0, 0, 0xee, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['y'] = { 0x00, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 
              0x3c, 0, 0, 0, 0x66, 0, 0, 0, 0x66, 0, 0, 0, 0x66, 0, 0, 0, },
    ['z'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xe0, 0, 0, 0, 0x70, 0, 0, 0, 
              0x38, 0, 0, 0, 0x1c, 0, 0, 0, 0x0e, 0, 0, 0, 0xfe, 0, 0, 0, },
};

static void vDrawString(int x, int y, const char *string, float r, float g, float b, float a)
{
    glPushMatrix();
        glColor4f(r, g, b, a);
        glRasterPos2i(x, y+8);
        glPixelTransferi(GL_UNPACK_ALIGNMENT, 4);
		while (*string != '\0')
		{
            glBitmap(8, 8, 0, 0, 8, 0, (const GLubyte *) &charmap[(int) *string]);
			string++;
		}
	glPopMatrix();
}

static void vDrawColoredLine(float x, float y, float tx, float ty, float width, float r, float g, float b, float a)
{
	glPushMatrix();
    glDisable(GL_TEXTURE_2D);
	GLfloat vcoords[] = {
		x, y,
		tx, ty
	};
	glColor4f(r, g, b, a);
	glLineWidth(width);
	glVertexPointer(2, GL_FLOAT, 0, vcoords);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINE_STRIP,0,2);
	glPopMatrix();
}

static void vDrawColoredQuad(float x, float y, float w, float h, float rot, float r, float g, float b, float a)
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(x,y,0.0);
    GLfloat vcoords[] = {
        0.0, 0.0,
        w, 0.0,
        w, h,
        0.0, h
    };
    glColor4f(r,g,b,a);
    glRotated(rot,0.0,0.0,1.0);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
}

static void vDrawTexturedQuad(float x, float y, float w, float h, float rot, float r, float g, float b, float a, GLuint id, int sid, int size)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(x+(w/2),y+(h/2),0.0);
    GLfloat vcoords[] = {
        -(w/2), -(h/2),
        w/2, -(h/2),
        w/2, h/2,
        -(w/2), h/2
    };
    GLfloat tcoords[] = {
        0.0+(0.25*(sid % 4)), 0.0+(0.25*(sid / 4)),
        0.25+(0.25*(sid % 4)), 0.0+(0.25*(sid / 4)),
        0.25+(0.25*(sid % 4)), 0.25+(0.25*(sid / 4)),
        0.0+(0.25*(sid % 4)), 0.25+(0.25*(sid / 4))
    };
    glColor4f(r,g,b,a);
    glBindTexture(GL_TEXTURE_2D, id);
    glRotated(rot,0.0,0.0,1.0);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
}

static void vDrawGrid(const struct video *v, const struct game *cg)
{
    int w, h;
	char buf[8];
    float r, g, b;
    float px, py, pw, ph;
    for(w=0;w<G_WIDTH;w++)
    {
        for(h=0;h<G_HEIGHT;h++)
        {
            px = w*32;
			py = h*32;
			pw = 32;
			ph = 32;
            switch(cg->grid[h][w])
            {
                case 0: r = 0.0; g = 1.0; b = 0.0; break;
                case 1: r = 0.5; g = 0.5; b = 0.3; break;
				case 2: r = 1.0; g = 0.5; b = 0.0; break;
                default:r = 1.0; g = 1.0; b = 1.0; break;
            }
            vDrawTexturedQuad(px, py, pw, ph, 0, 1, 1, 1, 1, v->terrain.texid, cg->grid[h][w], S32X32);
//            vDrawColoredQuad(px, py, pw, ph, 0, r, g, b, 1.0);
// Uncomment below to print the pathfinding results on the map.
/*			if ( cg->grid[h][w] == 0 )
			{*/
//				sprintf((char *)&buf, "%4d",cg->path[h][w]);
//				vDrawString(px, py, buf, 0, 0, 0, 1);
//			}*/
        }
    }
}

static void vDrawEnemy(struct game *g, struct video *v)
{
    struct enemy *e = g->enemy;
    while(e)
    {
		float a;
		if ( e->timeleft <= 50 ) {
			a = 1.0 - ((float)e->timeleft * 0.04);
//	        vDrawColoredQuad(e->x, e->y, 32, 32, 0, 0.5, 0.5, 0.5, a);
            vDrawTexturedQuad(e->x, e->y, 32, 32, e->rot, 1, 1, 1, a, v->octopi.texid, e->frame, S32X32);
			vDrawColoredQuad(e->x+2, e->y+28, ((float)e->hp/(float)e->hp_max)*28.0, 2, 0, 0, 1, 0, a);
			if ( e->debuffs[GDB_TYPE_DOT].time_left )
				vDrawColoredQuad(e->x, e->y, 4, 4, 0, 0, 1, 0, 1);
            if ( e->debuffs[GDB_TYPE_SLOW].time_left )
                vDrawColoredQuad(e->x+4, e->y, 4, 4, 0, 0, 0, 1, 1);
		}
        e = e->next;
    }
}

static void vDrawTower(struct game *g)
{
    char buf[4];
	struct tower *t = g->tower;
	while(t)
	{
        sprintf((char *)&buf, "%2d", t->type);
		vDrawColoredQuad(t->x*32+4, t->y*32+4, 24, 24, 0, 0.5, 0.5, 0.9, 1);
//        vDrawString(t->x*32+4, t->y*32+4, buf, 0, 0, 0, 1);
		t = t->next;
	}
}

static void vDrawShot(struct game *g)
{
    struct shot *s = g->shot;
    while(s)
    {
		if ( s->type == GS_TYPE_DIRECT )
		{
			vDrawColoredLine(s->x, s->y, s->tx, s->ty, 2, 1, 0, 0, 0.5);
			vDrawColoredLine(s->x, s->y, s->tx, s->ty, 1, 1, 0, 0, 1);
		}
		else
		{
			if ( s->video == GS_VIDEO_LASER_RED )
			{
				vDrawColoredQuad(s->x-4, s->y-1, 8, 2, s->rot, 1, 0.25, 0.25, 1);
			}
			else
			{
		        vDrawColoredQuad(s->x-2, s->y-2, 4, 4, 0, 1, 1, 1, 1);
			}
		}
        s = s->next;
    }
}

static void vDrawSidebar(struct game *g)
{
	int i;
	char buf[16];
    vDrawColoredQuad(512, 0, 128, 480, 0, 0.75, 0.75, 0.75, 1.0);

	vDrawString(528, 16, "level x", 0, 0, 0, 1);
	vDrawString(527, 15, "level x", 1, 1, 1, 1);

	sprintf((char *)&buf, "%5d", g->lives);
    vDrawString(528, 32, "lives", 0, 0, 0, 1);
	vDrawString(527, 31, "lives", 1, 1, 1, 1);
	vDrawString(592, 32, buf, 0, 0, 0, 1);
    vDrawString(591, 31, buf, 1, 0, 0, 1);

	sprintf((char *)&buf, "%5d", g->money);
    vDrawString(528, 48, "money", 0, 0, 0, 1);
	vDrawString(527, 47, "money", 1, 1, 1, 1);
    vDrawString(592, 48, buf, 0, 0, 0, 1);
    vDrawString(591, 47, buf, 1, 1, 0, 1);

    sprintf((char *)&buf, "%05d", g->score);
    vDrawString(528, 64, "score", 0, 0, 0, 1);
    vDrawString(527, 63, "score", 1, 1, 1, 1);
    vDrawString(592, 64, buf, 0, 0, 0, 1);
    vDrawString(591, 63, buf, 0, 1, 0, 1);

    sprintf((char *)&buf, "%5d", g->waveN);
    vDrawString(528, 80, "wave", 0, 0, 0, 1);
    vDrawString(527, 79, "wave", 1, 1, 1, 1);
    vDrawString(592, 80, buf, 0, 0, 0, 1);
    vDrawString(591, 79, buf, 0, 1, 0, 1);

    if ( g->wave )
    {
        sprintf((char *)&buf, "%5d", g->wave->timeleft);
        vDrawString(528, 96, "next", 0, 0, 0, 1);
        vDrawString(527, 95, "next", 1, 1, 1, 1);
        vDrawString(592, 96, buf, 0, 0, 0, 1);
        vDrawString(591, 95, buf, 0, 1, 0, 1);
    }

    vDrawString(528, 116, "towers", 0, 0, 0, 1);
    vDrawString(527, 115, "towers", 1, 1, 1, 1);


	for (i=0; i < G_TOWERS; i++)
	{
        if ( i == g->btowerid )
        {
            vDrawColoredQuad(532, 132+(i*24), 96, 16, 0, 1, 0.75, 0, 1);
            vDrawString(536, 136+(i*24), g->towerT[i].name, 0, 0, 0, 1);
        }
        else
        {
            vDrawColoredQuad(532, 132+(i*24), 96, 16, 0, 0, 0, 0, 1);
    	    vDrawColoredQuad(528, 128+(i*24), 96, 16, 0, 1, 1, 0, 1);
    		vDrawString(532, 132+(i*24), g->towerT[i].name, 0, 0, 0, 1);
        }
	}
}

void vDraw(struct video *v, struct game *g)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
// Stuff
    vDrawGrid(v, g);
    glDisable(GL_TEXTURE_2D);
    vDrawEnemy(g, v);
	vDrawTower(g);
    vDrawShot(g);
// The sidebar should be considered a part of the HUD/OSD/whatnot, and is drawn
// on top of other stuff.
    vDrawSidebar(g);
    SDL_GL_SwapBuffers();
}

struct texture vLoadTexture(char *fn)
{
    struct texture t;
    SDL_Surface *img = IMG_Load(fn);
    if ( !img )
    {
        printf("IMG_Load(%s) failed: %s\n", fn, SDL_GetError());
    }
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &t.texid);
    glBindTexture(GL_TEXTURE_2D, t.texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

    t.w = img->w;
    t.h = img->h;
    SDL_FreeSurface(img);
    return t;
}

struct video vSetup(void)
{
    struct video v;

    v.s = SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(  0.0, 640.0,
            480.0,   0.0,
            -10.0,  10.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    v.terrain = vLoadTexture("share/gfx/theme_lame.png");
    v.octopi = vLoadTexture("share/gfx/octopi.png");
    return v;
}
