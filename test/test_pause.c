#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_ReadWriteBarrier)
#define __sync_synchronize _ReadWriteBarrier
#endif

int x = 8;

/* mfence */
__declspec(noinline) test_sync(void)
{
    x = 9;
    __sync_synchronize();
    x = (int) time(NULL);
}

int main(int argc, char *argv[])
{
    test_sync();

/* cpu_relax */
#ifdef _MSC_VER
    __asm pause
#else
    asm volatile ( "pause" ::: "memory" );
#endif

    return 0;
}
