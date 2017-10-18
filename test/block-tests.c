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

#include "block-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_block_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "block.0.wasm");
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
    TEST_OUT_I32(vm, 0xf);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "effects");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.1.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.2.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.3.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.4.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.5.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.6.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.7.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.8.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.9.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.10.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.11.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.12.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.13.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.14.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.15.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.16.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.17.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.18.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.19.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.20.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.21.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "block.22.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
}
