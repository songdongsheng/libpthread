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
 *         Otherwise, the return value is -1, with errno set to EINVAL.
 */
int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{
    if (PTHREAD_PROCESS_PRIVATE != pshared)
        return set_errno(EINVAL);

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
    while(atomic_cmpxchg(lock, 1, 0) != 0)
        cpu_relax();

    return 0;
}

/**
 * Try acquire a spin lock.
 * @param  lock The spin lock object.
 * @return If it can acquire lock immediately, the return value is 0.
 *         Otherwise, the return value is -1, with errno set to EBUSY.
 */
int pthread_spin_trylock(pthread_spinlock_t *lock)
{
    long rv = atomic_cmpxchg(lock, 1, 0);
    if (rv == 0) return 0;
    return set_errno(EBUSY);
}

/**
 * Release a spin lock.
 * @param  lock The spin lock object.
 * @return Always return 0.
 */
int pthread_spin_unlock(pthread_spinlock_t *lock)
{
    *lock = 0;
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
