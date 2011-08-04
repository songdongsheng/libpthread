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

#include <pthread.h>
#include <stdio.h>

#include <winsock2.h>

#include "arch_thread.h"
#include "misc.h"

/*
 * thread-specific data = TSD
 * thread local storage = TLS
 */
int pthread_key_create(pthread_key_t *key, void (* dest)(void *))
{
    return set_errno(ENOTSUP);
}

int pthread_key_delete(pthread_key_t key)
{
    return set_errno(ENOTSUP);
}

void *pthread_getspecific(pthread_key_t key)
{
    return NULL;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    return set_errno(ENOTSUP);
}
