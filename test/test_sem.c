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

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "../src/misc.h"

int main(int argc, char *argv[])
{
    int rc;
    sem_t sem;
    struct timespec tp;

    rc = sem_init(&sem, PTHREAD_PROCESS_PRIVATE, 1);
    assert(rc == 0);
    printf("sem_init passed\n");

/*
    rc = sem_wait(sem);
    assert(rc == 0);
    printf("sem_wait passed\n");
*/
    rc = sem_trywait(sem);
    assert(rc == 0);
    printf("sem_trywait passed\n");

    arch_time_in_timespec(&tp);

    tp.tv_sec += 1;
    tp.tv_nsec += 15625000;
    if (tp.tv_nsec >= POW10_9) {
        tp.tv_nsec -= POW10_9;
        tp.tv_sec += 1;
    }
    rc = sem_timedwait(sem, &tp);
    assert(rc == -1);
    assert(errno == ETIMEDOUT);
    printf("sem_timedwait passed\n");

    rc = sem_post(sem);
    assert(rc == 0);
    printf("sem_post passed\n");

    rc = sem_destroy(sem);
    assert(rc == 0);
    printf("sem_destroy passed\n");

    sem = sem_open("MySem", 0, 0, 1);
    assert(sem == NULL);
    printf("sem_open passed\n");

    sem = sem_open("MySem", O_CREAT, 0, 1);
    assert(sem != NULL);
    printf("sem_open with create passed\n");

    rc = sem_close(sem);
    assert(rc == 0);
    printf("sem_close passed\n");

    rc = sem_unlink("MySem");
    assert(rc == 0);
    printf("sem_unlink passed\n");

    return 0;
}
