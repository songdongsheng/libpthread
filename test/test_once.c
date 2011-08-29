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

static pthread_once_t once_control = PTHREAD_ONCE_INIT;

static void init_routine(void)
{
    fprintf(stdout, "running init_routine\n");
}

static void *wroker(void *arg)
{
    pthread_once(&once_control, init_routine);
    return NULL;
}

int main(int argc, char *argv[])
{
    int rc, i = 0;
    void * result;
    pthread_t t[120];

    while (i < sizeof(t) / sizeof(t[0])) {
        rc = pthread_create(&t[i], NULL, wroker, NULL);
        assert(rc == 0);
        i++;
    }

    for(i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
        rc = pthread_join(t[i], &result);
        assert(rc == 0);
        assert(result == NULL);
    }
    printf("pthread_once passed\n");
    return 0;
}
