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
 * @file pthread.c
 * @brief Implementation Code of Thread Routines
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

extern DWORD libpthread_tls_index;

/**
 * Register fork handlers.
 * @param  prepare The prepare fork handler shall be called before fork() processing commences.
 * @param  parent The parent fork handle shall be called after fork() processing completes in the parent process.
 * @param  child The child fork handler shall be called after fork() processing completes in the child process.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    return 0;
}

static int libpthread_concurrency = 0;

/**
 * Get the level of concurrency.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_getconcurrency(void)
{
    return libpthread_concurrency;
}

/**
 * Set the level of concurrency.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_setconcurrency(int new_level)
{
    libpthread_concurrency = new_level;
    return 0;
}

/**
 * Initialize thread attributes object.
 * @param  attr The thread attributes object.
 * @return Always return 0.
 */
int pthread_attr_init(pthread_attr_t *attr)
{
    arch_thread_attr *pv = calloc(1, sizeof(arch_thread_attr));
    if (pv == NULL)
        return lc_set_errno(ENOMEM);

    pv->sched_policy = SCHED_OTHER;
    pv->sched_param.sched_priority = 8;

    *attr = pv;

    return 0;
}

/**
 * Get detach state in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  flag The detach state pointer.
 * @return Always return 0.
 */
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *flag)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    *flag = pv->detach_state;
    return 0;
}

/**
 * Set detach state in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  flag The detach state pointer.
 * @return Always return 0.
 */
int pthread_attr_setdetachstate(pthread_attr_t *attr, int flag)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->detach_state = flag;
    return 0;
}

/**
 * Get the thread guardsize attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *size)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    *size = pv->guard_size;
    return 0;
}

/**
 * Set the thread guardsize attribute.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t size)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->guard_size = size;
    return 0;
}

/**
 * Get inherit scheduler attribute in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  flag The detach state pointer.
 * @return Always return 0.
 */
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *flag)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    *flag = pv->inherit_sched;
    return 0;
}

/**
 * Set inherit scheduler attribute in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  flag The detach state pointer.
 * @return Always return 0.
 */
int pthread_attr_setinheritsched(pthread_attr_t *attr, int flag)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->inherit_sched = flag;
    return 0;
}

/**
 * Set scheduling parameter attributes in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  param The scheduling parameter.
 * @return Always return 0.
 */
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->sched_param.sched_priority = param->sched_priority;
    return 0;
}

/**
 * Get scheduling parameter attributes in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  param The scheduling parameter.
 * @return Always return 0.
 */
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    param->sched_priority = pv->sched_param.sched_priority;
    return 0;
}

/**
 * Set the scheduling policy attribute.
 * @param  attr The thread attributes object.
 * @param  policy The scheduling policy parameter.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    *policy = pv->sched_policy;
    return 0;
}

/**
 * Set the scheduling policy attribute.
 * @param  attr The thread attributes object.
 * @param  policy The scheduling policy parameter.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->sched_policy = policy;
    return 0;
}

/**
 * Get the contention scope attribute.
 * @param  attr The thread attributes object.
 * @param  scope The contention scope parameter.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_getscope(const pthread_attr_t *attr, int *scope)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    *scope = pv->scope;
    return 0;
}

/**
 * Set the contention scope attribute.
 * @param  attr The thread attributes object.
 * @param  scope The contention scope parameter.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->scope = scope;
    return 0;
}

/**
 * Get the stack attribute.
 * @param  attr The thread attributes object.
 * @param  addr The stack address parameter.
 * @param  size The stack size parameter.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_getstack(const pthread_attr_t *attr, void **addr, size_t *size)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;

    if (addr != NULL) *addr = pv->stack_addr;
    if (size != NULL) *size = pv->stack_size;

    return 0;
}

/**
 * Set the stack attribute.
 * @param  attr The thread attributes object.
 * @param  addr The stack address parameter.
 * @param  size The stack size parameter.
 * @return Always return 0.
 * @remark This function is provided for source code compatibility but no effect when called.
 */
int pthread_attr_setstack(pthread_attr_t *attr, void *addr, size_t size)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;

    pv->stack_addr = addr;
    pv->stack_size = size;

    return 0;
}

