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

#ifndef _ARCH_THREAD_H_
#define _ARCH_THREAD_H_ 1

/**
 * @file arch.h
 * @brief Implementation-related Definitions
 */

/**
 * @defgroup impl Implementation-related Definitions and Code
 * @ingroup libpthread
 * @{
 */

#include <winsock2.h>
#include <pthread.h>

typedef struct
{
    HANDLE handle;
} arch_sem_t;

typedef struct
{
    size_t stack_size;
    int detach_state;
    int inherit_sched;
    struct sched_param param;
} arch_attr_t;

struct arch_thread_cleanup_node {
    void (* cleaner)(void *);
    void *arg;
    struct arch_thread_cleanup_node *next, *prev;
};

typedef struct arch_thread_cleanup_node arch_thread_cleanup_list;

typedef struct {
    HANDLE handle;
    void *(* worker)(void *);
    void *arg;
    void *return_value;
    unsigned int state;
    arch_thread_cleanup_list *cleanup_list;
} arch_thread_info;

typedef struct {
    unsigned int state;
    unsigned int stack_size;
    int priority;
} arch_thread_attr;

/* sizeof(CRITICAL_SECTION): 24 */
typedef struct {
    CRITICAL_SECTION mutex;
} arch_thread_mutex;

/* sizeof(SRWLOCK): 4 */
typedef struct {
    char rwlock[8]; /* InitializeSRWLock */
} arch_thread_rwlock;

/* sizeof(CONDITION_VARIABLE): 4 */
typedef struct {
    char cond[8]; /* InitializeConditionVariable */
} arch_thread_cond;

/** @} */

#endif
