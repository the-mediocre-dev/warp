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

#include "const-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_const_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.0.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.1.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.2.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.3.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.4.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.5.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.6.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.7.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.8.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.9.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.10.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.11.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.12.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.13.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.14.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.15.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.16.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.17.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.18.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.19.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.20.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.21.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.22.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.23.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.24.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.25.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.26.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "const.27.wasm", WRP_SUCCESS, (*passed), (*failed));
}
