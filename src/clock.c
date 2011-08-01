#include <errno.h>

#include <winsock2.h>

#include "clock.h"
#include "misc.h"

extern DWORD libpthread_time_increment;
extern __int64 libpthread_hpet_frequency;

int clock_getres(clockid_t clk_id, struct timespec *res)
{
    if (clk_id == CLOCK_REALTIME) {
        res->tv_sec = libpthread_time_increment / 10000000;
        res->tv_nsec = (libpthread_time_increment % 10000000) * 100;
        return 0;
    } else if (clk_id == CLOCK_MONOTONIC) {
        if (libpthread_hpet_frequency < 1000)
            return set_errno(EINVAL);

        res->tv_sec = 0;
        res->tv_nsec = (int) ((1000000000 + (libpthread_hpet_frequency >> 1)) / libpthread_hpet_frequency);
        if (res->tv_nsec < 1)
            res->tv_nsec = 1;

        return 0;
    }

    return set_errno(EINVAL);
}

/* sleep up to 49.7 days */
int nanosleep(const struct timespec *request, struct timespec *remain)
{
    __int64 i64;

    if (request->tv_sec < 0 || request->tv_sec > 4294967 || request->tv_nsec < 0 || request->tv_nsec > 999999999)
        return set_errno(EINVAL);

    i64 = request->tv_sec * 1000LL + request->tv_nsec / 1000000;
    if (i64 < 0 || i64 >= 0xFFFFFFFF)
        return set_errno(EINVAL);

    Sleep((unsigned) i64);

    return 0;
}

/* sleep up to 49.7 days */
int clock_nanosleep(clockid_t clock_id, int flags,
                           const struct timespec *request,
                           struct timespec *remain)
{
    if (clock_id != 0)
        return set_errno(EINVAL);

    if (flags == 0)
        return nanosleep(request, NULL);

    /* TIMER_ABSTIME = 1 */
    Sleep(arch_rel_time_in_ms(request));

    return 0;
}

#define DELTA_EPOCH_IN_NS     11644473600000000000LL
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    __int64 t;

    if (clk_id == CLOCK_REALTIME) {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        t = (((__int64) ft.dwHighDateTime) << 32 | ft.dwLowDateTime) - DELTA_EPOCH_IN_NS;
        tp->tv_sec = t / 1000000000LL;
        tp->tv_nsec = (int) (t % 1000000000LL);
        return 0;
    } else if (clk_id == CLOCK_MONOTONIC) {
        LARGE_INTEGER performanceCount;

        if (libpthread_hpet_frequency < 1000 || QueryPerformanceCounter(&performanceCount) == 0)
            return set_errno(EINVAL);

        tp->tv_sec = performanceCount.QuadPart / libpthread_hpet_frequency;
        tp->tv_nsec = (int) ((performanceCount.QuadPart % libpthread_hpet_frequency + (libpthread_hpet_frequency >> 1)) / libpthread_hpet_frequency);
        if (tp->tv_nsec >= 1000000000) {
            tp->tv_sec ++;
            tp->tv_nsec -= 1000000000;
        }

        return 0;
    }

    return set_errno(EINVAL);
}

/* Number of 100ns-seconds between the beginning of the Windows epoch
 * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
 */
#define DELTA_EPOCH_IN_100NS     116444736000000000LL
int clock_settime(clockid_t clk_id, const struct timespec *tp)
{
    unsigned __int64 t64;
    FILETIME ft;
    SYSTEMTIME st;

    if (clk_id != CLOCK_REALTIME)
        return set_errno(EINVAL);

    t64 = tp->tv_sec * 10000000LL + tp->tv_nsec / 100 + DELTA_EPOCH_IN_100NS;
    ft.dwLowDateTime = (DWORD) t64;
    ft.dwHighDateTime = (DWORD) (t64 >> 32);

    if (FileTimeToSystemTime(&ft, &st) == 0)
        return set_errno(EINVAL);

    if (SetSystemTime(&st) == 0)
        return set_errno(EPERM);

    return 0;
}
