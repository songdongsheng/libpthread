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

void test_clock_nanosleep()
{
    int rc;
    struct timespec tp, request = { 0, 125000000 }, remain;

    rc = clock_nanosleep(CLOCK_MONOTONIC, 0, &request, &remain);
    assert(rc == -1 && errno == EINVAL);

    rc = clock_nanosleep(CLOCK_PROCESS_CPUTIME_ID, 0, &request, &remain);
    assert(rc == -1 && errno == EINVAL);

    rc = clock_nanosleep(CLOCK_THREAD_CPUTIME_ID, 0, &request, &remain);
    assert(rc == -1 && errno == EINVAL);

    rc = clock_gettime(CLOCK_REALTIME, &tp);
    assert(rc == 0);
    printf("[%10"PRId64".%09d] clock_gettime (CLOCK_REALTIME)\n", (__int64) tp.tv_sec, (int) tp.tv_nsec);

    rc = clock_nanosleep(CLOCK_REALTIME, 0, &request, &remain);
    assert(rc == 0);

    rc = clock_gettime(CLOCK_REALTIME, &tp);
    assert(rc == 0);
    printf("[%10"PRId64".%09d] clock_gettime (CLOCK_REALTIME)\n", (__int64) tp.tv_sec, (int) tp.tv_nsec);

    request.tv_sec = tp.tv_sec + 1;
    request.tv_nsec = 0;

    rc = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, &remain);
    assert(rc == 0);

    rc = clock_gettime(CLOCK_REALTIME, &tp);
    assert(rc == 0);
    printf("[%10"PRId64".%09d] clock_gettime (CLOCK_REALTIME)\n", (__int64) tp.tv_sec, (int) tp.tv_nsec);
}

int main(int argc, char *argv[])
{
    test_clock_nanosleep();

    return 0;
}
