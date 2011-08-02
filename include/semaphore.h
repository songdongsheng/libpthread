#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_   1

#include <pthread_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void    *sem_t;

#ifndef PTHREAD_PROCESS_SHARED
    #define PTHREAD_PROCESS_SHARED      0
    #define PTHREAD_PROCESS_PRIVATE     1
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
