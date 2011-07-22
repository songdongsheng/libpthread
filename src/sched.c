#include <winsock2.h>

#include "sched.h"

/*
 *  Windows Scheduling Priorities
 *  http://msdn.microsoft.com/en-us/library/ms685100
 */
int sched_yield(void)
{
    SwitchToThread(); /* Sleep(0); */

    return 0;
}

int sched_getscheduler(pid_t pid)
{
    return SCHED_OTHER;
}

int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)
{
    return 0;
}

int sched_get_priority_min(int policy)
{
    return THREAD_PRIORITY_IDLE;
}

int sched_get_priority_max(int policy)
{
    return THREAD_PRIORITY_TIME_CRITICAL;
}

int sched_setparam(pid_t pid, const struct sched_param *param)
{
    return 0;
}

int sched_getparam(pid_t pid, struct sched_param *param)
{
    param->sched_priority = THREAD_PRIORITY_NORMAL;
    return 0;
}

int sched_rr_get_interval(pid_t pid, struct timespec * tp)
{
    tp->tv_sec = 0;
    tp->tv_nsec = 15625000;
    return 0;
}
