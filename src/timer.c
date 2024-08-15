/*!
** @file   timer.c
** @author Edouard TISSERANT and Francis DUPIN
** @date   Tue Jun  5 09:32:32 2007
**
** @brief
**
**
*/

/* #define DEBUG_WAR_CONSOLE_ON */
/* #define DEBUG_ERR_CONSOLE_ON */

#include "timer.h"
#include "applicfg.h"
#include "config.h"

/*  ---------  The timer table --------- */
s_timer_entry timers[MAX_NB_TIMER] = {{TIMER_FREE, NULL, NULL, 0, 0, 0},};

TIMEVAL total_sleep_time = TIMEVAL_MAX;
TIMER_HANDLE last_timer_raw = -1;

#define min_val(a,b) ((a<b)?a:b)


//#define DEBUG
#ifdef DEBUG
#    define D_MSG_TIME(...)          \
          MSG_TIME(__VA_ARGS__)
#else
#    define D_MSG_TIME(...)
#endif

/*!
** -------  Use this to declare a new alarm ------
**
** @param d
** @param id
** @param callback
** @param value
** @param period
**
** @return
**/
TIMER_HANDLE SetAlarm(CO_Data* d, UNS32 id, TimerCallback_t callback, TIMEVAL value, TIMEVAL period)
{
	TIMER_HANDLE row_number;
	s_timer_entry *row;

	/* in order to decide new timer setting we have to run over all timer rows */
	for(row_number=0, row=timers; row_number <= last_timer_raw + 1 && row_number < MAX_NB_TIMER; row_number++, row++)
	{
		if (callback && 	/* if something to store */
			row->state == TIMER_FREE) /* and empty row */
		{	/* just store */
//			TIMEVAL real_timer_value;
			TIMEVAL elapsed_time;

			if (row_number == last_timer_raw + 1) last_timer_raw++;

			elapsed_time = getElapsedTime();
			/* set next wakeup alarm if new entry is sooner than others, or if it is alone */
//			real_timer_value = value;
//			real_timer_value = min_val(real_timer_value, TIMEVAL_MAX);

			if (total_sleep_time > elapsed_time && total_sleep_time - elapsed_time > value)
			{
				if(total_sleep_time != TIMEVAL_MAX)
					total_sleep_time = elapsed_time + value;
                                else
                                        total_sleep_time = value;
				setTimer(value);
			}
			row->callback = callback;
			row->d = d;
			row->id = id;
			row->val = elapsed_time + value;
			row->interval = period;
			row->state = TIMER_ARMED;
			D_MSG_TIME("Set timer %s, after %d us", name, value + elapsed_time);
			return row_number;
		}
	}

	return TIMER_NONE;
}

/*!
**  -----  Use this to remove an alarm ----
**
** @param handle
**
** @return
**/
TIMER_HANDLE DelAlarm(TIMER_HANDLE handle)
{
	/* Quick and dirty. system timer will continue to be trigged, but no action will be preformed. */
	//MSG_WAR(0x3320, "DelAlarm. handle = ", handle);
	if(handle != TIMER_NONE)
	{
		if(handle == last_timer_raw)
			last_timer_raw--;
		timers[handle].state = TIMER_FREE;
		D_MSG_TIME("Delete timer %s", timers[handle].name);
	}
	return TIMER_NONE;
}

/*!
** ------  TimeDispatch is called on each timer expiration ----
**
**/
int tdcount=0;
void TimeDispatch(void)
{
	TIMER_HANDLE i;
	TIMEVAL next_wakeup = TIMEVAL_MAX; /* used to compute when should normaly occur next wakeup */
	/* First run : change timer state depending on time */
	/* Get time since timer signal */
	UNS32 overrun = (UNS32)getElapsedTime();// Delete?

	s_timer_entry *row;

	D_MSG_TIME("List of timers:");
	for(i=0, row = timers; i <= last_timer_raw; i++, row++)
	{
		D_MSG_TIME("%d %s state-%d", i, row->name, row->state);
		if (row->state & TIMER_ARMED) /* if row is active */
		{
			/* Each armed timer value in decremented. */
			if(row->val <= total_sleep_time) {
				if (!row->interval) /* if simply outdated */
				{
					/* Check if this new timer value is the soonest */
					//D_MSG_TIME("TIMER_TRIG");
					row->state = TIMER_TRIG; /* ask for trig */
				}
				else /* or period have expired */
				{
					/* set val as interval, with 32 bit overrun correction, */
					/* modulo for 64 bit not available on all platforms     */
					row->state = TIMER_TRIG_PERIOD; /* ask for trig, periodic */
					/* Check if this new timer value is the soonest */
					row->val = row->interval;
					if (row->val < next_wakeup)
						next_wakeup = row->val;
				}
			}
			else {
				row->val -= total_sleep_time;
				if (row->val < next_wakeup)
					next_wakeup = row->val;
			}
		}
	}

	/* Remember how much time we should sleep. */
	total_sleep_time = next_wakeup;

	/* Set timer to soonest occurence */
	setTimer(next_wakeup);

	/* Then trig them or not. */
	for(i=0, row = timers; i<=last_timer_raw; i++, row++)
	{
		if (row->state & TIMER_TRIG)
		{
			D_MSG_TIME("TIMER_TRIG %s", row->name);
			row->state &= ~TIMER_TRIG; /* reset trig state (will be free if not periodic) */
			if(!row->interval)
				DelAlarm(i);
			if(row->callback)
				(*row->callback)(row->d, row->id); /* trig ! */
		}
	}
}
