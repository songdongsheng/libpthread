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

extern DWORD libpthread_tls_index;
extern HANDLE libpthread_heap;

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
 * Add a cleanup function for thread exit.
 *
 * @param  cleanup_routine The cleanup routine to be called.
 * @param  arg The argument of cleanup routine.
 * @bug The main thread do not support cleanup routines.
 */
void pthread_cleanup_push(void (*cleanup_routine)(void *), void *arg)
{
    arch_thread_cleanup_list *next;
    arch_thread_info *pv = (arch_thread_info *) TlsGetValue(libpthread_tls_index);

    if (pv != NULL) {
        arch_thread_cleanup_list *node = (arch_thread_cleanup_list *) HeapAlloc(libpthread_heap, HEAP_ZERO_MEMORY, sizeof(arch_thread_cleanup_list));

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
    arch_thread_info *pv = (arch_thread_info *) TlsGetValue(libpthread_tls_index);
    if (pv != NULL) {
        arch_thread_cleanup_list *node = pv->cleanup_list, *prev;

        if (node == NULL) return;
        while(node->next != NULL)
            node = node->next;

        if (execute) {
            node->cleaner(node->arg);
        }

        prev = node->prev;
        HeapFree(libpthread_heap, 0, node);
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
            HeapFree(libpthread_heap, 0, node);
            node = next;
        } while(node != NULL);
        pv->cleanup_list = NULL;
    }

    /* Make sure we free ourselves if we are detached */
    if ((pv->state & PTHREAD_CREATE_DETACHED) == PTHREAD_CREATE_DETACHED) {
        CloseHandle (pv->handle);
        HeapFree(libpthread_heap, 0, pv);
        TlsSetValue(libpthread_tls_index, NULL);
    }

    return 0;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
    arch_thread_info *pv = (arch_thread_info *) HeapAlloc(libpthread_heap, HEAP_ZERO_MEMORY, sizeof(arch_thread_info));
    if (pv == NULL)
        return set_errno(ENOMEM);

    /* TODO: use attr (StackSize)to create thread */
    pv->arg = arg;
    pv->worker = start_routine;
    pv->state = PTHREAD_CREATE_JOINABLE;
    pv->handle = (HANDLE) _beginthreadex(NULL, 0, worker_proxy, pv, CREATE_SUSPENDED, NULL);

    if (pv->handle == INVALID_HANDLE_VALUE) {
        HeapFree(libpthread_heap, 0, pv);
        return errno;
    }

    /* TODO: SetThreadPriority */

    if ((pv->state & PTHREAD_CREATE_DETACHED) != 0)
      CloseHandle(pv->handle);

    ResumeThread(pv->handle);
    *thread = (pthread_t) pv;
    return 0;
}

/**
 * Terminate calling thread.
 *
 * @param value_ptr The pointer of the calling thread return value.
 */
void pthread_exit(void *value_ptr)
{
    arch_thread_info *pv = (arch_thread_info *) TlsGetValue(libpthread_tls_index);
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
                HeapFree(libpthread_heap, 0, node);
                node = prev;
            } while(node != NULL);
            pv->cleanup_list = NULL;
        }

        /* Make sure we free ourselves if we are detached */
        if ((pv->state & PTHREAD_CREATE_DETACHED) == PTHREAD_CREATE_DETACHED) {
            CloseHandle (pv->handle);
            HeapFree(libpthread_heap, 0, pv);
            TlsSetValue(libpthread_tls_index, NULL);
        }

        _endthreadex(0);
    } else {
        exit(1); /* User should not call pthread_exit in the main thread */
    }
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
 *
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
 *
 * @return If the thread IDs t1 and t2 correspond to the same thread,
 * the return value is non-zero, otherwise it will return zero.
 */
int pthread_equal(pthread_t t1, pthread_t t2)
{
    return t1 == t2;
}

/**
 * Wait for thread termination.
 *
 * @param thread The target thread wait for termination.
 * @return If the function succeeds, the return value is 0.
 * Otherwise an error number will be returned to indicate the error.
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
 *
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
