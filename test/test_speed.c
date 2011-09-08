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

#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>
#include <pthread.h>
#include <pthread_clock.h>

#include "../src/misc.h"

#define TEST_TIMES  1000000
#define POW10_9     INT64_C(1000000000)

void test_cs()
{
    int i;
    struct timespec tp, tp2;
    CRITICAL_SECTION cs;

    InitializeCriticalSection(&cs);

    EnterCriticalSection(&cs);
    LeaveCriticalSection(&cs);

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = TEST_TIMES * 100; i > 0; i--) {
        EnterCriticalSection(&cs);
        LeaveCriticalSection(&cs);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    DeleteCriticalSection(&cs);

    fprintf(stdout, "EnterCriticalSectio/LeaveCriticalSection: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 100000.0));
}

void test_tid()
{
    int i;
    struct timespec tp, tp2;

    GetCurrentThreadId();

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = TEST_TIMES * 1000; i > 0; i--) {
        GetCurrentThreadId();
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    fprintf(stdout, "                      GetCurrentThreadId: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000000.0));
}

void test_evt()
{
    int i;
    long rc;
    struct timespec tp, tp2;
    HANDLE event = CreateEvent(NULL, FALSE, FALSE, NULL);

    for(i = 0; i < 3; i++) {
        if(SetEvent(event) == 0) {
            fprintf(stderr, "SetEvent failed: %ld\n", GetLastError());
            exit(1);
        }

        rc = WaitForSingleObject(event, INFINITE);
        if(rc != WAIT_OBJECT_0) {
            fprintf(stderr, "WaitForSingleObject(%ld) failed: %ld\n", rc, GetLastError());
            exit(1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        SetEvent(event);
        WaitForSingleObject(event, INFINITE);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    CloseHandle(event);

    fprintf(stdout, "            SetEvent/WaitForSingleObject: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_sem()
{
    int i;
    long rc, pc;
    struct timespec tp, tp2;
    HANDLE handle = CreateSemaphore(NULL, 0, 1, NULL);

    for(i = 0; i < 3; i++) {
        if(ReleaseSemaphore(handle, 1, &pc) == 0 || pc != 0) {
            fprintf(stderr, "ReleaseSemaphore failed: %ld\n", GetLastError());
            exit(1);
        }

        rc = WaitForSingleObject(handle, INFINITE);
        if(rc != WAIT_OBJECT_0) {
            fprintf(stderr, "WaitForSingleObject(%ld) failed: %ld\n", rc, GetLastError());
            exit(1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        ReleaseSemaphore(handle, 1, &pc);
        WaitForSingleObject(handle, INFINITE);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    CloseHandle(handle);

    fprintf(stdout, "    ReleaseSemaphore/WaitForSingleObject: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_lps()
{
    int i;
    struct timespec tp, tp2;
    HANDLE handle = CreateSemaphore(NULL, 0, 1, NULL);

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        get_ncpu();
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    CloseHandle(handle);

    fprintf(stdout, "                                get_ncpu: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_spin()
{
    int i;
    struct timespec tp, tp2;
    pthread_spinlock_t lock = PTHREAD_SPINLOCK_INITIALIZER;

    pthread_spin_lock(&lock);
    pthread_spin_unlock(&lock);

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = TEST_TIMES * 100; i > 0; i--) {
        pthread_spin_lock(&lock);
        pthread_spin_unlock(&lock);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    fprintf(stdout, "   pthread_spin_lock/pthread_spin_unlock: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 100000.0));
}

#ifdef _MSC_VER
void spin_count(int count)
#else
void __attribute__ ((noinline)) spin_count(int count)
#endif
{
    long i = 0, lock = 0;

    while(i++<count) {
        atomic_cmpxchg((volatile long *) &lock, 1, 0);
        cpu_relax();
    }
}

void test_spin_count()
{
    int i, count = 32;
    struct timespec tp, tp2;

    spin_count(count);

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        spin_count(count);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    fprintf(stdout, "                          spin_count(%02d): %7.3lf us\n", count,
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_mutex()
{
    int i;
    long rc;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct timespec tp, tp2;

    rc = pthread_mutex_lock(&mutex);
    if (rc != 0) {
        fprintf(stderr, "pthread_mutex_lock failed: %ld\n", rc);
        exit(1);
    }

    rc = pthread_mutex_unlock(&mutex);
    if (rc != 0) {
        fprintf(stderr, "pthread_mutex_unlock failed: %ld\n", rc);
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = TEST_TIMES * 100; i > 0; i--) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    fprintf(stdout, " pthread_mutex_lock/pthread_mutex_unlock: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 100000.0));
}

#ifndef _MSC_VER
__attribute__ ((noinline))
#endif
__int64 mono_getres(struct timespec *tp)  {
    LARGE_INTEGER pf;

    /* 0.307 us */
    if (QueryPerformanceFrequency(&pf) != 0) {
        return pf.QuadPart;
    }

    /* 0.336 us */
    /*
    if (GetSystemTimeAdjustment(&timeAdjustment, &timeIncrement, &isTimeAdjustmentDisabled) == 0)
        return -1;
*/
    return 0;
}

void test_mono()
{
    int i;
    struct timespec tp, tp2, ti;

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        mono_getres(&ti);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    fprintf(stdout, "               QueryPerformanceFrequency: %7.3lf us\n",
        (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

int main(int argc, char *argv[])
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    test_mono();
    test_mutex();
    test_spin_count();
    test_spin();
    test_lps();
    test_sem();
    test_evt();
    test_tid();
    test_cs();

    return 0;
}
