#ifndef __TIMERSCFG_H__
#define __TIMERSCFG_H__
#include "applicfg.h"

#define TIMER_FREE 0
#define TIMER_ARMED 1
#define TIMER_TRIG 2
#define TIMER_TRIG_PERIOD 3

#define TIMER_NONE -1
#define TIMER_HANDLE INTEGER16

// Whatever your microcontroller, the timer wont work if
// TIMEVAL is not at least on 32 bits
#define TIMEVAL UNS32

// using 16 bits timer
#define TIMEVAL_MAX 0xFFFFFFFF

// The timer is incrementing every 10 us.
#define MS_TO_TIMEVAL(ms) ((ms) * 1000)
#define US_TO_TIMEVAL(us) ((us) / 1)

#endif
