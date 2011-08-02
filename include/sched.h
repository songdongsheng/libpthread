#ifndef _SCHED_H_
#define _SCHED_H_       1

#ifndef _PID_T_
#include <sys/types.h> /* Adding pid_t definition.  */
#endif

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

#ifndef _PID_T_
    #define _PID_T_     1
    #ifdef _WIN64
        typedef __int64 pid_t;
    #else
        typedef int     pid_t;
    #endif
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

#ifndef SCHED_OTHER

#define SCHED_OTHER     0 /* NORMAL_PRIORITY_CLASS */
#define SCHED_FIFO      1
#define SCHED_RR        2
#define SCHED_MIN       SCHED_OTHER
#define SCHED_MAX       SCHED_RR

struct sched_param {
  int sched_priority;
};

int sched_yield(void);
int sched_rr_get_interval(pid_t pid, struct timespec * tp);
int sched_get_priority_min(int pol);
int sched_get_priority_max(int pol);
int sched_getscheduler(pid_t pid);
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
int sched_getparam(pid_t pid, struct sched_param *param);
int sched_setparam(pid_t pid, const struct sched_param *param);

#endif /* SCHED_OTHER */

#ifdef __cplusplus
}
#endif

#endif /* _SCHED_H_ */
