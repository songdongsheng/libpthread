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

#ifndef _MISC_H_
#define _MISC_H_    1

/**
 * @file misc.h
 * @brief Miscellaneous Definitions and Code
 */

/* Number of 100ns-seconds between the beginning of the Windows epoch
 * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
 */
#define DELTA_EPOCH_IN_SEC      INT64_C(11644473600)
#define DELTA_EPOCH_IN_USEC     INT64_C(11644473600000000)
#define DELTA_EPOCH_IN_100NS    INT64_C(116444736000000000)
#define DELTA_EPOCH_IN_NS       INT64_C(11644473600000000000)

#define POW10_2     INT64_C(100)
#define POW10_3     INT64_C(1000)
#define POW10_4     INT64_C(10000)
#define POW10_6     INT64_C(1000000)
#define POW10_7     INT64_C(10000000)
#define POW10_9     INT64_C(1000000000)

static __inline void _my_assert(char *message, char *file, unsigned int line)
{
    fprintf(stderr, "Assertion failed: %s , file %s, line %u\n", message, file, line);
    exit(1);
}

#define assert(_Expression) (void)( (!!(_Expression)) || (_my_assert(#_Expression, __FILE__, __LINE__), 0) )

static __inline int set_errno(int result)
{
    if (result != 0) {
        errno = result;
        return -1;
    }
    return 0;
}

static __inline __int64 FileTimeToUnixTimeIn100NS(FILETIME *input)
{
    return (((__int64) input->dwHighDateTime) << 32 | input->dwLowDateTime) - DELTA_EPOCH_IN_100NS;
}

/* Return milli-seconds since the Unix epoch (jan. 1, 1970) UTC */
static __inline __int64 arch_time_in_ms(void)
{
    FILETIME time;

    GetSystemTimeAsFileTime(&time);
    return FileTimeToUnixTimeIn100NS(&time) / POW10_4;
}

/* Return micro-seconds since the Unix epoch (jan. 1, 1970) UTC */
static __inline void arch_time_in_timespec(struct timespec *ts)
{
    __int64 t;
    FILETIME time;

    GetSystemTimeAsFileTime(&time);
    t = FileTimeToUnixTimeIn100NS(&time);
    ts->tv_sec = t / POW10_7;
    ts->tv_nsec= ((int) (t % POW10_7)) * POW10_2;
}

static __inline __int64 arch_time_in_ms_from_timespec(const struct timespec *ts)
{
    return ts->tv_sec * POW10_3 + ts->tv_nsec / POW10_6;
}

static __inline unsigned arch_rel_time_in_ms(const struct timespec *ts)
{
    __int64 t1 = arch_time_in_ms_from_timespec(ts);
    __int64 t2 = arch_time_in_ms();
    __int64 t = t1 - t2;

    if (t < 0 || t >= INT64_C(4294967295))
        return 0;

    return (unsigned) t;
}

static __inline void cpu_relax(void)
{
#ifdef _MSC_VER
    __asm rep nop
#else
    asm volatile("rep; nop" ::: "memory");
#endif
}

/*
 * http://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
 * http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
 */
static __inline long atomic_cmpxchg(long volatile *__ptr, long __new, long __old)
{
    long prev;

#ifdef _MSC_VER
__asm {
    push         ecx
    push         edx
    mov          ecx, dword ptr [__ptr]
    mov          edx, dword ptr [__new]
    mov          eax, dword ptr [__old]
    lock cmpxchg dword ptr [ecx], edx
    mov          dword ptr [prev], eax
    pop          edx
    pop          ecx
}
#else
    asm volatile("lock ; cmpxchgl %2, %1" : "=a" (prev), "+m" (*__ptr) : "q" (__new), "0" (__old) : "memory");
#endif

    return prev;
}
#endif
