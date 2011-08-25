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
 * @file clock.c
 * @brief Implementation Code of POSIX Time Routines
 */

#include <stdio.h>
#include <pthread_clock.h>

#include <winsock2.h>

#include "misc.h"

extern DWORD libpthread_time_increment;
extern __int64 libpthread_hpet_frequency;

/**
 * Get the resolution of the specified clock clock_id and
 * stores it in the struct timespec pointed to by res.
 * @param  clock_id The clock_id argument is the identifier of the particular
 *         clock on which to act. libpthread support the following clocks:
 * <pre>
 *     CLOCK_REALTIME System-wide real-time clock. Setting this clock
 *                 requires appropriate privileges.
 *     CLOCK_MONOTONIC Clock that cannot be set and represents monotonic
 *                 time since some unspecified starting point.
 * </pre>
 * @param  res The pointer to a timespec structure to receive the time
 *         resolution.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int clock_getres(clockid_t clock_id, struct timespec *res)
{
    if (clock_id == CLOCK_REALTIME) {
        res->tv_sec = libpthread_time_increment / POW10_7;
        res->tv_nsec = ((int)(libpthread_time_increment % POW10_7)) * POW10_2;
        return 0;
    } else if (clock_id == CLOCK_MONOTONIC) {
        if (libpthread_hpet_frequency < 1000)
            return set_errno(EINVAL);

        res->tv_sec = 0;
        res->tv_nsec = (int) ((POW10_9 + (libpthread_hpet_frequency >> 1)) / libpthread_hpet_frequency);
        if (res->tv_nsec < 1)
            res->tv_nsec = 1;

        return 0;
    }

    return set_errno(EINVAL);
}

/**
 * Get the time of the specified clock clock_id and stores it in the struct
 * timespec pointed to by tp.
 * @param  clock_id The clock_id argument is the identifier of the particular
 *         clock on which to act. libpthread support the following clocks:
 * <pre>
 *     CLOCK_REALTIME System-wide real-time clock. Setting this clock
 *                 requires appropriate privileges.
 *     CLOCK_MONOTONIC Clock that cannot be set and represents monotonic
 *                 time since some unspecified starting point.
 * </pre>
 * @param  tp The pointer to a timespec structure to receive the time.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    __int64 t;

    if (clock_id == CLOCK_REALTIME) {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        t = (((__int64) ft.dwHighDateTime) << 32 | ft.dwLowDateTime) - DELTA_EPOCH_IN_100NS;
        tp->tv_sec = t / POW10_7;
        tp->tv_nsec = ((int) (t % POW10_7)) * 100;
        return 0;
    } else if (clock_id == CLOCK_MONOTONIC) {
        LARGE_INTEGER performanceCount;

        if (libpthread_hpet_frequency < 1000 || QueryPerformanceCounter(&performanceCount) == 0)
            return set_errno(EINVAL);

        tp->tv_sec = performanceCount.QuadPart / libpthread_hpet_frequency;
        tp->tv_nsec = (int) ((performanceCount.QuadPart % libpthread_hpet_frequency + (libpthread_hpet_frequency >> 1)) * POW10_9 / libpthread_hpet_frequency);
        if (tp->tv_nsec >= POW10_9) {
            tp->tv_sec ++;
            tp->tv_nsec -= POW10_9;
        }

        return 0;
    }

    return set_errno(EINVAL);
}

/**
 * Sleep up to 49.7 days.
 * @param  request The requested sleep interval.
 * @param  remain This argument should normally be specified as NULL.
 *         The current implemention just ignore it.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int nanosleep(const struct timespec *request, struct timespec *remain)
{
    __int64 i64;

    if (request->tv_sec < 0 || request->tv_sec > 4294967 || request->tv_nsec < 0 || request->tv_nsec >= POW10_9)
        return set_errno(EINVAL);

    i64 = request->tv_sec * POW10_3 + request->tv_nsec / POW10_6;
    if (i64 < 0 || i64 >= 0xFFFFFFFF)
        return set_errno(EINVAL);

    Sleep((unsigned) i64);

    return 0;
}

/**
 * Sleep up to 49.7 days.
 * @param  clock_id This argument should always be CLOCK_REALTIME (0).
 * @param  flags 0 for relative sleep interval, others for absolute waking up.
 * @param  request The requested sleep interval or absolute waking up time.
 * @param  remain This argument should normally be specified as NULL.
 *         The current implemention just ignore it.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int clock_nanosleep(clockid_t clock_id, int flags,
                           const struct timespec *request,
                           struct timespec *remain)
{
    if (clock_id != CLOCK_REALTIME)
        return set_errno(EINVAL);

    if (flags == 0)
        return nanosleep(request, NULL);

    /* TIMER_ABSTIME = 1 */
    Sleep(arch_rel_time_in_ms(request));

    return 0;
}

/**
 * Set the time of the specified clock clock_id.
 * @param  clock_id This argument should always be CLOCK_REALTIME (0).
 * @param  tp The requested time.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int clock_settime(clockid_t clock_id, const struct timespec *tp)
{
    unsigned __int64 t64;
    FILETIME ft;
    SYSTEMTIME st;

    if (clock_id != CLOCK_REALTIME)
        return set_errno(EINVAL);

    t64 = tp->tv_sec * POW10_7 + tp->tv_nsec / 100 + DELTA_EPOCH_IN_100NS;
    ft.dwLowDateTime = (DWORD) t64;
    ft.dwHighDateTime = (DWORD) (t64 >> 32);

    if (FileTimeToSystemTime(&ft, &st) == 0)
        return set_errno(EINVAL);

    if (SetSystemTime(&st) == 0)
        return set_errno(EPERM);

    return 0;
}
