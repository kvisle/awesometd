#ifndef __GFX_CHARMAP_H__
#define __GFX_CHARMAP_H__

#define CHAR_COUNT 37
#define CHAR_SIZE_X 8
#define CHAR_SIZE_Y 8

void draw_text(SDL_Surface *s, const char *text, int x, int y);
void draw_text_color(SDL_Surface *s, const char *text, int x, int y, int r, int g, int b);


#endif /* __GFX_CHARMAP_H__ */
