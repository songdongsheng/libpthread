#include <stdio.h>
#include <stdlib.h>

void __cdecl libpthread_fini(void) {
    printf("libpthread_fini\n");
}

#if defined(__GNUC__)
__attribute__((constructor))
#endif
int __cdecl libpthread_init(void) {
    printf("libpthread_init\n");
    atexit(libpthread_fini);
    return 0;
}

#if defined(_MSC_VER)
/*
 * CRT Initialization
 * http://msdn.microsoft.com/en-us/library/bb918180.aspx
 */
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void *libpthread_init_fp = libpthread_init;
#endif

int main()
{
    printf("enter main\n");
    printf("leave main\n");
    return 0;
}