/**
 * Get stack size attribute in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  size The stack size pointer.
 * @return Always return 0.
 */
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *size)
{
    arch_thread_attr *pv = (arch_thread_attr *) *attr;
    *size = pv->stack_size;
    return 0;
}

/**
 * Set stack size attribute in thread attributes object.
 * @param  attr The thread attributes object.
 * @param  size The stack size pointer.
 * @return Always return 0.
 */
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t size)
{
    arch_thread_attr *pv = (arch_thread_attr *) attr;
    pv->stack_size = size;
    return 0;
}

/**
 * Destroy thread attributes object.
 * @param  attr The thread attributes object.
 * @return Always return 0.
 */
int pthread_attr_destroy(pthread_attr_t *attr)
{
    if (attr != NULL) {
        free(*attr);
        *attr = NULL;
    }

    return 0;
}

/**
 * Send a signal to a specified thread.
 *
 * @param  thread The thread to be signaled.
 * @param  sig The signal to be send.
 * @return Always return 0.
 *
 * @remark Windows don't really well support signals.
 * In addition to return 0, this function does nothing.
 */
int pthread_kill(pthread_t thread, int sig)
{
    return 0;
}

/**
 * Cancel execution of a thread.
 *
 * @param  thread The thread to be canceled.
 * @return Always return 0.
 *
 * @bug We do not support cancel point (yet).
 * In addition to return 0, this function does nothing.
 */
int pthread_cancel(pthread_t thread)
{
    return 0;
}

/**
 * Atomically both sets the calling thread's cancelability state to the
 * indicated state and, if oldstate is not NULL, returns the previous
 * cancelability state at the location referenced by oldstate. Legal
 * values for state are PTHREAD_CANCEL_ENABLE and PTHREAD_CANCEL_DISABLE.
 *
 * @param  state The new cancelability state.
 * @param  oldstate The old cancelability state.
 * @return Always return 0.
 *
 * @bug We do not support cancel point (yet).
 * In addition to return 0, this function does nothing.
 */
int pthread_setcancelstate(int state, int *oldstate)
{
    return 0;
}

/**
 * Atomically both sets the calling thread's cancelability type to the
 * indicated type and, if oldtype is not NULL, returns the previous
 * cancelability type at the location referenced by oldtype.  Legal
 * values for type are PTHREAD_CANCEL_DEFERRED and PTHREAD_CANCEL_ASYNCHRONOUS.
 *
 * @param  type The new cancelability type.
 * @param  oldtype The old cancelability type.
 * @return Always return 0.
 *
 * @bug We do not support cancel point (yet).
 * In addition to return 0, this function does nothing.
 */
int pthread_setcanceltype(int type, int *oldtype)
{
    return 0;
}

/**
 * Create a cancellation point in the calling thread.
 *
 * @bug We do not support cancel point (yet).
 * In addition to return 0, this function does nothing.
 */
void pthread_testcancel(void)
{
    return;
}

/**
 * Add a cleanup function for thread exit.
 *
 * @param  cleanup_routine The cleanup routine to be called.
 * @param  arg The argument of cleanup routine.
 * @bug The main thread do not support cleanup routines.
 */
void pthread_cleanup_push(void (*cleanup_routine)(void *), void *arg)
{
    arch_thread_cleanup_list *next;
    arch_thread_info *pv = TlsGetValue(libpthread_tls_index);

    if (pv != NULL) {
        arch_thread_cleanup_list *node = calloc(1, sizeof(arch_thread_cleanup_list));

        node->arg = arg;
        node->cleaner = cleanup_routine;

        if (pv->cleanup_list == NULL) {
            pv->cleanup_list = node;
            return;
        }

        next = pv->cleanup_list;
        while(next->next != NULL)
            next = next->next;

        next->next = node;
        node->prev = next;
    }
}

/**
 * Pop and call the top cleanup routine.
 * The pthread_cleanup_pop() function pops the top cleanup routine off
 * of the current threads cleanup routine stack, and, if execute is
 * non-zero, it will execute the function. If there is no cleanup
 * routine then pthread_cleanup_pop() does nothing.
 *
 * @param  execute If execute is non-zero, the top-most clean-up handler
 * is popped and executed.
 * @bug The main thread do not support cleanup routines.
 */
