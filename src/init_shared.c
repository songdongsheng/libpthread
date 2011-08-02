#include <winsock2.h>

HANDLE libpthread_heap;
DWORD libpthread_tls_index;
DWORD libpthread_time_increment;
__int64 libpthread_hpet_frequency;

static BOOL libpthread_fini(void) {
    TlsFree(libpthread_tls_index);
    return TRUE;
}

static BOOL libpthread_init(void) {
    DWORD   timeAdjustment, timeIncrement;
    BOOL    isTimeAdjustmentDisabled;

    LARGE_INTEGER pf;

    if ((libpthread_heap = GetProcessHeap()) == NULL)
        return FALSE;

    if ((libpthread_tls_index = TlsAlloc()) == TLS_OUT_OF_INDEXES)
        return FALSE;

    if (GetSystemTimeAdjustment(&timeAdjustment, &timeIncrement, &isTimeAdjustmentDisabled) == 0)
        return FALSE;

    libpthread_time_increment = timeIncrement;

    if (QueryPerformanceFrequency(&pf) != 0) {
        libpthread_hpet_frequency = pf.QuadPart;
    } else {
        libpthread_hpet_frequency = 0;
    }

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
