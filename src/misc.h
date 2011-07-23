#include <errno.h>

#include <winsock2.h>

static inline int set_errno(int result)
{
    if (result != 0) {
        errno = result;
        return -1;
    }
    return 0;
}

/* Number of micro-seconds between the beginning of the Windows epoch
 * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
 */
#define DELTA_EPOCH_IN_USEC     11644473600000000LL;
static __inline __int64 FileTimeToUnixTime(FILETIME *input)
{
    return (((__int64) input->dwHighDateTime) << 32 | input->dwLowDateTime) / 10 - DELTA_EPOCH_IN_USEC;
}

/* Return micro-seconds since the Unix epoch (jan. 1, 1970) UTC */
static inline __int64 arch_time_in_us(void)
{
    FILETIME time;

    GetSystemTimeAsFileTime(&time);
    return FileTimeToUnixTime(&time);
}

static inline __int64 arch_time_in_us_from_timespec(const struct timespec *ts)
{
    return ts->tv_sec * 1000000LL + ts->tv_nsec / 1000;
}

static inline unsigned arch_rel_time_in_ms(const struct timespec *ts)
{
    __int64 t1 = arch_time_in_us_from_timespec(ts);
    __int64 t2 = arch_time_in_us();

    if (t1 < t2) return 0;

    return (unsigned) (t1 - t2);
}
