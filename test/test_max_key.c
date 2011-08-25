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

#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

/* 32-bit (Windows XP, 2008 R2) test_max_key passed (1086) */
/* 64-bit (Windows 2008 R2)     test_max_key passed (1085) */
int main(int argc, char *argv[])
{
    int count = 0;
    DWORD key;

    while(1) {
        if ((key = TlsAlloc()) == TLS_OUT_OF_INDEXES)
            break;
        count++;
    }

    printf("test_max_key passed (%d)\n", count);

    return 0;
}
