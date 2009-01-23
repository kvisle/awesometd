#ifndef __TIMERS_H__
#define __TIMERS_H__

#define TIMER_REPORTFPS         1
#define TIMER_GAMECYCLE         2
#define TIMER_RENDER            3

void init_timers(void);
Uint32 timeduserevent(Uint32 interval, void *param);
Uint32 timedrenderevent(Uint32 interval, void *param);
void mark_rendered(void);

#endif
