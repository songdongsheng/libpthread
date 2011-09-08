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

char *os_time(FILETIME *ft, char *buf)
{
    union {
        FILETIME ns100;
        __int64 QuadPart;
    } t64;

    if(ft == NULL) {
        FILETIME tft;
        GetSystemTimeAsFileTime(&tft);
        t64.ns100.dwLowDateTime = tft.dwLowDateTime;
        t64.ns100.dwHighDateTime = tft.dwHighDateTime;
    } else {
        t64.ns100.dwLowDateTime = ft->dwLowDateTime;
        t64.ns100.dwHighDateTime = ft->dwHighDateTime;
    }

    sprintf(buf, "%11I64d.%07d", t64.QuadPart / 10000000, (int)(t64.QuadPart % 10000000));
    return buf;
}

int main()
{
    int i;
    char buffer[64];
    union {
        FILETIME ns100;
        __int64 QuadPart;
    } ft, ft2;

    printf("[%s] enter main\n", os_time(NULL, buffer));

    for(i = 0; i < 25; i++) {
        GetSystemTimeAsFileTime(&ft.ns100);
        do {
            GetSystemTimeAsFileTime(&ft2.ns100);
        } while(ft.QuadPart == ft2.QuadPart);
        printf("[%s] diff: 0.%07d\n", os_time(NULL, buffer), (int) (ft2.QuadPart - ft.QuadPart));
    }

    printf("[%s] leave main\n", os_time(NULL, buffer));

    return 0;
}
