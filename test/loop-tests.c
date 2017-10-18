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

#include "loop-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_loop_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "loop.0.wasm");

    START_FUNC_TESTS(vm, "empty");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "singular");
    TEST_OUT_I32(vm, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "multi");
    TEST_OUT_I32(vm, 8);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "deep");
    TEST_OUT_I32(vm, 150);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-unary-operand");
    TEST_OUT_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-operand");
    TEST_OUT_I32(vm, 12);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-test-operand");
    TEST_OUT_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-operand");
    TEST_OUT_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "break-bare");
    TEST_OUT_I32(vm, 19);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "break-value");
    TEST_OUT_I32(vm, 18);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "break-repeated");
    TEST_OUT_I32(vm, 18);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "break-inner");
    TEST_OUT_I32(vm, 0x1f);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "effects");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "while");
    TEST_IN_I64_OUT_I64(vm, 0, 1);
    TEST_IN_I64_OUT_I64(vm, 1, 1);
    TEST_IN_I64_OUT_I64(vm, 2, 2);
    TEST_IN_I64_OUT_I64(vm, 3, 6);
    TEST_IN_I64_OUT_I64(vm, 5, 120);
    TEST_IN_I64_OUT_I64(vm, 20, 2432902008176640000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "for");
    TEST_IN_I64_OUT_I64(vm, 0, 1);
    TEST_IN_I64_OUT_I64(vm, 1, 1);
    TEST_IN_I64_OUT_I64(vm, 2, 2);
    TEST_IN_I64_OUT_I64(vm, 3, 6);
    TEST_IN_I64_OUT_I64(vm, 5, 120);
    TEST_IN_I64_OUT_I64(vm, 20, 2432902008176640000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nesting");
    TEST_IN_F32_F32_OUT_F32(vm, 0, 7, 0);
    TEST_IN_F32_F32_OUT_F32(vm, 7, 0, 0);
    TEST_IN_F32_F32_OUT_F32(vm, 1, 1, 1);
    TEST_IN_F32_F32_OUT_F32(vm, 1, 2, 2);
    TEST_IN_F32_F32_OUT_F32(vm, 1, 3, 4);
    TEST_IN_F32_F32_OUT_F32(vm, 1, 4, 6);
    TEST_IN_F32_F32_OUT_F32(vm, 1, 100, 2550);
    TEST_IN_F32_F32_OUT_F32(vm, 1, 101, 2601);
    TEST_IN_F32_F32_OUT_F32(vm, 2, 1, 1);
    TEST_IN_F32_F32_OUT_F32(vm, 3, 1, 1);
    TEST_IN_F32_F32_OUT_F32(vm, 10, 1, 1);
    TEST_IN_F32_F32_OUT_F32(vm, 2, 2, 3);
    TEST_IN_F32_F32_OUT_F32(vm, 2, 3, 4);
    TEST_IN_F32_F32_OUT_F32(vm, 7, 4, 10.3095235825f);
    TEST_IN_F32_F32_OUT_F32(vm, 7, 100, 4381.54785156f);
    TEST_IN_F32_F32_OUT_F32(vm, 7, 101, 2601);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.1.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.3.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.4.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.5.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.6.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.7.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.8.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "loop.9.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
}
