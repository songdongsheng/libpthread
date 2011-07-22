#include <winsock2.h>

typedef struct
{
    HANDLE handle;
    volatile int value;
} arch_sem_t;
