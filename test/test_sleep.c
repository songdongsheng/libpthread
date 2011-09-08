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

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

void do_sleep_by_sleep_ex(long ms)
{
    SleepEx(ms, TRUE);
}

void do_sleep_by_timer(long ms)
{
    DWORD rc;
    HANDLE timer;
    LARGE_INTEGER li;

    timer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (timer == NULL) {
        fprintf(stderr, "CreateWaitableTimer failed: %ld\n", GetLastError());
        exit(1);
    }

    li.QuadPart = -ms * 10000;
    if (SetWaitableTimer(timer, &li, 0, NULL, NULL, TRUE) == 0) {
        fprintf(stderr, "SetWaitableTimer failed: %ld\n", GetLastError());
        exit(1);
    }

    rc = WaitForSingleObject(timer, INFINITE);
    if (rc != WAIT_OBJECT_0) {
        fprintf(stderr, "WaitForSingleObject failed(%ld): %ld\n", rc, GetLastError());
        exit(1);
    }

    CloseHandle(timer);
}

static __int64 hpet_hz = 0;
char *hpet_time(__int64 hpet_pc, char *buf)
{
    if (hpet_hz == 0) {
        LARGE_INTEGER li;
        if (QueryPerformanceFrequency(&li) == 0) {
            fprintf(stderr, "QueryPerformanceFrequency failed: %ld\n", GetLastError());
            exit(1);
        }
        fprintf(stdout, "HPET frequency: %I64d\n\n", li.QuadPart);
        hpet_hz = li.QuadPart;
    }

    if (hpet_pc == 0) {
        LARGE_INTEGER li;
        if (QueryPerformanceCounter(&li) == 0) {
            fprintf(stderr, "QueryPerformanceCounter failed: %ld\n", GetLastError());
            exit(1);
        }
        hpet_pc = li.QuadPart;
    }

    sprintf(buf, "%I64d.%09d", hpet_pc / hpet_hz, (int)
        ((hpet_pc % hpet_hz) / (double) hpet_hz * 1000000000));
    return buf;
}

void do_test(char *title, void (*fp)(long))
{
    int i;
    double real;
    char buffer[64];
    LARGE_INTEGER li, li2;

    printf("[%s] enter %s test\n", hpet_time(0, buffer), title);

    for(i = 1; i <= 36; i++) {
        li.QuadPart = 0;
        li2.QuadPart = 0;
        QueryPerformanceCounter(&li);
        fp(i);
        QueryPerformanceCounter(&li2);
        real = (li2.QuadPart - li.QuadPart) / (double) hpet_hz * 1000;
        printf("[%s] sleep %2d ms, real %9.6lf ms\n", hpet_time(0, buffer),
                i, real);
    }

    printf("[%s] leave %s test\n", hpet_time(0, buffer), title);
}

int main()
{
    do_test("SleepEx", do_sleep_by_sleep_ex);
    printf("\n");

    do_test("WaitableTimer", do_sleep_by_timer);

    return 0;
}
