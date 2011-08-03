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
    volatile unsigned int state;
    arch_thread_cleanup_list *cleanup_list;
} arch_thread_info;

#endif
