#ifndef _SCHED_H_
#define _SCHED_H_       1

#include <pthread_types.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* _SCHED_H_ */
