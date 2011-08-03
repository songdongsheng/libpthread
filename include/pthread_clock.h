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

#ifndef _PTHREAD_CLOCK_H_
#define _PTHREAD_CLOCK_H_   1

#include <pthread_types.h>

#ifdef __cplusplus
extern "C" {
#endif

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
int clock_getres(clockid_t clock_id, struct timespec *res);

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
int clock_gettime(clockid_t clock_id, struct timespec *tp);

/**
 * Sleep up to 49.7 days.
 * @param  request The requested sleep interval.
 * @param  remain This argument should normally be specified as NULL.
 *         The current implemention just ignore it.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int nanosleep(const struct timespec *request, struct timespec *remain);

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
                           struct timespec *remain);

/**
 * Set the time of the specified clock clock_id.
 * @param  clock_id This argument should always be CLOCK_REALTIME (0).
 * @param  tp The requested time.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int clock_settime(clockid_t clock_id, const struct timespec *tp);

#ifdef __cplusplus
}
#endif

#endif /* _PTHREAD_CLOCK_H_ */
