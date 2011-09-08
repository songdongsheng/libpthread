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
#include <time.h>

#include <windows.h>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_ReadWriteBarrier)
#define __sync_synchronize _ReadWriteBarrier
#endif

int x = 8;

/* mfence */
__declspec(noinline) void test_sync(void)
{
    x = 9;
    __sync_synchronize();
    x = (int) time(NULL);
}

int main(int argc, char *argv[])
{
    test_sync();

/* cpu_relax() */
#ifdef _MSC_VER
    /* __asm pause */
    /* __asm rep nop */
    /* _mm_pause(); */
    YieldProcessor();
#else
    /* __builtin_ia32_pause(); gcc 4.7 */
    asm volatile("rep; nop" ::: "memory");
#endif

    printf("test_pause passed\n");

    return 0;
}
