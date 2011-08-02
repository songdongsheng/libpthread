#include <stdlib.h>
#include <pthread.h>

#include <winsock2.h>

#include "arch_thread.h"
#include "misc.h"

int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a)
{
    return set_errno(ENOTSUP);
}

int pthread_mutex_destroy(pthread_mutex_t *m)
{
    return set_errno(ENOTSUP);
}

int pthread_mutex_lock(pthread_mutex_t *m)
{
    return set_errno(ENOTSUP);
}

int pthread_mutex_trylock(pthread_mutex_t *m)
{
    return set_errno(ENOTSUP);
}

int pthread_mutex_timedlock(pthread_mutex_t *m, const struct timespec *ts)
{
    return set_errno(ENOTSUP);
}

int pthread_mutex_unlock(pthread_mutex_t *m)
{
    return set_errno(ENOTSUP);
}
