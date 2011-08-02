#include <stdlib.h>
#include <pthread.h>

#include <winsock2.h>

#include "arch_thread.h"
#include "misc.h"

int pthread_attr_init(pthread_attr_t *attr)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *size)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t size)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_setdetachstate(pthread_attr_t *a, int flag)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_getdetachstate(const pthread_attr_t *a, int *flag)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_setinheritsched(pthread_attr_t *a, int flag)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_getinheritsched(const pthread_attr_t *a, int *flag)
{
    return set_errno(ENOTSUP);
}
