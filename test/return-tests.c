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

#include "return-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_return_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    // load_mdle(vm, dir, path_buf, path_buf_sz, "return.0.wasm");
    // unload_mdle(vm);

    // TEST_MODULE(vm, dir, path_buf, path_buf_sz, "return.1.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    // TEST_MODULE(vm, dir, path_buf, path_buf_sz, "return.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    // TEST_MODULE(vm, dir, path_buf, path_buf_sz, "return.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
}
