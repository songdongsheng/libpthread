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

#if !defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0600
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>

int main(int argc, char *argv[])
{
    /* SRWLOCK rw; InitializeSRWLock */
    /* CONDITION_VARIABLE cv; InitializeConditionVariable */
    /* CRITICAL_SECTION cs; InitializeCriticalSectionAndSpinCount */

#ifdef _WIN64
    printf("On 64-bit OS:\n");
#else
    printf("On 32-bit OS:\n");
#endif

    printf("sizeof(CRITICAL_SECTION): %u\n", sizeof(CRITICAL_SECTION));
    printf("sizeof(SRWLOCK): %u\n", sizeof(SRWLOCK));
    printf("sizeof(CONDITION_VARIABLE): %u\n", sizeof(CONDITION_VARIABLE));

    return 0;
}
