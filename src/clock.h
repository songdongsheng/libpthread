#ifndef _CLOCK_H_
#define _CLOCK_H_   1

#ifndef _TIME_T_DEFINED
#include <time.h> /* Adding time_t definition.  */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INT64_C
#ifdef  _MSC_VER
#define INT64_C(c)      c ## I64
#define UINT64_C(c)     c ## UI64
#else
#define INT64_C(c)      c ## LL
#define UINT64_C(c)     c ## ULL
#endif
#endif

#ifndef PRId64
#ifdef  _WIN32
#define PRId64          "I64d"
#define PRIu64          "I64u"
#define PRIx64          "I64x"
#define PRIX64          "I64X"
#else
#define PRId64          "lld"
#define PRIu64          "llu"
#define PRIx64          "llx"
#define PRIX64          "llX"
#endif
#endif

#ifndef __clockid_t_defined
typedef int clockid_t;
#define __clockid_t_defined
#endif  /* __clockid_t_defined */

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME                  0
#endif

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC                 1
#endif

#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
struct timespec {
    time_t  tv_sec;       /* Seconds */
    long    tv_nsec;      /* Nanoseconds */
};

struct itimerspec {
    struct timespec  it_interval; /* Timer period */
    struct timespec  it_value;    /* Timer expiration */
};
#endif  /* _TIMESPEC_DEFINED */

int nanosleep(const struct timespec *request, struct timespec *remain);

int clock_getres(clockid_t clk_id, struct timespec *res);

int clock_gettime(clockid_t clk_id, struct timespec *tp);

int clock_settime(clockid_t clk_id, const struct timespec *tp);

int clock_nanosleep(clockid_t clock_id, int flags,
                           const struct timespec *request,
                           struct timespec *remain);
#ifdef __cplusplus
}
#endif

#endif
