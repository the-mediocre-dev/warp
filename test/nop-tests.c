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

#include "nop-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_nop_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "nop.0.wasm");

    START_FUNC_TESTS(vm, "as-func-first");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-func-mid");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-func-last");
    TEST_OUT_I32(vm, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-func-everywhere");
    TEST_OUT_I32(vm, 4);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);
}
