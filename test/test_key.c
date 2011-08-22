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

int main(int argc, char *argv[])
{
    int rc;
    char *pc;
    char buf[4] = { 17, 18, 'U', 'p' };
    pthread_key_t key = 0;

    rc = pthread_key_create(&key, NULL);
    assert(rc == 0);
    printf("pthread_key_create passed (%lu)\n", key);

    rc = pthread_setspecific(key, buf);
    assert(rc == 0);
    printf("pthread_setspecific passed\n");

    pc = pthread_getspecific(key);
    assert(pc != NULL);
    for(rc = 0; rc < 4; rc ++)
        assert(pc[rc] == buf[rc]);
    printf("pthread_getspecific passed\n");

    rc = pthread_key_delete(key);
    assert(rc == 0);
    printf("pthread_key_delete passed\n");

    return 0;
}
