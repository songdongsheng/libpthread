/*
 * Copyright (c) 2011, Dongsheng Song <songdongsheng@live.cn>
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file mutex.c
 * @brief Implementation Code of Mutex Routines
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

/**
 * Create a mutex attribute object.
 * @param attr The pointer of the mutex attribute object.
 * @return Always return 0.
 * @remark We provide pthread_mutexattr_* functions only for compatibility,
 *         please use pthread_mutex_init(&mutex, NULL) for new applications.
 */
int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    arch_mutex_attr *pv = calloc(1, sizeof(arch_mutex_attr));
    if (pv == NULL)
        return ENOMEM;

    pv->type = PTHREAD_MUTEX_DEFAULT;
    pv->pshared = PTHREAD_PROCESS_PRIVATE;
    pv->robust = PTHREAD_MUTEX_STALLED;

    *attr = pv;

    return 0;
}

/**
 * Get the mutex robust attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param robust The mutex robust attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    *robust = pv->robust;
    return 0;
}

/**
 * Set the mutex robust attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param robust The mutex robust attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    pv->robust = robust;
    return 0;
}

/**
 * Get the mutex type attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param type The mutex type.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    *type = pv->type;
    return 0;
}

/**
 * Set the mutex type attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param type The mutex type.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    pv->type = type;
    return 0;
}

/**
 * Get the mutex process-shared attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param pshared The process-shared attribute.
 * @return Always return 0.
 * @remark The only type we support is PTHREAD_PROCESS_PRIVATE.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    *pshared = pv->pshared;
    return 0;
}

/**
 * Set the mutex process-shared attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param pshared The process-shared attribute.
 * @return Always return 0.
 * @remark The only type we support is PTHREAD_PROCESS_PRIVATE.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    pv->pshared = pshared;
    return 0;
}

/**
 * Get the mutex protocol attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param protocol The mutex protocol.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    *protocol = pv->protocol;
    return 0;
}

/**
 * Set the mutex protocol attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param protocol The mutex protocol.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    pv->protocol = protocol;
    return 0;
}

/**
 * Get the mutex prioceiling attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param prioceiling The mutex prioceiling attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    *prioceiling = pv->prioceiling;
    return 0;
}

/**
 * Set the mutex prioceiling attribute.
 * @param attr The pointer of the mutex attribute object.
 * @param prioceiling The mutex prioceiling attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling)
{
    arch_mutex_attr *pv = (arch_mutex_attr *) attr;
    pv->prioceiling = prioceiling;
    return 0;
}

/**
 * Destroy a mutex attribute object.
 * @param attr The pointer of the mutex attribute object.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    if (attr != NULL) {
        free(*attr);
        *attr = NULL;
    }

    return 0;
}

static int arch_mutex_init(pthread_mutex_t *m, int lock)
{
    int cpu_count = get_ncpu();
    arch_mutex *pv = calloc(1, sizeof(arch_mutex));
    if (pv == NULL)
        return ENOMEM;

    /* see test_speed, about 1/2 the system call*/
    if (cpu_count > 1) pv->spin_count = 32;

    if (!lock) {
        *m = pv;
        return 0;
    }

    if (atomic_cmpxchg_ptr(m, pv, NULL) != NULL) {
        free(pv);
    }

    return 0;
}

static __inline int spin_lock_with_count(volatile long *lock, int count)
{
    int i = 0;

    do {
        if (atomic_cmpxchg((volatile long *) lock, 1, 0) == 0)
            return 1;
        cpu_relax();
    } while(++i < count);

    return 0;
}

static __inline void spin_unlock(volatile long *lock)
{
    *lock = 0;
}

static __inline void arch_mutex_init_handle(HANDLE *sync)
{
    HANDLE handle;

    while (*sync == NULL) {
        handle = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (handle != NULL) {
            if (atomic_cmpxchg_ptr(sync, handle, NULL) != NULL)
                (void) CloseHandle(handle);
            return;
        } else {
            (void) SleepEx(1000, TRUE); /* Waiting for resources available */
        }
    }
}

