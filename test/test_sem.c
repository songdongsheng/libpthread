#include <assert.h>
#include <errno.h>
#include <fcntl.h>

#include "semaphore.h"
#include "../src/misc.h"

int main(int argc, char *argv[])
{
    int rc;
    sem_t *sem;
    struct timespec tp;

    rc = sem_init(sem, PTHREAD_PROCESS_PRIVATE, 1);
    assert(rc == 0);

    rc = sem_wait(sem);
    assert(rc == 0);

    rc = sem_trywait(sem);
    assert(rc == ETIMEDOUT);

    arch_time_in_timespec(&tp);

    tp.tv_sec += 1;
    tp.tv_nsec += 15625000;
    if (tp.tv_nsec > 999999999) {
        tp.tv_nsec -= 1000000000;
        tp.tv_sec += 1;
    }
    rc = sem_timedwait(sem, &tp);
    assert(rc == ETIMEDOUT);

    rc = sem_post(sem);
    assert(rc == 0);

    rc = sem_destroy(sem);
    assert(rc == 0);

    sem = sem_open("MySem", 0, 0, 1);
    assert(rc == NULL);

    sem = sem_open("MySem", O_CREAT, 0, 1);
    assert(rc != NULL);

    rc = sem_destroy(sem);
    assert(rc == 0);

    rc = sem_unlink(sem);
    assert(rc == 0);

    return 0;
}
