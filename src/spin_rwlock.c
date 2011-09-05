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
 * @file spin_rwlock.c
 * @brief Implementation Code of Spin RWLock Routines
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

/**
 * Initialize a spin rwlock.
 * @param  lock The spin rwlock object.
 * @return If it can acquire memory immediately, the return value is 0.
 *         Otherwise, ENOMEM returned to indicate the error.
 */
int pthread_spin_rwlock_init(pthread_spin_rwlock_t *lock)
{
    arch_spin_rwlock *pv = calloc(1, sizeof(arch_spin_rwlock));
    if (pv == NULL)
        return ENOMEM;

    *lock = pv;

    return 0;
}

/**
 * Acquire a spin rdlock.
 * @param  lock The spin rwlock object.
 * @return Always return 0.
 */
int pthread_spin_rwlock_reader_lock(pthread_spin_rwlock_t *lock)
{
    int id;
    arch_spin_rwlock *pv = (arch_spin_rwlock *) *lock;
    if (pv == NULL)
        return EINVAL;

    id = atomic_fetch_and_add(& pv->ticket, 1);
    while (atomic_read(& pv->owner) != id)
        cpu_relax();

    atomic_fetch_and_add(& pv->readers, 1);
    pv->owner++;

    return 0;
}

/**
 * Release a spin rdlock.
 * @param  lock The spin rwlock object.
 * @return Always return 0.
 */
int pthread_spin_rwlock_reader_unlock(pthread_spin_rwlock_t *lock)
{
    arch_spin_rwlock *pv = (arch_spin_rwlock *) *lock;
    if (pv == NULL)
        return EINVAL;

    atomic_fetch_and_add(& pv->readers, -1);

    return 0;
}

/**
 * Acquire a spin wrlock.
 * @param  lock The spin rwlock object.
 * @return Always return 0.
 */
int pthread_spin_rwlock_writer_lock(pthread_spin_rwlock_t *lock)
{
    int id;
    arch_spin_rwlock *pv = (arch_spin_rwlock *) *lock;
    if (pv == NULL)
        return EINVAL;

    id = atomic_fetch_and_add(& pv->ticket, 1);
    while (atomic_read(& pv->owner) != id)
        cpu_relax();

    while (atomic_read(& pv->readers) > 0)
        cpu_relax();

    return 0;
}

/**
 * Release a spin wrlock.
 * @param  lock The spin rwlock object.
 * @return Always return 0.
 */
int pthread_spin_rwlock_writer_unlock(pthread_spin_rwlock_t *lock)
{
    arch_spin_rwlock *pv = (arch_spin_rwlock *) *lock;
    if (pv == NULL)
        return EINVAL;

    pv->owner++;

    return 0;
}

/**
 * Destroy a spin lock.
 * @param  lock The spin rwlock object.
 * @return Always return 0.
 */
int pthread_spin_rwlock_destroy(pthread_spin_rwlock_t *lock)
{
    arch_spin_rwlock *pv = (arch_spin_rwlock *) *lock;
    if (pv != NULL)
        free(pv);

    return 0;
}
