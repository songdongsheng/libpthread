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

#include <winsock2.h>
#include <pthread.h>

#include "../src/misc.h"

pthread_spinlock_t lock;

int main(int argc, char *argv[])
{
    assert(pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE) == 0);
    printf("pthread_spin_init passed\n");

    assert(pthread_spin_lock(&lock) == 0);
    printf("pthread_spin_lock passed\n");

    assert(pthread_spin_trylock(&lock) == EBUSY);
    printf("pthread_spin_trylock passed\n");

    assert(pthread_spin_unlock(&lock) == 0);
    printf("pthread_spin_unlock passed\n");

    assert(pthread_spin_destroy(&lock) == 0);
    printf("pthread_spin_destroy passed\n");

    return 0;
}
