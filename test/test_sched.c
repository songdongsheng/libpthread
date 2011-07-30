#include <assert.h>
#include <string.h>

#include "sched.h"

int main(int argc, char *argv[])
{
    int rc, priority = SCHED_OTHER;
    struct timespec tp;
    struct sched_param sp;
    pid_t pid = 1;

    memset(&sp, 0, sizeof(sp));

    rc = sched_yield();
    assert(rc == 0);

    rc = sched_getscheduler(pid);
    assert(rc == SCHED_OTHER);

    rc = sched_setscheduler(pid, priority, &sp);
    assert(rc == 0);

    rc = sched_get_priority_min(pid);
    assert(rc == THREAD_PRIORITY_IDLE);

    rc = sched_get_priority_max(pid);
    assert(rc == THREAD_PRIORITY_TIME_CRITICAL);

    rc = sched_setparam(pid, &sp);
    assert(rc == 0);

    rc = sched_getparam(pid, &sp);
    assert(rc == 0);
    assert(sp.sched_priority == THREAD_PRIORITY_NORMAL);

    rc = sched_rr_get_interval(pid, &tp);
    assert(rc == 0);
    assert(tp.tv_sec == 0);
    assert(tp.tv_nsec == 15625000);

    return 0;
}
