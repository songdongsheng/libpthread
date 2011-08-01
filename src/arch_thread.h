#ifndef _ARCH_THREAD_H_
#define _ARCH_THREAD_H_ 1

#include <winsock2.h>

struct arch_thread_cleanup_node {
    void (* cleaner)(void *);
    void *arg;
    struct arch_thread_cleanup_node *next, *prev;
};

typedef struct arch_thread_cleanup_node arch_thread_cleanup_list;

typedef struct {
    HANDLE handle;
    void *(* worker)(void *);
    void *arg;
    void *return_value;
    volatile unsigned int state;
    arch_thread_cleanup_list *cleanup_list;
} arch_thread_info;

#endif
