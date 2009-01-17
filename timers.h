#ifndef __TIMERS_H__
#define __TIMERS_H__

#define TIMER_REPORTFPS         1
#define TIMER_SPAWNMONSTER      2
#define TIMER_MOVEMONSTER       3
#define TIMER_ANIMATESPRITES    4
#define TIMER_SHOOTTOWERS       5

void init_timers(void);
Uint32 timeduserevent(Uint32 interval, void *param);

#endif