void pthread_cleanup_pop(int execute)
{
    arch_thread_info *pv = TlsGetValue(libpthread_tls_index);
    if (pv != NULL) {
        arch_thread_cleanup_list *node = pv->cleanup_list, *prev;

        if (node == NULL) return;
        while(node->next != NULL)
            node = node->next;

        if (execute) {
            node->cleaner(node->arg);
        }

        prev = node->prev;
        free(node);
        if(prev == NULL)
            pv->cleanup_list = NULL;
        else
            prev->next = NULL;
    }
}

static unsigned int __stdcall worker_proxy (void *arg)
{
    arch_thread_info *pv = (arch_thread_info *) arg;

    TlsSetValue(libpthread_tls_index, pv);

    pv->return_value = pv->worker(pv->arg);

    /* Free memory used by clean-up handlers */
    if (pv->cleanup_list) {
        arch_thread_cleanup_list *node = pv->cleanup_list;
        do {
            arch_thread_cleanup_list *next = node->next;
            /* Cleanup handlers are not called if the thread terminates by
             * performing a return from the thread start function.
             *
             * node->cleaner(node->arg);
             */
            free(node);
            node = next;
        } while(node != NULL);
        pv->cleanup_list = NULL;
    }

    /* Make sure we free ourselves if we are detached */
    if ((pv->state & PTHREAD_CREATE_DETACHED) == PTHREAD_CREATE_DETACHED) {
        CloseHandle (pv->handle);
        free(pv);
        TlsSetValue(libpthread_tls_index, NULL);
    }

    return 0;
}

/**
 * Create a new thread.
 * @param thread The new thread.
 * @param attr The thread attributes object.
 * @param start_routine The application-defined function to be executed by the new thread.
 * @param arg The pointer to a variable to be passed to the thread.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
    unsigned stack_size = 0;
    arch_thread_info *pv = calloc(1, sizeof(arch_thread_info));
    if (pv == NULL)
        return lc_set_errno(ENOMEM);

    if (attr != NULL) stack_size = ((arch_thread_attr * ) attr)->stack_size;

    pv->arg = arg;
    pv->worker = start_routine;
    pv->state = PTHREAD_CREATE_JOINABLE;
    pv->handle = (HANDLE) _beginthreadex(NULL, stack_size, worker_proxy, pv, CREATE_SUSPENDED, NULL);

    if (pv->handle == INVALID_HANDLE_VALUE) {
        free(pv);
        return errno;
    }

    if (attr != NULL) {
        SetThreadPriority(pv->handle, sched_priority_to_os_priority(((arch_thread_attr * ) attr)->sched_param.sched_priority));

        if ((((arch_thread_attr * ) attr)->detach_state & PTHREAD_CREATE_DETACHED) != 0) {
            CloseHandle(pv->handle);
            pv->state = PTHREAD_CREATE_DETACHED;
            pv->handle = NULL;
        }
    }

    ResumeThread(pv->handle);
    *thread = (pthread_t) pv;
    return 0;
}

/**
 * Terminate calling thread.
 * @param value_ptr The pointer of the calling thread return value.
 */
void pthread_exit(void *value_ptr)
{
    arch_thread_info *pv = TlsGetValue(libpthread_tls_index);
    if (pv != NULL) {
        pv->return_value = value_ptr;

        /* Call clean-up handlers, and free memory */
        if (pv->cleanup_list) {
            arch_thread_cleanup_list *node = pv->cleanup_list;
            while(node->next != NULL)
                node = node->next;
            do {
                arch_thread_cleanup_list *prev = node->prev;
                node->cleaner(node->arg);
                free(node);
                node = prev;
            } while(node != NULL);
            pv->cleanup_list = NULL;
        }

        /* Make sure we free ourselves if we are detached */
        if ((pv->state & PTHREAD_CREATE_DETACHED) == PTHREAD_CREATE_DETACHED) {
            CloseHandle (pv->handle);
            free(pv);
            TlsSetValue(libpthread_tls_index, NULL);
        }

        _endthreadex(0);
    } else {
        exit(1); /* User should not call pthread_exit in the main thread */
    }
}

