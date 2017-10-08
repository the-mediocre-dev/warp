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

void run_if_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{

    load_mdle(vm, dir, path_buf, path_buf_sz, "if.0.wasm");

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

    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.1.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.2.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.3.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.4.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.5.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.6.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.7.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.8.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.9.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.10.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.11.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.12.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.13.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.14.wasm", WRP_ERR_VALUEFUL_IF_WITH_NO_ELSE, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.15.wasm", WRP_ERR_VALUEFUL_IF_WITH_NO_ELSE, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.16.wasm", WRP_ERR_VALUEFUL_IF_WITH_NO_ELSE, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.17.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.18.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.19.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.20.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.21.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.22.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.23.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.24.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.25.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.26.wasm", WRP_ERR_CALL_RESULT_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.27.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.28.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.29.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.30.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.31.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.32.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.33.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_INVALID_MODULE(vm, dir, path_buf, path_buf_sz, "if.34.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
}
