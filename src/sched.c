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
 * @file sched.c
 * @brief Implementation Code of Scheduling Routines
 */

#include <sched.h>
#include <stdio.h>

#include <winsock2.h>

#include "misc.h"

/*
 *  Windows Scheduling Priorities
 *  http://msdn.microsoft.com/en-us/library/ms685100
 */

/**
 * Yield the processor.
 * @return Always return 0.
 */
int sched_yield(void)
{
    SwitchToThread(); /* Sleep(0); */

    return 0;
}

/**
 * Get the scheduling policy.
 * @param  pid Ignored.
 * @return Always return SCHED_OTHER.
 */
int sched_getscheduler(pid_t pid)
{
    return SCHED_OTHER;
}

/**
 * Set the scheduling parameters.
 * @param  pid The process identifier.
 * @param  policy The scheduling policy.
 * @param  param The scheduling parameters.
 * @return Always return 0.
 *         If pid is 0 and policy is SCHED_OTHER, then call SetThreadPriority with the mapped priority.
 *         Otherwise, do nothing.
 */
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)
{
    if (pid == 0 && policy == SCHED_OTHER && param != NULL) {
        int priority = sched_priority_to_os_priority(param->sched_priority);
        if(!SetThreadPriority(GetCurrentThread(), priority))
            return lc_set_errno(EPERM);
    }

    return 0;
}

/**
 * Get the minimum priority value.
 * @param  policy Ignored.
 * @return Always return 1.
 */
int sched_get_priority_min(int policy)
{
    return 1; /* THREAD_PRIORITY_IDLE */
}

/**
 * Get the maximum priority value.
 * @param  policy Ignored.
 * @return Always return 15.
 */
int sched_get_priority_max(int policy)
{
    return 15; /* THREAD_PRIORITY_TIME_CRITICAL */
}

/**
 * Set scheduling parameters.
 * @param  pid The process identifier.
 * @param  param The scheduling parameters.
 * @return sched_setscheduler(pid, SCHED_OTHER, param).
 */
int sched_setparam(pid_t pid, const struct sched_param *param)
{
    return sched_setscheduler(pid, SCHED_OTHER, param);
}

/**
 * Get scheduling parameters.
 * @param  pid The process identifier.
 * @param  param The scheduling parameters.
 * @return Always return 0.
 *         If pid is 0, and GetThreadPriority succeeds, param->sched_priority is the mapped priority.
 *         Otherwise, param->sched_priority is always 8.
 */
int sched_getparam(pid_t pid, struct sched_param *param)
{
    param->sched_priority = 8; /* THREAD_PRIORITY_NORMAL */
    if (pid == 0)
        param->sched_priority = os_priority_to_sched_priority(GetThreadPriority(GetCurrentThread()));

    return 0;
}

/**
 * Get the SCHED_RR interval.
 * @param  pid The process identifier.
 * @param  tp The SCHED_RR interval.
 * @return Always return 0.
 */
int sched_rr_get_interval(pid_t pid, struct timespec * tp)
{
    DWORD   timeAdjustment, timeIncrement;
    BOOL    isTimeAdjustmentDisabled;

    (void) GetSystemTimeAdjustment(&timeAdjustment, &timeIncrement, &isTimeAdjustmentDisabled);
    tp->tv_sec = 0;
    tp->tv_nsec = timeIncrement * 100;

    return 0;
}
