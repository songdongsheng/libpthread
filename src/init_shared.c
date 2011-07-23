#include <winsock2.h>

DWORD libpthread_tls_index;

BOOL libpthread_fini(void) {
    TlsFree(libpthread_tls_index);
    return TRUE;
}

BOOL libpthread_init(void) {
    if ((libpthread_tls_index = TlsAlloc()) == TLS_OUT_OF_INDEXES)
        return FALSE;

    return TRUE;
}

BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason) {
    case DLL_PROCESS_ATTACH:
        return libpthread_init();

    case DLL_PROCESS_DETACH:
        return libpthread_fini();
    }

    return TRUE;
}
