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

#include <pthread_clock.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "../src/misc.h"

int main(int argc, char *argv[])
{
    int rc;
    struct timespec r1, r2, m1, m2, request, now;

    rc = clock_gettime(CLOCK_REALTIME, &r1);
    assert(rc == 0);
    printf("[%"PRId64".%09d] clock_gettime (CLOCK_REALTIME) passed\n", (__int64) r1.tv_sec, (int) r1.tv_nsec);

    rc = clock_gettime(CLOCK_MONOTONIC, &m1);
    assert(rc == 0);
    printf("[%"PRId64".%09d] clock_gettime (CLOCK_MONOTONIC) passed\n", (__int64) m1.tv_sec, (int) m1.tv_nsec);

    request.tv_sec = 1;
    request.tv_nsec = 0;
    rc = nanosleep(&request, NULL);
    assert(rc == 0);
    clock_gettime(CLOCK_REALTIME, &now);
    printf("[%"PRId64".%09d] nanosleep passed\n", (__int64) now.tv_sec, (int) now.tv_nsec);

    rc = clock_gettime(CLOCK_REALTIME, &r2);
    assert(rc == 0);
    printf("[%"PRId64".%09d] clock_gettime (CLOCK_REALTIME) passed 2\n", (__int64) r2.tv_sec, (int) r2.tv_nsec);

    rc = clock_gettime(CLOCK_MONOTONIC, &m2);
    assert(rc == 0);
    printf("[%"PRId64".%09d] clock_gettime (CLOCK_MONOTONIC) passed 2\n", (__int64) m2.tv_sec, (int) m2.tv_nsec);

    request.tv_sec = 1;
    request.tv_nsec = 0;
    rc = clock_nanosleep(CLOCK_REALTIME, 0, &request, NULL);
    assert(rc == 0);
    clock_gettime(CLOCK_REALTIME, &now);
    printf("[%"PRId64".%09d] clock_nanosleep passed\n", (__int64) now.tv_sec, (int) now.tv_nsec);

    request.tv_sec = 3 + now.tv_sec;
    request.tv_nsec = 0;
    rc = clock_nanosleep(CLOCK_REALTIME, 1, &request, NULL);
    assert(rc == 0);
    clock_gettime(CLOCK_REALTIME, &now);
    printf("[%"PRId64".%09d] clock_nanosleep passed 2\n", (__int64) now.tv_sec, (int) now.tv_nsec);

    return 0;
}
