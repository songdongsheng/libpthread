#include <stdlib.h>
#include <pthread.h>

#include <winsock2.h>

#include "arch_thread.h"
#include "misc.h"

/*
 * thread-specific data = TSD
 * thread local storage = TLS
 */
int pthread_key_create(pthread_key_t *key, void (* dest)(void *))
{
    return set_errno(ENOTSUP);
}

int pthread_key_delete(pthread_key_t key)
{
    return set_errno(ENOTSUP);
}

void *pthread_getspecific(pthread_key_t key)
{
    return NULL;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    return set_errno(ENOTSUP);
}
