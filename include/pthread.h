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

#ifndef _PTHREAD_H_
#define _PTHREAD_H_

/**
 * @file pthread.h
 * @brief POSIX Thread Definitions and Routines
 */

/**
 * @defgroup libpthread POSIX Thread Library for Windows
 * @{
 */

/**
 * @defgroup thread POSIX Thread Definitions and Routines
 * @{
 */

#include <pthread_types.h>
#include <sched.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PTHREAD_KEYS_MAX            1024

#define PTHREAD_PROCESS_PRIVATE     0
#define PTHREAD_PROCESS_SHARED      1

#define PTHREAD_CREATE_JOINABLE     0
#define PTHREAD_CREATE_DETACHED     1

#define PTHREAD_ONCE_INIT           0

#define PTHREAD_MUTEX_NORMAL        0
#define PTHREAD_MUTEX_RECURSIVE     1
#define PTHREAD_MUTEX_ERRORCHECK    2
#define PTHREAD_MUTEX_DEFAULT       PTHREAD_MUTEX_NORMAL

#define PTHREAD_SPINLOCK_INITIALIZER    0
#define PTHREAD_MUTEX_INITIALIZER       NULL
#define PTHREAD_RWLOCK_INITIALIZER      NULL
#define PTHREAD_COND_INITIALIZER        NULL

typedef uintptr_t pthread_t;
typedef void *pthread_attr_t;

typedef unsigned long pthread_once_t;
typedef unsigned long pthread_key_t;
typedef unsigned long pthread_spinlock_t;

typedef unsigned long pthread_mutexattr_t;
typedef unsigned long pthread_condattr_t;
typedef unsigned long pthread_rwlockattr_t;

typedef void    *pthread_mutex_t;
typedef void    *pthread_cond_t;
typedef void    *pthread_rwlock_t;

typedef void    *pthread_barrier_t;
typedef void    *pthread_barrierattr_t;

int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int flag);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *flag);
int pthread_attr_setinheritsched(pthread_attr_t *attr, int flag);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *flag);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *size);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t size);

int pthread_create(pthread_t *t, const pthread_attr_t *attr, void *(* start_routine)(void *), void *arg);
int pthread_join(pthread_t t, void **value_ptr);
int pthread_detach(pthread_t t);
int pthread_once(pthread_once_t *once_control, void (* init_routine)(void));
pthread_t pthread_self(void);
int pthread_equal(pthread_t t1, pthread_t t2);
void pthread_exit(void *value_ptr);

int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);

void pthread_cleanup_push(void (*cleanup_routine)(void *), void *arg);
void pthread_cleanup_pop(int execute);

int pthread_kill(pthread_t t, int sig);
int pthread_cancel(pthread_t t);
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
void pthread_testcancel(void);

int pthread_key_create(pthread_key_t *key, void (* destructor)(void *));
int pthread_key_delete(pthread_key_t key);
void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void *pointer);

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);
int pthread_spin_destroy(pthread_spinlock_t *lock);

int pthread_mutexattr_init(pthread_mutexattr_t *a);
int pthread_mutexattr_destroy(pthread_mutexattr_t *a);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *a, int *type);
int pthread_mutexattr_settype(pthread_mutexattr_t *a, int type);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *a, int *pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t * a, int pshared);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *a, int *protocol);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *a, int protocol);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *a, int *prio);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *a, int prio);

int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a);
int pthread_mutex_destroy(pthread_mutex_t *m);
int pthread_mutex_lock(pthread_mutex_t *m);
int pthread_mutex_trylock(pthread_mutex_t *m);
int pthread_mutex_unlock(pthread_mutex_t *m);

int pthread_barrierattr_init(void **attr);
int pthread_barrierattr_destroy(void **attr);
int pthread_barrierattr_setpshared(void **attr, int s);
int pthread_barrierattr_getpshared(void **attr, int *s);

int pthread_barrier_destroy(pthread_barrier_t *b);
int pthread_barrier_init(pthread_barrier_t *b, const void *attr, unsigned int count);
int pthread_barrier_wait(pthread_barrier_t *b);

int pthread_condattr_destroy(pthread_condattr_t *a);
int pthread_condattr_init(pthread_condattr_t *a);
int pthread_condattr_getpshared(const pthread_condattr_t *a, int *s);
int pthread_condattr_setpshared(pthread_condattr_t *a, int s);

int pthread_cond_init(pthread_cond_t *cv, const pthread_condattr_t *a);
int pthread_cond_destroy(pthread_cond_t *cv);
int pthread_cond_signal (pthread_cond_t *cv);
int pthread_cond_broadcast (pthread_cond_t *cv);
int pthread_cond_wait (pthread_cond_t *cv, pthread_mutex_t *external_mutex);
int pthread_cond_timedwait(pthread_cond_t *cv, pthread_mutex_t *external_mutex, const struct timespec *t);

int pthread_rwlock_init(pthread_rwlock_t *rwlock_, const pthread_rwlockattr_t *attr);
int pthread_rwlock_wrlock(pthread_rwlock_t *l);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock, const struct timespec *ts);
int pthread_rwlock_rdlock(pthread_rwlock_t *l);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *l, const struct timespec *ts);
int pthread_rwlock_unlock(pthread_rwlock_t *l);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *l);
int pthread_rwlock_trywrlock(pthread_rwlock_t *l);
int pthread_rwlock_destroy (pthread_rwlock_t *l);

#ifdef __cplusplus
}
#endif

/** @} */
/** @} */

#endif /* _PTHREAD_H_ */
