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

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

extern HANDLE libpthread_heap;

/**
 * Create a mutex attribute object.
 * @param a The pointer of the mutex attribute object.
 * @return Always return 0.
 * @remark We provide pthread_mutexattr_* functions only for compatibility,
 *         please use pthread_mutex_init(&mutex, NULL) for new applications.
 */
int pthread_mutexattr_init(pthread_mutexattr_t *a)
{
    *a = 0;
    return 0;
}

/**
 * Get the mutex type attribute.
 * @param a The pointer of the mutex attribute object.
 * @param type The mutex type.
 * @return Always return 0.
 * @remark We do not support mutex type attribute, this function does nothing.
 */
int pthread_mutexattr_gettype(const pthread_mutexattr_t *a, int *type)
{
    *type = 0;
    return 0;
}

/**
 * Set the mutex type attribute.
 * @param a The pointer of the mutex attribute object.
 * @param type The mutex type.
 * @return Always return 0.
 * @remark We do not support mutex type attribute, this function does nothing.
 */
int pthread_mutexattr_settype(pthread_mutexattr_t *a, int type)
{
    *a = 0;
    return 0;
}

/**
 * Get the mutex process-shared attribute.
 * @param a The pointer of the mutex attribute object.
 * @param pshared The process-shared attribute.
 * @return Always return 0.
 * @remark The only type we support is PTHREAD_PROCESS_PRIVATE.
 */
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *a, int *pshared)
{
    *pshared = PTHREAD_PROCESS_PRIVATE;
    return 0;
}

/**
 * Set the mutex process-shared attribute.
 * @param a The pointer of the mutex attribute object.
 * @param pshared The process-shared attribute.
 * @return Always return 0.
 * @remark The only type we support is PTHREAD_PROCESS_PRIVATE.
 */
int pthread_mutexattr_setpshared(pthread_mutexattr_t * a, int pshared)
{
    if (pshared != PTHREAD_PROCESS_PRIVATE)
        return set_errno(EINVAL);

    *a = 0;
    return 0;
}

/**
 * Get the mutex protocol attribute.
 * @param a The pointer of the mutex attribute object.
 * @param protocol The mutex protocol.
 * @return Always return 0.
 * @remark We do not support mutex protocol attribute, this function does nothing.
 */
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *a, int *protocol)
{
    *protocol = 0;
    return 0;
}

/**
 * Set the mutex protocol attribute.
 * @param a The pointer of the mutex attribute object.
 * @param protocol The mutex protocol.
 * @return Always return 0.
 * @remark We do not support mutex protocol attribute, this function does nothing.
 */
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *a, int protocol)
{
    *a = 0;
    return 0;
}

/**
 * Get the mutex prioceiling attribute.
 * @param a The pointer of the mutex attribute object.
 * @param prio The mutex prioceiling attribute.
 * @return Always return 0.
 * @remark We do not support mutex prioceiling attribute, this function does nothing.
 */
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *a, int *prio)
{
    *prio = 0;
    return 0;
}

/**
 * Set the mutex prioceiling attribute.
 * @param a The pointer of the mutex attribute object.
 * @param prio The mutex prioceiling attribute.
 * @return Always return 0.
 * @remark We do not support mutex prioceiling attribute, this function does nothing.
 */
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *a, int prio)
{
    *a = 0;
    return 0;
}

/**
 * Destroy a mutex attribute object.
 * @param a The pointer of the mutex attribute object.
 * @return Always return 0.
 * @remark We do not support mutex attribute, all pthread_mutexattr_* functions does nothing.
 */
int pthread_mutexattr_destroy(pthread_mutexattr_t *a)
{
    *a = 0;
    return 0;
}

static int arch_mutex_init(pthread_mutex_t *m)
{
    arch_thread_mutex *pv = HeapAlloc(libpthread_heap, HEAP_ZERO_MEMORY, sizeof(arch_thread_mutex));
    if (pv == NULL)
        return set_errno(ENOMEM);

    InitializeCriticalSection(& pv->mutex);
    if (atomic_cmpxchg_ptr(m, pv, NULL) != NULL) {
        DeleteCriticalSection(& pv->mutex);
        HeapFree(libpthread_heap, 0, pv);
    }

    return 0;
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
    return arch_mutex_init(m);
}

/**
 * Acquire a mutex lock.
 * @param m The pointer of the mutex object.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ENOMEM).
 */
int pthread_mutex_lock(pthread_mutex_t *m)
{
    arch_thread_mutex *pv;

    if (*m == NULL) {
        int rc = arch_mutex_init(m);
        if (rc != 0) return rc;
    }

    pv = (arch_thread_mutex *) *m;
    EnterCriticalSection(& pv->mutex);

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
    arch_thread_mutex *pv;

    if (*m == NULL) {
        int rc = arch_mutex_init(m);
        if (rc != 0) return rc;
    }

    pv = (arch_thread_mutex *) *m;
    if( 0 != TryEnterCriticalSection(& pv->mutex))
        return 0;

    return set_errno(EBUSY);
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
    arch_thread_mutex *pv = (arch_thread_mutex *) *m;
    if (pv != NULL) {
        LeaveCriticalSection(& pv->mutex);
        return 0;
    }

    return set_errno(EINVAL);
}

/**
 * Destroy a mutex lock.
 * @param m The pointer of the mutex object.
 * @return Always return 0.
 */
int pthread_mutex_destroy(pthread_mutex_t *m)
{
    arch_thread_mutex *pv = (arch_thread_mutex *) *m;
    if (pv != NULL) {
        DeleteCriticalSection(& pv->mutex);
        HeapFree(libpthread_heap, 0, pv);
    }

    return 0;
}
