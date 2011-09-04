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
 * @file spin.c
 * @brief Implementation Code of Spin Lock Routines
 */

#include <pthread.h>
#include <stdio.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

/**
 * Initialize a spin lock.
 * @param  lock The spin lock object.
 * @param  pshared Must be PTHREAD_PROCESS_PRIVATE (0).
 * @return If the pshared is PTHREAD_PROCESS_PRIVATE, the return value is 0.
 *         Otherwise, EINVAL returned to indicate the error.
 */
int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{
    if (PTHREAD_PROCESS_PRIVATE != pshared)
        return EINVAL;

    *lock = 0;
    return 0;
}

/**
 * Acquire a spin lock.
 * @param  lock The spin lock object.
 * @return Always return 0.
 */
int pthread_spin_lock(pthread_spinlock_t *lock)
{
    /* owner: 0~15, next: 16~31 */
    unsigned short ticket = (unsigned short) (atomic_fetch_and_add(lock, 0x10000) >> 16);

    while ((unsigned short) atomic_read(lock) != ticket)
        cpu_relax();

    return 0;
}

/**
 * Try acquire a spin lock.
 * @param  lock The spin lock object.
 * @return If it can acquire lock immediately, the return value is 0.
 *         Otherwise, EBUSY returned to indicate the error.
 */
int pthread_spin_trylock(pthread_spinlock_t *lock)
{
    /* owner: 0~15, next: 16~31 */
    long tmp = atomic_read(lock);
    if ((unsigned short) tmp == (unsigned short) (tmp >> 16)) {
        if (atomic_cmpxchg(lock, tmp + 0x10000, tmp) == tmp)
            return 0;
    }

    return EBUSY;
}

/**
 * Release a spin lock.
 * @param  lock The spin lock object.
 * @return Always return 0.
 */
int pthread_spin_unlock(pthread_spinlock_t *lock)
{
    /* owner: 0~15, next: 16~31 */
#ifdef _MSC_VER
    unsigned short owner = (unsigned short) (*lock + 1);
    *lock = (*lock & 0xFFFF0000) | owner;
#else
    asm volatile("incw %0"
                 : "+m" (lock)
                 :
                 : "memory", "cc");
#endif
    return 0;
}

/**
 * Destroy a spin lock (reset to unlocked state).
 * @param  lock The spin lock object.
 * @return Always return 0.
 */
int pthread_spin_destroy(pthread_spinlock_t *lock)
{
    *lock = 0;
    return 0;
}
