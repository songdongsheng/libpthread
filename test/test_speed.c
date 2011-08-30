
#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>
#include <pthread.h>
#include <pthread_clock.h>

#define TEST_TIMES  1000000
#define POW10_9     INT64_C(1000000000)

void test_cs()
{
    int i;
    struct timespec tp, tp2;
    CRITICAL_SECTION cs;

    InitializeCriticalSection(&cs);

    EnterCriticalSection(&cs);
    LeaveCriticalSection(&cs);

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        EnterCriticalSection(&cs);
        LeaveCriticalSection(&cs);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    DeleteCriticalSection(&cs);

    fprintf(stdout, "EnterCriticalSectio/LeaveCriticalSection: %7.3lf us\n", (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_tid()
{
    int i;
    struct timespec tp, tp2;

    GetCurrentThreadId();

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        GetCurrentThreadId();
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    fprintf(stdout, "                      GetCurrentThreadId: %7.3lf us\n", (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_evt()
{
    int i;
    long rc;
    struct timespec tp, tp2;
    HANDLE event = CreateEvent(NULL, FALSE, FALSE, NULL);

    for(i = 0; i < 3; i++) {
        if(SetEvent(event) == 0) {
            fprintf(stderr, "SetEvent failed: %ld\n", GetLastError());
            exit(1);
        }

        rc = WaitForSingleObject(event, INFINITE);
        if(rc != WAIT_OBJECT_0) {
            fprintf(stderr, "WaitForSingleObject(%ld) failed: %ld\n", rc, GetLastError());
            exit(1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        SetEvent(event);
        WaitForSingleObject(event, INFINITE);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    CloseHandle(event);

    fprintf(stdout, "            SetEvent/WaitForSingleObject: %7.3lf us\n", (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

void test_sem()
{
    int i;
    long rc, pc;
    struct timespec tp, tp2;
    HANDLE handle = CreateSemaphore(NULL, 0, 1, NULL);

    for(i = 0; i < 3; i++) {
        if(ReleaseSemaphore(handle, 1, &pc) == 0 || pc != 0) {
            fprintf(stderr, "ReleaseSemaphore failed: %ld\n", GetLastError());
            exit(1);
        }

        rc = WaitForSingleObject(handle, INFINITE);
        if(rc != WAIT_OBJECT_0) {
            fprintf(stderr, "WaitForSingleObject(%ld) failed: %ld\n", rc, GetLastError());
            exit(1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &tp);
    for(i = 0; i < TEST_TIMES; i++) {
        ReleaseSemaphore(handle, 1, &pc);
        WaitForSingleObject(handle, INFINITE);
    }
    clock_gettime(CLOCK_MONOTONIC, &tp2);

    CloseHandle(handle);

    fprintf(stdout, "    ReleaseSemaphore/WaitForSingleObject: %7.3lf us\n", (tp2.tv_nsec - tp.tv_nsec + (tp2.tv_sec - tp.tv_sec) * POW10_9) / (TEST_TIMES * 1000.0));
}

int main(int argc, char *argv[])
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    test_sem();
    test_evt();
    test_tid();
    test_cs();

    return 0;
}
