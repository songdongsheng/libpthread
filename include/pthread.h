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

/* Tell we have POSIX threads.  */
#ifndef _POSIX_THREADS
#define _POSIX_THREADS          200809L
#endif

/* We support spinlocks.  */
#ifndef _POSIX_SPIN_LOCKS
#define _POSIX_SPIN_LOCKS       200809L
#endif

/* The barrier functions are available.  */
#ifndef _POSIX_BARRIERS
#define _POSIX_BARRIERS         200809L
#endif

/* Reader/Writer locks are available.  */
#ifndef _POSIX_READER_WRITER_LOCKS
#define _POSIX_READER_WRITER_LOCKS  200809L
#endif

/* We support the Timeouts option.  */
#ifndef _POSIX_TIMEOUTS
#define _POSIX_TIMEOUTS         200809L
#endif

/* We support user-defined stack sizes.  */
#ifndef _POSIX_THREAD_ATTR_STACKSIZE
#define _POSIX_THREAD_ATTR_STACKSIZE            200809L
#endif

/* The following options are not supported */
#undef _POSIX_THREAD_ATTR_STACKADDR
#define _POSIX_THREAD_ATTR_STACKADDR -1

#undef _POSIX_THREAD_PRIO_INHERIT
#define _POSIX_THREAD_PRIO_INHERIT -1

#undef _POSIX_THREAD_PRIO_PROTECT
#define _POSIX_THREAD_PRIO_PROTECT -1

#undef _POSIX_THREAD_PRIORITY_SCHEDULING
#define _POSIX_THREAD_PRIORITY_SCHEDULING -1

#undef _POSIX_THREAD_PROCESS_SHARED
#define _POSIX_THREAD_PROCESS_SHARED -1

/* POSIX Thread Definitions */
#define PTHREAD_KEYS_MAX            1024

#define PTHREAD_PROCESS_PRIVATE     0
#define PTHREAD_PROCESS_SHARED      1

#define PTHREAD_CREATE_JOINABLE     0
#define PTHREAD_CREATE_DETACHED     1

#define PTHREAD_INHERIT_SCHED       0
#define PTHREAD_EXPLICIT_SCHED      1

#define PTHREAD_PRIO_NONE           0
#define PTHREAD_PRIO_INHERIT        1
#define PTHREAD_PRIO_PROTECT        2

#define PTHREAD_SCOPE_PROCESS       0
#define PTHREAD_SCOPE_SYSTEM        1

#define PTHREAD_ONCE_INIT           0

#define PTHREAD_CANCELED            ((void*)-1)
#define PTHREAD_CANCEL_ENABLE       0
#define PTHREAD_CANCEL_DISABLE      1
#define PTHREAD_CANCEL_DEFERRED     0
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

#define PTHREAD_MUTEX_NORMAL        0
#define PTHREAD_MUTEX_RECURSIVE     1
#define PTHREAD_MUTEX_ERRORCHECK    2
#define PTHREAD_MUTEX_DEFAULT       PTHREAD_MUTEX_NORMAL

#define PTHREAD_MUTEX_STALLED       0
#define PTHREAD_MUTEX_ROBUST        1

#define PTHREAD_SPINLOCK_INITIALIZER    {0, 0}
#define PTHREAD_SPIN_RWLOCK_INITIALIZER {0, 0, 0}
#define PTHREAD_MUTEX_INITIALIZER       NULL
#define PTHREAD_RWLOCK_INITIALIZER      NULL
#define PTHREAD_COND_INITIALIZER        NULL

#define PTHREAD_BARRIER_SERIAL_THREAD   -1

typedef uintptr_t pthread_t;
typedef void *pthread_attr_t;

typedef long pthread_once_t;
typedef long pthread_key_t;

typedef void    *pthread_mutexattr_t;
typedef void    *pthread_condattr_t;
typedef void    *pthread_rwlockattr_t;
typedef void    *pthread_barrierattr_t;

typedef void    *pthread_mutex_t;
typedef void    *pthread_cond_t;
typedef void    *pthread_rwlock_t;
typedef void    *pthread_barrier_t;

typedef struct {
    long owner;
    long ticket;
} pthread_spinlock_t;

typedef struct {
    long owner;
    long ticket;
    long readers;
} pthread_spin_rwlock_t;

/*
    #include <signal.h>
    int pthread_sigmask(int how, const sigset_t *set, sigset_t *old_set);
    int sigprocmask(int how, const sigset_t *set, sigset_t *old_set);
 */
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
int pthread_getconcurrency(void);
int pthread_setconcurrency(int new_level);

int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *flag);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int flag);
int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *size);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t size);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *flag);
int pthread_attr_setinheritsched(pthread_attr_t *attr, int flag);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getstack(const pthread_attr_t *attr, void **addr, size_t *size);
int pthread_attr_setstack(pthread_attr_t *attr, void *addr, size_t size);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *size);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t size);
int pthread_attr_destroy(pthread_attr_t *attr);

int pthread_create(pthread_t *t, const pthread_attr_t *attr, void *(* start_routine)(void *), void *arg);
int pthread_once(pthread_once_t *once_control, void (* init_routine)(void));
pthread_t pthread_self(void);
int pthread_equal(pthread_t t1, pthread_t t2);
int pthread_detach(pthread_t t);
int pthread_join(pthread_t t, void **value_ptr);
void pthread_exit(void *value_ptr);

int pthread_setschedprio(pthread_t thread, int priority);
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
void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void *pointer);
int pthread_key_delete(pthread_key_t key);

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);
int pthread_spin_destroy(pthread_spinlock_t *lock);

int pthread_spin_rwlock_init(pthread_spin_rwlock_t *lock, int pshared);
int pthread_spin_rwlock_reader_lock(pthread_spin_rwlock_t *lock);
int pthread_spin_rwlock_reader_unlock(pthread_spin_rwlock_t *lock);
int pthread_spin_rwlock_writer_lock(pthread_spin_rwlock_t *lock);
int pthread_spin_rwlock_writer_unlock(pthread_spin_rwlock_t *lock);
int pthread_spin_rwlock_destroy(pthread_spin_rwlock_t *lock);

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling);
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling);
int pthread_mutex_consistent(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

int pthread_barrierattr_init(pthread_barrierattr_t *attr);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *s);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int s);
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);

int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count);
int pthread_barrier_wait(pthread_barrier_t *barrier);
int pthread_barrier_destroy(pthread_barrier_t *barrier);

int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_getclock(const pthread_condattr_t *attr, clockid_t *clock_id);
int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);
int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
int pthread_condattr_destroy(pthread_condattr_t *attr);

int pthread_cond_init(pthread_cond_t *cv, const pthread_condattr_t *attr);
int pthread_cond_signal (pthread_cond_t *cv);
int pthread_cond_broadcast (pthread_cond_t *cv);
int pthread_cond_wait (pthread_cond_t *cv, pthread_mutex_t *external_mutex);
int pthread_cond_timedwait(pthread_cond_t *cv, pthread_mutex_t *external_mutex, const struct timespec *t);
int pthread_cond_destroy(pthread_cond_t *cv);

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock, const struct timespec *abs_timeout);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock, const struct timespec *abs_timeout);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_destroy (pthread_rwlock_t *rwlock);

#ifdef __cplusplus
}
#endif

/** @} */
/** @} */

#endif /* _PTHREAD_H_ */
