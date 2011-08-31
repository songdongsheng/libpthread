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
    int i, rc;
    long __new = 1, __old = 2;
    struct timespec tp;

    rc = clock_getres(CLOCK_MONOTONIC, &tp);
    assert(rc == 0);
    printf("   CLOCK_MONOTONIC resolution: %d.%09d sec\n", (int) tp.tv_sec, (int) tp.tv_nsec);

    rc = clock_getres(CLOCK_REALTIME, &tp);
    assert(rc == 0);
    printf("    CLOCK_REALTIME resolution: %d.%09d sec\n", (int) tp.tv_sec, (int) tp.tv_nsec);

    rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
    assert(rc == 0);
    printf("CLOCK_PROCESS_CPUTIME_ID time: %d.%09d sec\n", (int) tp.tv_sec, (int) tp.tv_nsec);

    rc = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp);
    assert(rc == 0);
    printf(" CLOCK_THREAD_CPUTIME_ID time: %d.%09d sec\n", (int) tp.tv_sec, (int) tp.tv_nsec);

    for (i = 0; i < 100000000; i++)
        atomic_cmpxchg(&__new, __old, __old);

    rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
    assert(rc == 0);
    printf("CLOCK_PROCESS_CPUTIME_ID time: %d.%09d sec\n", (int) tp.tv_sec, (int) tp.tv_nsec);

    rc = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp);
    assert(rc == 0);
    printf(" CLOCK_THREAD_CPUTIME_ID time: %d.%09d sec\n", (int) tp.tv_sec, (int) tp.tv_nsec);

    return 0;
}
