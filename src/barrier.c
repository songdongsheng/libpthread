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
 * @file barrier.c
 * @brief Implementation Code of Barrier Routines
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

/**
 * Create a barrier attribute object.
 * @param attr The pointer of the barrier attribute object.
 * @return Always return 0.
 * @remark We provide pthread_barrierattr_* functions only for compatibility,
 *         please use pthread_barrier_init(&barrier, NULL, count) for new applications.
 */
int pthread_barrierattr_init(pthread_barrierattr_t *attr)
{
    arch_barrier_attr *pv = calloc(1, sizeof(arch_barrier_attr));
    if (pv == NULL)
        return ENOMEM;

    pv->pshared = PTHREAD_PROCESS_PRIVATE;

    *attr = pv;

    return 0;
}

/**
 * Get the barrier process-shared attribute.
 * @param attr The pointer of the barrier attribute object.
 * @param pshared The process-shared attribute.
 * @return Always return 0.
 * @remark The only type we support is PTHREAD_PROCESS_PRIVATE.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared)
{
    arch_barrier_attr *pv = (arch_barrier_attr *) attr;
    *pshared = pv->pshared;
    return 0;
}

/**
 * Set the barrier process-shared attribute.
 * @param attr The pointer of the barrier attribute object.
 * @param pshared The process-shared attribute.
 * @return Always return 0.
 * @remark The only type we support is PTHREAD_PROCESS_PRIVATE.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared)
{
    arch_barrier_attr *pv = (arch_barrier_attr *) attr;
    pv->pshared = pshared;
    return 0;
}

/**
 * Destroy a barrier attribute object.
 * @param attr The pointer of the barrier attribute object.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr)
{
    if (attr != NULL) {
        free(*attr);
        *attr = NULL;
    }

    return 0;
}

/**
 * Create a barrier object.
 * @param barrier The pointer of the barrier object.
 * @param attr The pointer of the barrier attribute object.
 * @param count The number of threads that must call pthread_barrier_wait()
 *        before  any  of  them successfully return from the call.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (EINVAL, ENOMEM, or EAGAIN).
 * @remark We provide pthread_barrierattr_* functions only for compatibility,
 *         please use pthread_barrier_init(&barrier, NULL, count) for new applications.
 */
int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count)
{
    arch_barrier *pv;

    if (count < 1)
        return lc_set_errno(EINVAL);

    if ((pv = calloc(1, sizeof(arch_barrier))) == NULL)
        return lc_set_errno(ENOMEM);

    if ((pv->semaphore[0] = CreateSemaphore(NULL, 0, count, NULL)) == NULL)
        return lc_set_errno(EAGAIN);

    if ((pv->semaphore[1] = CreateSemaphore(NULL, 0, count, NULL)) == NULL) {
        long rc = GetLastError();
        CloseHandle(pv->semaphore[0]);
        SetLastError(rc);
        return lc_set_errno(EAGAIN);
    }

    pv->total = count;
    pv->count = count;
    *barrier = pv;

    return 0;
}

/**
 * Wait on a barrier lock.
 * @param m The pointer of the barrier object.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (EINVAL).
 */
int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    long rc, index;
    arch_barrier *pv = (arch_barrier *) *barrier;

    if (pv == NULL)
        return lc_set_errno(EINVAL);

    if (atomic_fetch_and_add(& pv->count, -1) != 1) {
        WaitForSingleObject(pv->semaphore[pv->index], INFINITE);
        return 0;
    }

    index = pv->index;
    if (index) pv->index = 0; else pv->index = 1;

    pv->count = pv->total;
    if (pv->total > 1)
        ReleaseSemaphore(pv->semaphore[index], pv->total - 1, NULL);

    return PTHREAD_BARRIER_SERIAL_THREAD;
}

/**
 * Destroy a barrier lock.
 * @param m The pointer of the barrier object.
 * @return Always return 0.
 */
int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
    arch_barrier *pv = (arch_barrier *) *barrier;
    if (pv != NULL) {
        if (pv->semaphore[0] != NULL) CloseHandle(pv->semaphore[0]);
        if (pv->semaphore[1] != NULL) CloseHandle(pv->semaphore[1]);
        free(pv);
    }

    return 0;
}
