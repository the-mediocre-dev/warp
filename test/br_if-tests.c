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

#include "br_if-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_br_if_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "br_if.0.wasm");

    START_FUNC_TESTS(vm, "as-block-first");
    TEST_IN_I32_OUT_I32(vm, 0, 2);
    TEST_IN_I32_OUT_I32(vm, 1, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-mid");
    TEST_IN_I32_OUT_I32(vm, 0, 2);
    TEST_IN_I32_OUT_I32(vm, 1, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-last");
    TEST_IN_I32(vm, 0);
    TEST_IN_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-last-value");
    TEST_IN_I32_OUT_I32(vm, 0, 11);
    TEST_IN_I32_OUT_I32(vm, 1, 11);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-first");
    TEST_IN_I32_OUT_I32(vm, 0, 2);
    TEST_IN_I32_OUT_I32(vm, 1, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-mid");
    TEST_IN_I32_OUT_I32(vm, 0, 2);
    TEST_IN_I32_OUT_I32(vm, 1, 4);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-last");
    TEST_IN_I32(vm, 0);
    TEST_IN_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-then");
    TEST_IN_I32_I32(vm, 0, 0);
    TEST_IN_I32_I32(vm, 4, 0);
    TEST_IN_I32_I32(vm, 0, 1);
    TEST_IN_I32_I32(vm, 4, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-else");
    TEST_IN_I32_I32(vm, 0, 0);
    TEST_IN_I32_I32(vm, 3, 0);
    TEST_IN_I32_I32(vm, 0, 1);
    TEST_IN_I32_I32(vm, 3, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-block-value");
    TEST_IN_I32_OUT_I32(vm, 0, 21);
    TEST_IN_I32_OUT_I32(vm, 1, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br-value");
    TEST_IN_I32_OUT_I32(vm, 0, 5);
    TEST_IN_I32_OUT_I32(vm, 1, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_if-value");
    TEST_IN_I32_OUT_I32(vm, 0, 5);
    TEST_IN_I32_OUT_I32(vm, 1, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_if-value-cond");
    TEST_IN_I32_OUT_I32(vm, 0, 5);
    TEST_IN_I32_OUT_I32(vm, 1, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_table-value");
    TEST_IN_I32_OUT_I32(vm, 0, 5);
    TEST_IN_I32_OUT_I32(vm, 1, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_table-value-index");
    TEST_IN_I32_OUT_I32(vm, 0, 5);
    TEST_IN_I32_OUT_I32(vm, 1, 9);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.1.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.3.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.4.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.5.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.6.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.7.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.8.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.9.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.10.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.11.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.12.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.13.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.14.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.15.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.16.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.17.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.18.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.19.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.20.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.21.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.22.wasm", WRP_ERR_INVALID_BLOCK_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.23.wasm", WRP_ERR_INVALID_BLOCK_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br_if.24.wasm", WRP_ERR_INVALID_BLOCK_IDX, (*passed), (*failed));
}
