#ifndef __GFX_BUTTONS_H__
#define __GFX_BUTTONS_H__

void init_buttons(void);
void press_button(int x, int y);
void draw_hint_text(SDL_Surface *s);
void draw_buttons(SDL_Surface *s);

#endif /* __GFX_BUTTONS_H__ */
