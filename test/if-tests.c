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
#include "test-builder.h"
#include "test-common.h"

void run_if_tests(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{

    load_mdle(vm, dir, path_buf, path_buf_sz, "if.wasm");

    START_FUNC_TESTS(vm, "empty");
    TEST_IN_I32(vm, 0);
    TEST_IN_I32(vm, 1);
    TEST_IN_I32(vm, 100);
    TEST_IN_I32(vm, -2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "singular");
    TEST_IN_I32_OUT_I32(vm, 0, 8);
    TEST_IN_I32_OUT_I32(vm, 1, 7);
    TEST_IN_I32_OUT_I32(vm, 10, 7);
    TEST_IN_I32_OUT_I32(vm, -10, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "multi");
    TEST_IN_I32_OUT_I32(vm, 0, 9);
    TEST_IN_I32_OUT_I32(vm, 1, 8);
    TEST_IN_I32_OUT_I32(vm, 13, 8);
    TEST_IN_I32_OUT_I32(vm, -5, 8);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 11);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 10);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 10);
    TEST_IN_I32_I32_OUT_I32(vm, 3, 2, 9);
    TEST_IN_I32_I32_OUT_I32(vm, 0, -100, 10);
    TEST_IN_I32_I32_OUT_I32(vm, 10, 10, 9);
    TEST_IN_I32_I32_OUT_I32(vm, 0, -1, 10);
    TEST_IN_I32_I32_OUT_I32(vm, -111, -2, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-unary-operand");
    TEST_IN_I32_OUT_I32(vm, 0, 0);
    TEST_IN_I32_OUT_I32(vm, 1, 0);
    TEST_IN_I32_OUT_I32(vm, -1, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-operand");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 15);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, -12);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, -15);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 12);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-test-operand");
    TEST_IN_I32_OUT_I32(vm, 0, 1);
    TEST_IN_I32_OUT_I32(vm, 1, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-operand");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "break-bare");
    TEST_OUT_I32(vm, 19);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "break-value");
    TEST_IN_I32_OUT_I32(vm, 1, 18);
    TEST_IN_I32_OUT_I32(vm, 0, 21);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "effects");
    TEST_IN_I32_OUT_I32(vm, 1, -14);
    TEST_IN_I32_OUT_I32(vm, 0, -6);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);
}
