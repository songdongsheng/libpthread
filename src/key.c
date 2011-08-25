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

/**
 * @file key.c
 * @brief Implementation Code of TSD/TLS
 */

#include <pthread.h>
#include <stdio.h>

#include <winsock2.h>

#include "arch.h"
#include "misc.h"

/*
 * thread specific data = TSD
 * thread local storage = TLS
 */

extern HANDLE libpthread_heap;

int pthread_key_create(pthread_key_t *key, void (* dest)(void *))
{
    if ((*key = TlsAlloc()) == TLS_OUT_OF_INDEXES)
        return set_errno(EAGAIN);

    return 0;
}

int pthread_key_delete(pthread_key_t key)
{
    if (TlsFree(key) == 0)
        return set_errno(EINVAL);

    return 0;
}

void *pthread_getspecific(pthread_key_t key)
{
    void *rp = TlsGetValue(key);

    if ((rp == NULL) && (GetLastError() != ERROR_SUCCESS))
        set_errno(EINVAL);

    return rp;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    if (TlsSetValue(key, (LPVOID) value) == 0)
        return set_errno(EINVAL);

    return 0;
}
