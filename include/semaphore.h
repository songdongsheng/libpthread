#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_   1

#ifndef _MODE_T_
#include <sys/types.h> /* Adding mod_t definition.  */
#endif

#ifndef _TIME_T_DEFINED
#include <time.h> /* Adding time_t definition.  */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MODE_T_
#define _MODE_T_
typedef unsigned short _mode_t;

#ifndef NO_OLDNAMES
typedef _mode_t mode_t;
#endif
#endif  /* Not _MODE_T_ */

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

typedef void    *sem_t;

#ifndef PTHREAD_PROCESS_SHARED
    #define PTHREAD_PROCESS_SHARED      0
    #define PTHREAD_PROCESS_PRIVATE     1
#endif

#ifndef ETIMEDOUT
#define ETIMEDOUT       138
#endif

#define SEM_VALUE_MAX   INT_MAX
#define SEM_FAILED      NULL

int sem_init(sem_t * sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
int sem_post(sem_t *sem);

sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
int sem_close(sem_t *sem);
int sem_unlink(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* _SEMAPHORE_H_ */
