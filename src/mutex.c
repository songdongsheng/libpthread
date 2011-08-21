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

#include <pthread.h>
#include <stdio.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

extern HANDLE libpthread_heap;

static arch_thread_mutex * arch_mutex_init(pthread_mutex_t *m)
{
    arch_thread_mutex *pv = HeapAlloc(libpthread_heap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, sizeof(arch_thread_mutex));
    InitializeCriticalSection(& pv->mutex);
    return pv;
}

int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a)
{
    *m = (pthread_mutex_t *) arch_mutex_init(m);

    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *m)
{
    arch_thread_mutex *pv = (arch_thread_mutex *) *m;
    if (pv != NULL) {
        DeleteCriticalSection(& pv->mutex);
        HeapFree(libpthread_heap, 0, pv);
    }

    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *m)
{
    arch_thread_mutex *pv;

    if (*m == NULL) /* TODO: Lock */
        *m = (pthread_mutex_t *) arch_mutex_init(m);

    pv = (arch_thread_mutex *) *m;

    EnterCriticalSection(& pv->mutex);
    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *m)
{
    arch_thread_mutex *pv;

    if (*m == NULL) /* TODO: Lock */
        *m = (pthread_mutex_t *) arch_mutex_init(m);

    pv = (arch_thread_mutex *) *m;

    if( 0 != TryEnterCriticalSection(& pv->mutex))
        return 0;
    return set_errno(EBUSY);
}

int pthread_mutex_timedlock(pthread_mutex_t *m, const struct timespec *ts)
{
    return pthread_mutex_lock(m);
}

int pthread_mutex_unlock(pthread_mutex_t *m)
{
    arch_thread_mutex *pv = (arch_thread_mutex *) *m;
    if (pv != NULL) {
        LeaveCriticalSection(& pv->mutex);
        return 0;
    }

    return set_errno(EINVAL);
}
