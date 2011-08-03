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

#include <sched.h>

#include <winsock2.h>

/*
 *  Windows Scheduling Priorities
 *  http://msdn.microsoft.com/en-us/library/ms685100
 */
int sched_yield(void)
{
    SwitchToThread(); /* Sleep(0); */

    return 0;
}

int sched_getscheduler(pid_t pid)
{
    return SCHED_OTHER;
}

int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)
{
    return 0;
}

int sched_get_priority_min(int policy)
{
    return 1; /* THREAD_PRIORITY_IDLE */
}

int sched_get_priority_max(int policy)
{
    return 15; /* THREAD_PRIORITY_TIME_CRITICAL */
}

int sched_setparam(pid_t pid, const struct sched_param *param)
{
    return 0;
}

int sched_getparam(pid_t pid, struct sched_param *param)
{
    param->sched_priority = 8; /* THREAD_PRIORITY_NORMAL */
    return 0;
}

int sched_rr_get_interval(pid_t pid, struct timespec * tp)
{
    tp->tv_sec = 0;
    tp->tv_nsec = 15625000;
    return 0;
}
