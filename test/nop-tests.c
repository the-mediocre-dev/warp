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

    START_FUNC_TESTS(vm, "as-drop-last");
    TEST_IN_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-drop-everywhere");
    TEST_IN_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-mid1");
    TEST_IN_I32_OUT_I32(vm, 3, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-mid2");
    TEST_IN_I32_OUT_I32(vm, 3, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-last");
    TEST_IN_I32_OUT_I32(vm, 3, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-everywhere");
    TEST_IN_I32_OUT_I32(vm, 3, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-first");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-mid");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-last");
    TEST_OUT_I32(vm, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-everywhere");
    TEST_OUT_I32(vm, 4);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-first");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-mid");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-everywhere");
    TEST_OUT_I32(vm, 4);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-last");
    TEST_OUT_I32(vm, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-condition");
    TEST_IN_I32(vm, 0);
    TEST_IN_I32(vm, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-then");
    TEST_IN_I32(vm, 0);
    TEST_IN_I32(vm, 4);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-else");
    TEST_IN_I32(vm, 0);
    TEST_IN_I32(vm, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br-last");
    TEST_IN_I32_OUT_I32(vm, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br-everywhere");
    TEST_IN_I32_OUT_I32(vm, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_if-mid");
    TEST_IN_I32_OUT_I32(vm, 5, 5);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_if-last");
    TEST_IN_I32_OUT_I32(vm, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_if-everywhere");
    TEST_IN_I32_OUT_I32(vm, 7, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_table-mid");
    TEST_IN_I32_OUT_I32(vm, 5, 5);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_table-last");
    TEST_IN_I32_OUT_I32(vm, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_table-everywhere");
    TEST_IN_I32_OUT_I32(vm, 7, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-return-last");
    TEST_IN_I32_OUT_I32(vm, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-return-everywhere");
    TEST_IN_I32_OUT_I32(vm, 7, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-mid1");
    TEST_IN_I32_I32_I32_OUT_I32(vm, 3, 1, 2, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-mid2");
    TEST_IN_I32_I32_I32_OUT_I32(vm, 0, 3, 1, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-last");
    TEST_IN_I32_I32_I32_OUT_I32(vm, 10, 9, -1, 20);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-everywhere");
    TEST_IN_I32_I32_I32_OUT_I32(vm, 2, 1, 5, -2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-unary-last");
    TEST_IN_I32_OUT_I32(vm, 30, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-unary-everywhere");
    TEST_IN_I32_OUT_I32(vm, 12, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-mid");
    TEST_IN_I32_OUT_I32(vm, 3, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-last");
    TEST_IN_I32_OUT_I32(vm, 3, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-everywhere");
    TEST_IN_I32_OUT_I32(vm, 3, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-test-last");
    TEST_IN_I32_OUT_I32(vm, 0, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-test-everywhere");
    TEST_IN_I32_OUT_I32(vm, 0, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-mid");
    TEST_IN_I32_OUT_I32(vm, 3, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-last");
    TEST_IN_I32_OUT_I32(vm, 3, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-everywhere");
    TEST_IN_I32_OUT_I32(vm, 3, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-grow_memory-last");
    TEST_IN_I32_OUT_I32(vm, 2, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-grow_memory-everywhere");
    TEST_IN_I32_OUT_I32(vm, 12, 3);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "nop.1.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "nop.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "nop.3.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "nop.4.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
}
