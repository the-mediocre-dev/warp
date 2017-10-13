/*
 *  Copyright 2017 Adam Dicker
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "memory-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_memory_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.0.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.1.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.2.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.3.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.4.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.5.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.6.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.7.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.8.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.9.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.10.wasm", WRP_SUCCESS, (*passed), (*failed));
}
