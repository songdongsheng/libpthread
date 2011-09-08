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

double sub_and_div(const struct timespec *t1, const struct timespec *t2, const struct timespec *r)
{
    __int64 diff = (t2->tv_sec - t1->tv_sec) * POW10_9 + (t2->tv_nsec - t1->tv_nsec);
    return diff / (double) (r->tv_sec * POW10_9 + r->tv_nsec);
}

void test_clock_getres(char *name, int id)
{
    int rc;
    double d;
    struct timespec tp, t1, t2;

    rc = clock_getres(id, &tp);
    assert(rc == 0);
    printf("%s resolution: %d.%09d sec\n", name, (int) tp.tv_sec, (int) tp.tv_nsec);

    rc = clock_gettime(id, &t1);
    assert(rc == 0);
    printf("%s time: %d.%09d sec\n", name, (int) t1.tv_sec, (int) t1.tv_nsec);

    if (id == CLOCK_REALTIME || id == CLOCK_MONOTONIC) {
        struct timespec request = {0, 125000000};
        clock_nanosleep(CLOCK_REALTIME, 0, &request, NULL);
    } else {
        long i;
        for (i = 0; i < 10000000; i++) {
            rand();
        }
    }

    rc = clock_gettime(id, &t2);
    assert(rc == 0);
    printf("%s time: %d.%09d sec\n", name, (int) t2.tv_sec, (int) t2.tv_nsec);

    d = sub_and_div(&t1, &t2, &tp);
    printf("sub_and_div: %7.3lf\n", d);
    printf("\n");
}

int main(int argc, char *argv[])
{
    test_clock_getres("          CLOCK_REALTIME", CLOCK_REALTIME);
    test_clock_getres("         CLOCK_MONOTONIC", CLOCK_MONOTONIC);
    test_clock_getres("CLOCK_PROCESS_CPUTIME_ID", CLOCK_PROCESS_CPUTIME_ID);
    test_clock_getres(" CLOCK_THREAD_CPUTIME_ID", CLOCK_THREAD_CPUTIME_ID);

    return 0;
}
