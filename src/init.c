/*
 * Copyright (c) 2011, Dongsheng Song <songdongsheng@live.cn>
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file init.c
 * @brief Initialization Code of Libpthread
 */

#include <winsock2.h>

DWORD libpthread_tls_index;

static BOOL libpthread_fini(void) {
    TlsFree(libpthread_tls_index);
    return TRUE;
}

static BOOL libpthread_init(void) {
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