/**
 * Create a mutex object.
 * @param m The pointer of the mutex object.
 * @param a The pointer of the mutex attribute object.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ENOMEM).
 * @remark We provide pthread_mutexattr_* functions only for compatibility,
 *         please use pthread_mutex_init(&mutex, NULL) for new applications.
 */
int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a)
{
    *m = NULL;
    return arch_mutex_init(m, 0);
}

/**
 * Get the mutex prioceiling attribute.
 * @param mutex The pointer of the mutex object.
 * @param prioceiling The mutex prioceiling attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling)
{
    *prioceiling = 0;
    return 0;
}

/**
 * Set the mutex prioceiling attribute.
 * @param mutex The pointer of the mutex object.
 * @param prioceiling The new mutex prioceiling attribute.
 * @param old_ceiling The old mutex prioceiling attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling)
{
    *old_ceiling = 0;
    return 0;
}

/**
 * Mark state protected by robust mutex as consistent.
 * @param mutex The pointer of the mutex object.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_mutex_consistent(pthread_mutex_t *mutex)
{
    return 0;
}

/**
 * Acquire a mutex lock.
 * @param m The pointer of the mutex object.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ENOMEM, EDEADLK).
 */
int pthread_mutex_lock(pthread_mutex_t *m)
{
    arch_mutex *pv;

    if (*m == NULL) {
        int rc = arch_mutex_init(m, 1);
        if (rc != 0) return rc;
    }

    pv = (arch_mutex *) *m;

    while(1) {
        if (spin_lock_with_count(& pv->lock_status, pv->spin_count)) {
            /* pv->thread_id = GetCurrentThreadId(); */
            return 0;
        }

        if (pv->sync == NULL)
            arch_mutex_init_handle(& pv->sync);

        (void) atomic_fetch_and_add(& pv->wait, 1);
        /* Small probability event, but we must examine it. */
        if (atomic_cmpxchg((volatile long *) & pv->lock_status, 1, 0) == 0) {
            /* pv->thread_id = GetCurrentThreadId(); */
            (void) atomic_fetch_and_add(& pv->wait, -1);
            return 0;
        }
        (void) WaitForSingleObject(pv->sync, INFINITE);
        (void) atomic_fetch_and_add(& pv->wait, -1);
    }

    return 0;
}

/**
 * Try acquire a mutex lock.
 * @param m The pointer of the mutex object.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ENOMEM or EBUSY).
 */
int pthread_mutex_trylock(pthread_mutex_t *m)
{
    arch_mutex *pv;

    if (*m == NULL) {
        int rc = arch_mutex_init(m, 1);
        if (rc != 0) return rc;
    }

    pv = (arch_mutex *) *m;

    if (spin_lock_with_count(& pv->lock_status, pv->spin_count)) {
        /* pv->thread_id = GetCurrentThreadId(); */
        return 0;
    }

    return EBUSY;
}

/**
 * Release a mutex lock.
 * @param m The pointer of the mutex object.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (EINVAL).
 */
int pthread_mutex_unlock(pthread_mutex_t *m)
{
    arch_mutex *pv = (arch_mutex *) *m;
    if (pv != NULL) {
        /* pv->thread_id = 0; */
        atomic_set(& pv->lock_status, 0);
        if (atomic_read(& pv->wait))
            SetEvent(pv->sync);
        return 0;
    }

    return EINVAL;
}

/**
 * Destroy a mutex lock.
 * @param m The pointer of the mutex object.
 * @return Always return 0.
 */
int pthread_mutex_destroy(pthread_mutex_t *m)
{
    arch_mutex *pv = (arch_mutex *) *m;
    if (pv != NULL) {
        if (pv->sync != NULL) CloseHandle(pv->sync);
        free(pv);
    }

    return 0;
}
