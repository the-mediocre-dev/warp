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

#include "if-tests.h"
#include "test-common.h"

void test_if(struct wrp_vm *vm, const char *dir, uint8_t *path_buf, size_t path_buf_sz)
{
    WRP_ASSERT(make_path(dir, "if.wasm", path_buf, path_buf_sz), "failed to make path to 'if.wasm'");

    uint8_t *buf = NULL;
    size_t buf_sz = 0;
    WRP_ASSERT(load_buf(path_buf, &buf, &buf_sz), "failed to load 'if.wasm'");

    struct wrp_wasm_mdle *mdle = wrp_instantiate_mdle(vm, buf, buf_sz);
    WRP_ASSERT(mdle, "failed to instantiate 'if.wasm'");
    WRP_ASSERT(wrp_attach_mdle(vm, mdle), "failed to attach module 'if.wasm'");

    wrp_destroy_mdle(vm, mdle);
}
