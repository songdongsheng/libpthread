#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include <winsock2.h>

#include "semaphore.h"
#include "arch_sem.h"
#include "misc.h"

#ifndef PTHREAD_PROCESS_SHARED
    #define PTHREAD_PROCESS_SHARED      0
    #define PTHREAD_PROCESS_PRIVATE     1
#endif

#ifndef ETIMEDOUT
#define ETIMEDOUT       138
#endif

int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    arch_sem_t *pv;

    if (sem == NULL || value > (unsigned int) SEM_VALUE_MAX)
        return set_errno(EINVAL);

    if (pshared != PTHREAD_PROCESS_PRIVATE)
        return set_errno(EPERM);

    if (NULL == (pv = (arch_sem_t *)calloc(1, sizeof(arch_sem_t))))
        return set_errno(ENOMEM);

    if ((pv->handle = CreateSemaphore (NULL, 0, SEM_VALUE_MAX, NULL)) == NULL) {
        free(pv);
        return set_errno(ENOSPC);
    }

    pv->value = value;
    *sem = pv;
    return 0;
}

int sem_destroy(sem_t *sem)
{
    arch_sem_t *pv = (arch_sem_t *) sem;

    if (pv == NULL)
        return set_errno(EINVAL);

    if (pv->value < 0)
        return set_errno(EBUSY);

    if (CloseHandle (pv->handle) == 0)
        return set_errno(EINVAL);

    free(pv);
    return 0;
}

int sem_post(sem_t *sem)
{
    arch_sem_t *pv = (arch_sem_t *) sem;

    if (sem == NULL || pv == NULL)
        return set_errno(EINVAL);

    if (ReleaseSemaphore(pv->handle, 1, NULL) == 0)
        return set_errno(EINVAL);

    return 0;
}

int sem_wait(sem_t *sem)
{
    arch_sem_t *pv = (arch_sem_t *) sem;

    if (sem == NULL || pv == NULL)
        return set_errno(EINVAL);

    if (WaitForSingleObject(pv->handle, INFINITE) != WAIT_OBJECT_0)
        return set_errno(EPERM);

    return 0;
}

int sem_trywait(sem_t *sem)
{
    unsigned rc;
    arch_sem_t *pv = (arch_sem_t *) sem;

    if (sem == NULL || pv == NULL)
        return set_errno(EINVAL);

    if ((rc = WaitForSingleObject(pv->handle, 0)) == WAIT_OBJECT_0)
        return 0;

    if (rc == WAIT_TIMEOUT)
        return set_errno(ETIMEDOUT);

    return set_errno(EPERM);
}

int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
    unsigned rc;
    arch_sem_t *pv = (arch_sem_t *) sem;

    if (sem == NULL || pv == NULL)
        return set_errno(EINVAL);

    if ((rc = WaitForSingleObject(pv->handle, arch_rel_time_in_ms(abs_timeout))) == WAIT_OBJECT_0)
        return 0;

    if (rc == WAIT_TIMEOUT)
        return set_errno(ETIMEDOUT);

    return set_errno(EPERM);
}

sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value)
{
    unsigned rc;
    int len;
    char buffer[512];
    arch_sem_t *pv;

    if (value > (unsigned int) SEM_VALUE_MAX || (len = strlen(name)) > sizeof(buffer) - 8) {
        set_errno(EINVAL);
        return NULL;
    }

    if (NULL == (pv = (arch_sem_t *)calloc(1, sizeof(arch_sem_t)))) {
        set_errno(ENOMEM);
        return NULL;
    }

    memcpy(buffer, "Global\\", 7);
    memcpy(buffer + 7, name, len);
    buffer[len + 8] = '\0';

    /* Only the creater can unlink the semaphore ! */
    if((pv->handle = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE, FALSE, buffer)) != NULL) {
        if ((oflag & O_CREAT) && (oflag & O_EXCL)) {
            CloseHandle(pv->handle);
            free(pv);
            set_errno(EEXIST);
            return NULL;
        }
        return (sem_t *) pv;
    }

    rc = GetLastError();
    if (rc != ERROR_SEM_NOT_FOUND) {
        free(pv);
        set_errno(EPERM);
        return NULL;
    }

    if (!(oflag & O_CREAT)) {
        free(pv);
        set_errno(ENOENT);
        return NULL;
    }

    if ((pv->handle = CreateSemaphore (NULL, 0, SEM_VALUE_MAX, buffer)) == NULL)
    {
        free(pv);
        set_errno(ENOSPC);
        return NULL;
    }

    return (sem_t *) pv;
}

int sem_close(sem_t *sem)
{
    return sem_destroy(sem);
}

/*
 *  The semaphore object is destroyed when its last handle has been closed.
 */
int sem_unlink(const char *name)
{
    return 0;
}
