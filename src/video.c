/*
    Awesome Tower Defense
    Copyright (C) 2008-2010  Trygve Vea and contributors (read AUTHORS)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <dirent.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


#include "video.h"
#include "video-game.h"

static char *TextureDirectories[] = {
    "/usr/share/awesometd/textures",
    "../share/textures",
    ".",
    "NULL"
};

static String *Numbers;

void VideoDrawTexturedQuadC(float x, float y, float w, float h, float rot, Texture *tex, int sliceid,float r, float g, float b, float alpha)
{
    float x_start = (float)(sliceid % tex->columns) / (float)tex->columns;
    float x_end = x_start + 1.0/(float)tex->columns;
    float y_start = (float)(sliceid / tex->columns) / (float)tex->rows;
    float y_end = y_start + 1.0/(float)tex->rows;

    glPushMatrix();
    glTranslatef(x,y,0.0);
    GLfloat vcoords[] = {
        ceilf(0.0-(w/2.0)), ceilf(0.0-(h/2.0)),
        ceilf(w/2.0), ceilf(0.0-(h/2.0)),
        ceilf(w/2.0), ceilf(h/2.0),
        ceilf(0.0-(w/2.0)), ceilf(h/2.0)
    };
    GLfloat tcoords[] = {
        x_start, y_start,
        x_end, y_start,
        x_end, y_end,
        x_start, y_end
    };
    glColor4d(r,g,b,alpha);
    glRotated(rot,0.0,0.0,1.0);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex->texid);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void VideoDrawColoredQuad(float x, float y, float w, float h, float rot, float r, float g, float b, float a)
{
    glPushMatrix();
    glTranslatef(x,y,0.0);
    GLfloat vcoords[] = {
        ceilf(0.0-(w/2)), ceilf(0.0-(h/2.0)),
        ceilf(w/2.0), ceilf(0.0-(h/2.0)),
        ceilf(w/2.0), ceilf(h/2.0),
        ceilf(0.0-(w/2.0)), ceilf(h/2.0)
    };
    glColor4d(r,g,b,a);
    glRotated(rot,0.0,0.0,1.0);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
}

void VideoDrawCircle(float r)
{
    int i;
    GLfloat vcoords[360*2];
    for (i=0;i<360;i++) {
        vcoords[i*2] = cos(i*(M_PI/180));
        vcoords[i*2+1] = sin(i*(M_PI/180));
    }
    glScalef(r,r,0.0);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,360);
}

void VideoScanDirForTextures(char * dir)
{
    DIR *datadir;
    struct dirent *ent;
    if ((datadir = opendir(dir)) == NULL )
        return;
    while (( ent = readdir(datadir)) != NULL )
    {
        if ( g_pattern_match_simple("*.png",ent->d_name) )
        {
            gchar *key = g_strconcat(ent->d_name,NULL);
            Texture *value = g_malloc(sizeof(Texture));
            value->filename = g_strconcat(dir,"/",ent->d_name,NULL);
            value->texid = 0;
            g_hash_table_insert(TextureTable,key,value);
        }
    }
    closedir(datadir);
    return;
}

int VideoSetMode(int w, int h) {
    screen = SDL_SetVideoMode(w, h, 
                              VIDEOMODE_DEPTH, SDL_OPENGL|SDL_RESIZABLE);
    if (screen == NULL)
    {
        printf("Error when initializing video: %s\n", SDL_GetError());
        return -1;
    }
    SDL_WM_SetCaption("Awesome Tower Defense 0.4","");
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(  0.0, w,
                h, 0.0,
            -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    VideoGameSnapCamera();
    return 0;
}

void VideoFreeText(String *ptr)
{
    glDeleteTextures(1,&(ptr->tex.texid));
    g_free(ptr);
}

String * VideoLoadText(char *str, SDL_Color fg, int ft)
{
    TTF_Font *f;
    String *s = g_malloc(sizeof(String));
    s->tex.texid = 0;
    s->tex.columns = 1;
    s->tex.rows = 1;
    s->tex.frames = 1;
    if ( ft == 0 ) f = font;
    else f = monofont;
    SDL_Surface *i = TTF_RenderUTF8_Blended(f,str,fg);
    if ( i )
    {
        s->w = i->w;
        s->h = i->h;
        glGenTextures(1,&(s->tex.texid));
        glBindTexture(GL_TEXTURE_2D,s->tex.texid);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, i->w, i->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, i->pixels);
        SDL_FreeSurface(i);
        return s;
    }
    else
    {
        return s;
    }
}

Texture VideoLoadTexture(char *filename, int size)
{
    Texture tex;
    tex.filename = filename;
    printf("Loading texture: %s\n",filename);
    SDL_Surface *i = IMG_Load(filename);
    glEnable(GL_TEXTURE_2D);
    if ( i )
    {
        glGenTextures(1, &tex.texid);
        glBindTexture(GL_TEXTURE_2D, tex.texid);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, i->w, i->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, i->pixels);
        switch(size)
        {
            case TEXTURE_SIZE_48x64:
                tex.columns = i->w / 48;
                tex.rows = i->h / 64;
                break;
            case TEXTURE_SIZE_32x32:
                tex.columns = i->w / 32;
                tex.rows = i->h / 32;
                break;
            case TEXTURE_SIZE_64x64:
                tex.columns = i->w / 64; 
                tex.rows = i->h / 64; 
                break;
            case TEXTURE_SIZE_128x128:
                tex.columns = i->w / 128; 
                tex.rows = i->h / 128;
                break;
        }
        tex.frames = i->w/i->h;
        SDL_FreeSurface(i);
        return tex;
    }
    else
    {
        printf("IMG_Load: %s\n",IMG_GetError());
        return tex;
    }
}

int VideoInit(void)
{
    int i;
    char *h = getenv("HOME");
    gchar *hgd = g_strconcat(h,"/.awesometd/textures",NULL);
    TextureTable = g_hash_table_new(g_str_hash,g_str_equal);
    for (i=0;!g_pattern_match_simple("NULL",TextureDirectories[i]);i++)
        VideoScanDirForTextures(TextureDirectories[i]);
    VideoScanDirForTextures(hgd);
    g_free(hgd);

    TTF_Init();
    font = TTF_OpenFont("../share/fonts/font.ttf",16);
    monofont = TTF_OpenFont("../share/fonts/mono.ttf",24);
    TTF_SetFontStyle(font,TTF_STYLE_BOLD);
    
    int x = VideoSetMode(VIDEOMODE_WIDTH,VIDEOMODE_HEIGHT);
    SDL_Color tc = { 255, 255, 255 };
    Numbers = VideoLoadText("0123456789",tc,1);

    VideoGameInitIcons();

    return x;
}

void VideoDrawNumber(int x, int y, int val)
{
    int i;
    char string[9];
    sprintf(string,"%8d",val);
    GLfloat vcoords[] = {
        0.0, 0.0,
        14.4, 0.0,
        14.4, 25.0,
        0.0, 25.0
    };
    glPushMatrix();
    glTranslatef(x,y,0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,Numbers->tex.texid);
    glColor4d(1.0,1.0,1.0,1.0);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    for (i=0;i<8;i++)
    {
        glTranslatef(-15.0,0.0,0.0);
        int num = string[7-i]-48;
        if ( num > 9 || num < 0 ) break;
        GLfloat tcoords[] = {
            num*0.1, 0.0,
            0.1+(num*0.1), 0.0,
            0.1+(num*0.1), 1.0,
            num*0.1, 1.0
        };
        glTexCoordPointer(2,GL_FLOAT,0,tcoords);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void VideoDraw(void) {
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    VideoGameDraw();
    SDL_GL_SwapBuffers();
}
