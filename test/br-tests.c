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

#include "br-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_br_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "br.0.wasm");

    START_FUNC_TESTS(vm, "type-i32");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-i64");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-f32");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-f64");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-i32-value");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-i64-value");
    TEST_OUT_I64(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-f32-value");
    TEST_OUT_F32(vm, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "type-f64-value");
    TEST_OUT_F64(vm, 4);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-first");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-mid");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-last");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-block-value");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-first");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-mid");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loop-last");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br-value");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_if-cond");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_if-value");
    TEST_OUT_I32(vm, 8);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_if-value-cond");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_table-index");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_table-value");
    TEST_OUT_I32(vm, 10);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-br_table-value-index");
    TEST_OUT_I32(vm, 11);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-return-value");
    TEST_OUT_I64(vm, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-cond");
    TEST_OUT_I32(vm, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-then");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 6, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-then");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-else");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 6, 4);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-if-else");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-first");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 6, 5);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 6, 5);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-second");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 6, 6);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 6, 6);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-select-cond");
    TEST_OUT_I32(vm, 7);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-first");
    TEST_OUT_I32(vm, 12);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-mid");
    TEST_OUT_I32(vm, 13);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call-last");
    TEST_OUT_I32(vm, 14);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call_indirect-func");
    TEST_OUT_I32(vm, 20);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call_indirect-first");
    TEST_OUT_I32(vm, 21);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call_indirect-mid");
    TEST_OUT_I32(vm, 22);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-call_indirect-last");
    TEST_OUT_I32(vm, 23);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-set_local-value");
    TEST_OUT_I32(vm, 17);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-load-address");
    TEST_OUT_F32(vm, 1.7f);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-loadN-address");
    TEST_OUT_I64(vm, 30);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-store-address");
    TEST_OUT_I32(vm, 30);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-store-value");
    TEST_OUT_I32(vm, 31);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-storeN-address");
    TEST_OUT_I32(vm, 32);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-storeN-value");
    TEST_OUT_I32(vm, 33);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-unary-operand");
    TEST_OUT_F32(vm, 3.4f);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-left");
    TEST_OUT_I32(vm, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-binary-right");
    TEST_OUT_I64(vm, 45);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-test-operand");
    TEST_OUT_I32(vm, 44);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-left");
    TEST_OUT_I32(vm, 43);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-compare-right");
    TEST_OUT_I32(vm, 42);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-convert-operand");
    TEST_OUT_I32(vm, 41);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "as-grow_memory-size");
    TEST_OUT_I32(vm, 40);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-block-value");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br-value");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_if-value");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_if-value-cond");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_table-value");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "nested-br_table-value-index");
    TEST_OUT_I32(vm, 9);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.1.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.3.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.4.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.5.wasm", WRP_ERR_INVALID_BLOCK_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.6.wasm", WRP_ERR_INVALID_BLOCK_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "br.7.wasm", WRP_ERR_INVALID_BLOCK_IDX, (*passed), (*failed));
}
