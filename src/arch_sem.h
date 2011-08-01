#ifndef _ARCH_SEM_H_
#define _ARCH_SEM_H_    1

#include <winsock2.h>

typedef struct
{
    HANDLE handle;
} arch_sem_t;

#endif
