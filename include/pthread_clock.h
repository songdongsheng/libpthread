#ifndef _PTHREAD_CLOCK_H_
#define _PTHREAD_CLOCK_H_   1

#include <pthread_types.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#endif /* _PTHREAD_CLOCK_H_ */