/**
 * Get scheduling policy and parameters of a thread.
 * @param thread The target thread.
 * @param  policy Always return SCHED_OTHER.
 * @param  param The thread scheduling priority.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ESRCH).
 */
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param)
{
    if (policy != NULL)
        *policy = SCHED_OTHER;

    if (param != NULL) {
        int priority;
        HANDLE handle;
        arch_thread_info *pv = (arch_thread_info *) thread;

        if (pv != NULL) handle = pv->handle;
        else handle = GetCurrentThread();

        priority = GetThreadPriority(handle);
        if (priority == THREAD_PRIORITY_ERROR_RETURN)
            return lc_set_errno(ESRCH);
        param->sched_priority = os_priority_to_sched_priority(priority);
    }

    return 0;
}

/**
 * Set scheduling policy and parameters of a thread.
 * @param thread The target thread.
 * @param  policy Ignored.
 * @param  param The thread scheduling priority.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ESRCH).
 */
int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param)
{
    if (param != NULL)
        return pthread_setschedprio(thread, param->sched_priority);

    return 0;
}

/**
 * Set scheduling priority of a thread.
 * @param thread The target thread.
 * @param  priority The thread scheduling priority.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error (ESRCH).
 */
int pthread_setschedprio(pthread_t thread, int priority)
{
    HANDLE handle;
    arch_thread_info *pv = (arch_thread_info *) thread;

    if (pv != NULL) handle = pv->handle;
    else handle = GetCurrentThread();

    if (SetThreadPriority(handle, sched_priority_to_os_priority(priority)) == 0)
        return lc_set_errno(ESRCH);

    return 0;
}

/**
 * Detach a thread.
 *
 * @param t The thread to be detached.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 * @bug The main thread do not support detach.
 */
int pthread_detach (pthread_t t)
{
    DWORD dwFlags;
    arch_thread_info *pv = (arch_thread_info *) t;
    if (pv != NULL) {
        pv->state |= PTHREAD_CREATE_DETACHED;

        if (pv == NULL || pv->handle == NULL || !GetHandleInformation(pv->handle, &dwFlags))
            return ESRCH;

        if (pv->handle != NULL)
            CloseHandle(pv->handle);
    }

    return 0;
}

/**
 * Get the calling thread's ID.
 * @return The calling thread's ID.
 * @bug The pthread_self() function returns NULL for main thread.
 * I don't think the main thread should support join, detach, or cleanup routines.
 */
pthread_t pthread_self(void)
{
    return (pthread_t) TlsGetValue(libpthread_tls_index);
}

/**
 * Compare thread IDs.
 * @return If the thread IDs t1 and t2 correspond to the same thread,
 * the return value is non-zero, otherwise it will return zero.
 */
int pthread_equal(pthread_t t1, pthread_t t2)
{
    return t1 == t2;
}

/**
 * Wait for thread termination.
 * @param thread The target thread wait for termination.
 * @param value_ptr The pointer of the target thread return value.
 * @return If the function succeeds, the return value is 0.
 *         Otherwise an error number will be returned to indicate the error.
 * @bug The main thread do not support join.
 */
int pthread_join(pthread_t thread, void **value_ptr)
{
    DWORD dwFlags;
    arch_thread_info *pv = (arch_thread_info *) thread;

    if (pv == NULL || pv->handle == NULL || !GetHandleInformation(pv->handle, &dwFlags))
        return ESRCH;

    if ((pv->state & PTHREAD_CREATE_DETACHED) != 0)
        return EINVAL;

    if (pthread_equal(pthread_self(), thread))
        return EDEADLK;

    WaitForSingleObject(pv->handle, INFINITE);
    CloseHandle(pv->handle);

    if (value_ptr)
        *value_ptr = pv->return_value;

    return 0;
}

/**
 * Once-only initialization.
 * @param  once_control The control variable which initialized to PTHREAD_ONCE_INIT.
 * @param  init_routine The initialization code which executed at most once.
 * @return Always return 0.
 */
int pthread_once(pthread_once_t *once_control, void (* init_routine)(void))
{
    if (atomic_cmpxchg((long volatile *) once_control, 1, 0) == 0) {
        init_routine();
        *(volatile int *) once_control = 2;
    } else {
        while(*(volatile int *) once_control != 2)
            SwitchToThread();
    }

    return 0;
}
