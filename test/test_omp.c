/*
 * gcc -O2 -fopenmp -o test_omp test_omp.c
 *
 * [pthread-w32, gcc-4.6.2-20110801]
 * Hello World from thread = 0
 * Number of threads = 2
 * Hello World from thread = 1
 *
 * [Linux-2.6.32, gcc-4.4.5]
 * Hello World from thread = 0
 * Number of threads = 8
 * Hello World from thread = 2
 * Hello World from thread = 4
 * Hello World from thread = 5
 * Hello World from thread = 7
 * Hello World from thread = 3
 * Hello World from thread = 6
 * Hello World from thread = 1
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int nthreads, tid;

    /* Fork a team of threads giving them their own copies of variables */
    #pragma omp parallel private(nthreads, tid)
    {
        /* Obtain thread number */
        tid = omp_get_thread_num();
        printf("Hello World from thread = %d\n", tid);

        /* Only master thread does this */
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
    }
    /* All threads join master thread and disband */

    return 0;
}
