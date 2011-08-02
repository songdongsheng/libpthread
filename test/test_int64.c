#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    __int64 x = (__int64) time(NULL);

    x *= 1000;

    printf("ms: %I64d\n", x);
    printf("ms: %I64u\n", x);
    printf("ms: %I64x\n", x);
    printf("ms: %I64X\n", x);

#if _MSC_VER >= 1500
    printf("ms: %lld\n", x);
    printf("ms: %llu\n", x);
    printf("ms: %llx\n", x);
    printf("ms: %llX\n", x);
#endif

    return 0;
}
