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

#ifndef _SCHED_H_
#define _SCHED_H_       1

/**
 * @file sched.h
 * @brief POSIX Process Scheduling Definitions and Routines
 */

/**
 * @defgroup sched POSIX Process Scheduling Definitions and Routines
 * @{
 */

#include <pthread_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Priority scheduling is supported.  */
#ifndef _POSIX_PRIORITY_SCHEDULING
#define _POSIX_PRIORITY_SCHEDULING              200809L
#endif

#define SCHED_OTHER     0 /* NORMAL_PRIORITY_CLASS */
#define SCHED_FIFO      1
#define SCHED_RR        2
#define SCHED_MIN       SCHED_OTHER
#define SCHED_MAX       SCHED_RR

struct sched_param {
  int sched_priority;
};

int sched_yield(void);
int sched_rr_get_interval(pid_t pid, struct timespec * tp);
int sched_get_priority_min(int pol);
int sched_get_priority_max(int pol);
int sched_getscheduler(pid_t pid);
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
int sched_getparam(pid_t pid, struct sched_param *param);
int sched_setparam(pid_t pid, const struct sched_param *param);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* _SCHED_H_ */
