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
