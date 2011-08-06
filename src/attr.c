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

#include "arch.h"
#include "misc.h"

int pthread_attr_init(pthread_attr_t *attr)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *size)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t size)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_setdetachstate(pthread_attr_t *a, int flag)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_getdetachstate(const pthread_attr_t *a, int *flag)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_setinheritsched(pthread_attr_t *a, int flag)
{
    return set_errno(ENOTSUP);
}

int pthread_attr_getinheritsched(const pthread_attr_t *a, int *flag)
{
    return set_errno(ENOTSUP);
}
