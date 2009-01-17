#ifndef __CURSOR_H__
#define __CURSOR_H__

void update_cursor(int x, int y);
void draw_cursor(SDL_Surface *s);
void get_cursor_location(int *x, int *y);

#endif /*__CURSOR_H__*/
