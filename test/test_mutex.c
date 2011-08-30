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
#include <stdlib.h>

#include <winsock2.h>

#include "../src/misc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    int rc;
    pthread_mutex_t mutex;

    /* static initializer test */
    printf("g_mutex: %p, & g_mutex: %p\n", g_mutex, &g_mutex);
    rc = pthread_mutex_lock(&g_mutex);
    printf("g_mutex: %p, & g_mutex: %p\n", g_mutex, &g_mutex);
    assert(rc == 0);
    printf("static pthread_mutex_lock passed\n");

    rc = pthread_mutex_trylock(&g_mutex);
    assert(rc == EBUSY);
    printf("static pthread_mutex_trylock passed\n");

    rc = pthread_mutex_unlock(&g_mutex);
    assert(rc == 0);
    printf("static pthread_mutex_unlock passed\n");

    rc = pthread_mutex_destroy(&g_mutex);
    assert(rc == 0);
    printf("static pthread_mutex_destroy passed\n");

    /* normal initializer test */
    printf("mutex: %p, & mutex: %p\n", mutex, &mutex);
    rc = pthread_mutex_init(&mutex, NULL);
    assert(rc == 0);
    printf("pthread_mutex_init passed\n");

    rc = pthread_mutex_lock(&mutex);
    assert(rc == 0);
    printf("pthread_mutex_lock passed\n");

    rc = pthread_mutex_trylock(&g_mutex);
    assert(rc == EBUSY);
    printf("pthread_mutex_trylock passed\n");

    rc = pthread_mutex_unlock(&mutex);
    assert(rc == 0);
    printf("pthread_mutex_unlock passed\n");

    rc = pthread_mutex_destroy(&mutex);
    assert(rc == 0);
    printf("pthread_mutex_destroy passed\n");

    return 0;
}
