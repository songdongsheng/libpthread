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
    /* owner: 0~15, ticket: 16~31 */
#if 1
    long ticket = 0xFFFF & (atomic_fetch_and_add(lock, 0x10000) >> 16);

    while ((0xFFFF & atomic_read(lock)) != ticket)
        cpu_relax();
#else
    int inc = 0x10000;
    int tmp;

    asm volatile("lock xaddl %0, %1\n"
                 "movzwl %w0, %2\n\t"
                 "shrl $16, %0\n\t"
                 "1:\t"
                 "cmpl %0, %2\n\t"
                 "je 2f\n\t"
                 "pause\n\t"
                 "movzwl %1, %2\n\t"
                 "jmp 1b\n"
                 "2:"
                 : "+r" (inc), "+m" (*lock), "=&r" (tmp)
                 :
                 : "memory", "cc");
#endif

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
    /* owner: 0~15, ticket: 16~31 */
#if 1
    long tmp = atomic_read(lock);
    if ((tmp & 0xFFFF) == (0xFFFF & (tmp >> 16))) {
        if (atomic_cmpxchg(lock, tmp + 0x10000, tmp) == tmp)
            return 0;
    }

    return EBUSY;
#else
    int tmp;
    int ticket;

    asm volatile("movl %2, %0\n\t"
                 "movl %0, %1\n\t"
                 "roll $16, %0\n\t"
                 "cmpl %0, %1\n\t"
                 "jne 1f\n\t"
                 "addl $0x10000, %1\n\t"
                 "lock cmpxchgl %1, %2\n\t"
                 "1:"
                 "sete %b1\n\t"
                 "movzbl %b1, %0\n\t"
                 : "=&a" (tmp), "=&q" (ticket), "+m" (*lock)
                 :
                 : "memory", "cc");

    return tmp ? 0 : EBUSY;
#endif
}

/**
 * Release a spin lock.
 * @param  lock The spin lock object.
 * @return Always return 0.
 */
int pthread_spin_unlock(pthread_spinlock_t *lock)
{
    /* owner: 0~15, ticket: 16~31 */

#if 1
    long owner = 0xFFFF & (*lock + 1);
    *lock = (*lock & 0xFFFF0000) | owner;
#else
    asm volatile("incw %0"
                 : "+m" (*lock)
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
