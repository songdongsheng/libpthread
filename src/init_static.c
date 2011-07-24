#include <winsock2.h>

/*
 * It's complex and expensive in static libray automatically initialized.
 * Therefore, I do not intend to support static library in the near future.
 */
DWORD libpthread_tls_index;

void __cdecl libpthread_fini(void) {
    TlsFree(libpthread_tls_index);
}

#if defined(__GNUC__)
__attribute__((constructor))
#endif
int __cdecl libpthread_init(void) {
    if ((libpthread_tls_index = TlsAlloc()) == TLS_OUT_OF_INDEXES)
        abort();

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
