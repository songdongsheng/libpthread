#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#include "semaphore.h"
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
    assert(rc == -1);
    assert(errno == ETIMEDOUT);
    printf("sem_trywait passed\n");

    arch_time_in_timespec(&tp);

    tp.tv_sec += 1;
    tp.tv_nsec += 15625000;
    if (tp.tv_nsec > 999999999) {
        tp.tv_nsec -= 1000000000;
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

    sem = NULL;
    printf("sem_open passed 222\n");

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
