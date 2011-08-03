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

#include <stdlib.h>
#include <pthread.h>

#include <winsock2.h>

#include "arch_thread.h"
#include "misc.h"

extern DWORD libpthread_tls_index;
extern HANDLE libpthread_heap;

void pthread_testcancel(void)
{
    return;
}

int pthread_setcancelstate(int state, int *oldstate)
{
    return 0;
}

int pthread_setcanceltype(int type, int *oldtype)
{
    return 0;
}

int pthread_cancel(pthread_t thread)
{
    return 0;
}

int pthread_kill(pthread_t thread, int sig)
{
    return pthread_cancel(thread);
}

void pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    arch_thread_cleanup_list *next;
    arch_thread_info *pv = (arch_thread_info *) TlsGetValue(libpthread_tls_index);
    arch_thread_cleanup_list *node = (arch_thread_cleanup_list *) HeapAlloc(libpthread_heap, HEAP_ZERO_MEMORY, sizeof(arch_thread_cleanup_list));

    node->arg = arg;
    node->cleaner = routine;

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

void pthread_cleanup_pop(int execute)
{
    arch_thread_info *pv = (arch_thread_info *) TlsGetValue(libpthread_tls_index);
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

unsigned int __stdcall worker_proxy (void *arg)
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
    thread = (pthread_t *)pv;
    return 0;
}

void pthread_exit(void *value_ptr)
{
    arch_thread_info *pv = (arch_thread_info *) TlsGetValue(libpthread_tls_index);
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
}

int pthread_detach (pthread_t t)
{
    DWORD dwFlags;
    arch_thread_info *pv = (arch_thread_info *) t;
    pv->state |= PTHREAD_CREATE_DETACHED;

    if (pv == NULL || pv->handle == NULL || !GetHandleInformation(pv->handle, &dwFlags))
        return ESRCH;

    if (pv->handle != NULL)
        CloseHandle(pv->handle);

    return 0;
}

pthread_t pthread_self(void)
{
    return (pthread_t) TlsGetValue(libpthread_tls_index);
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
    return t1 == t2;
}

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
