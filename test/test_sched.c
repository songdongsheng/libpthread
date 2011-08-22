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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>

#include "../src/misc.h"

int main(int argc, char *argv[])
{
    int rc, priority = SCHED_OTHER;
    struct timespec tp;
    struct sched_param sp;
    pid_t pid = 0;

    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = 15;

    rc = sched_yield();
    assert(rc == 0);
    printf("sched_yield passed\n");

    rc = sched_getscheduler(pid);
    assert(rc == SCHED_OTHER);
    printf("sched_getscheduler passed\n");

    rc = sched_setscheduler(pid, priority, &sp);
    assert(rc == 0);
    printf("sched_setscheduler passed\n");

    rc = sched_get_priority_min(pid);
    assert(rc == 1);
    printf("sched_get_priority_min passed\n");

    rc = sched_get_priority_max(pid);
    assert(rc == 15);
    printf("sched_get_priority_max passed\n");

    rc = sched_setparam(pid, &sp);
    assert(rc == 0);
    printf("sched_setparam passed\n");

    rc = sched_getparam(pid, &sp);
    assert(rc == 0);
    assert(sp.sched_priority == 15);
    printf("sched_getparam passed\n");

    rc = sched_rr_get_interval(pid, &tp);
    assert(rc == 0);
    assert(tp.tv_sec == 0);
    assert(tp.tv_nsec == 15625000);
    printf("sched_rr_get_interval passed\n");

    return 0;
}
