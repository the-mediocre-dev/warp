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

#include "memory-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_memory_tests(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.0.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.1.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.2.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.3.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.4.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.5.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.6.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.7.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.8.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.9.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.10.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.11.wasm", WRP_ERR_MDLE_MEMORY_OVERFLOW, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.12.wasm", WRP_ERR_MDLE_MEMORY_OVERFLOW, (*passed), (*failed));

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory.13.wasm");
    START_FUNC_TESTS(vm, "memsize");
    TEST_OUT_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));
    unload_mdle(vm);

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory.14.wasm");
    START_FUNC_TESTS(vm, "memsize");
    TEST_OUT_I32(vm, 0);
    END_FUNC_TESTS((*passed), (*failed));
    unload_mdle(vm);

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory.15.wasm");
    START_FUNC_TESTS(vm, "memsize");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));
    unload_mdle(vm);

    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.16.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.17.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.18.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.19.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.20.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.21.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.22.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.23.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.24.wasm", WRP_ERR_INVALID_MEM_IDX, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.25.wasm", WRP_ERR_TYPE_MISMATCH, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.26.wasm", WRP_ERR_INVALID_INITIALZER_EXPRESSION, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.27.wasm", WRP_ERR_INVALID_INITIALZER_EXPRESSION, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.28.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.29.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.30.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.31.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.32.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.33.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.34.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.35.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_LINK(vm, dir, path_buf, path_buf_sz, "memory.36.wasm", WRP_ERR_INVALID_MEMORY_ACCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.37.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.38.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.39.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.40.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.41.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.42.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.43.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.44.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.45.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.46.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.47.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.48.wasm", WRP_ERR_INVALID_MEM_LIMIT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.49.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.50.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.51.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.52.wasm", WRP_SUCCESS, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.53.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.54.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.55.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.56.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.57.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.58.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.59.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.60.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));
    TEST_MODULE(vm, dir, path_buf, path_buf_sz, "memory.61.wasm", WRP_ERR_INVALID_ALIGNMENT, (*passed), (*failed));

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory.62.wasm");
    START_FUNC_TESTS(vm, "data");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "aligned");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "unaligned");
    TEST_OUT_I32(vm, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "cast");
    TEST_OUT_F64(vm, 42.0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_s");
    TEST_IN_I32_OUT_I32(vm, -1, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_u");
    TEST_IN_I32_OUT_I32(vm, -1, 255);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_s");
    TEST_IN_I32_OUT_I32(vm, -1, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_u");
    TEST_IN_I32_OUT_I32(vm, -1, 65535);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_s");
    TEST_IN_I32_OUT_I32(vm, 100, 100);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_u");
    TEST_IN_I32_OUT_I32(vm, 200, 200);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_s");
    TEST_IN_I32_OUT_I32(vm, 20000, 20000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_u");
    TEST_IN_I32_OUT_I32(vm, 40000, 40000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_s");
    TEST_IN_I32_OUT_I32(vm, 0xfedc6543, 0x43);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_s");
    TEST_IN_I32_OUT_I32(vm, 0x3456cdef, 0xffffffef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_u");
    TEST_IN_I32_OUT_I32(vm, 0xfedc6543, 0x43);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load8_u");
    TEST_IN_I32_OUT_I32(vm, 0x3456cdef, 0xef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_s");
    TEST_IN_I32_OUT_I32(vm, 0xfedc6543, 0x6543);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_s");
    TEST_IN_I32_OUT_I32(vm, 0x3456cdef, 0xffffcdef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_u");
    TEST_IN_I32_OUT_I32(vm, 0xfedc6543, 0x6543);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32_load16_u");
    TEST_IN_I32_OUT_I32(vm, 0x3456cdef, 0xcdef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_s");
    TEST_IN_I64_OUT_I64(vm, -1, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_u");
    TEST_IN_I64_OUT_I64(vm, -1, 255);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_s");
    TEST_IN_I64_OUT_I64(vm, -1, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_u");
    TEST_IN_I64_OUT_I64(vm, -1, 65535);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_s");
    TEST_IN_I64_OUT_I64(vm, -1, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_u");
    TEST_IN_I64_OUT_I64(vm, -1, 4294967295);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_s");
    TEST_IN_I64_OUT_I64(vm, 100, 100);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_u");
    TEST_IN_I64_OUT_I64(vm, 200, 200);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_s");
    TEST_IN_I64_OUT_I64(vm, 20000, 20000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_u");
    TEST_IN_I64_OUT_I64(vm, 40000, 40000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_s");
    TEST_IN_I64_OUT_I64(vm, 20000, 20000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_u");
    TEST_IN_I64_OUT_I64(vm, 40000, 40000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_s");
    TEST_IN_I64_OUT_I64(vm, 0xfedcba9856346543, 0x43);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_s");
    TEST_IN_I64_OUT_I64(vm, 0x3456436598bacdef, 0xffffffffffffffef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_u");
    TEST_IN_I64_OUT_I64(vm, 0xfedcba9856346543, 0x43);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load8_u");
    TEST_IN_I64_OUT_I64(vm, 0x3456436598bacdef, 0xef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_s");
    TEST_IN_I64_OUT_I64(vm, 0xfedcba9856346543, 0x6543);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_s");
    TEST_IN_I64_OUT_I64(vm, 0x3456436598bacdef, 0xffffffffffffcdef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_u");
    TEST_IN_I64_OUT_I64(vm, 0xfedcba9856346543, 0x6543);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load16_u");
    TEST_IN_I64_OUT_I64(vm, 0x3456436598bacdef, 0xcdef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_s");
    TEST_IN_I64_OUT_I64(vm, 0xfedcba9856346543, 0x56346543);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_s");
    TEST_IN_I64_OUT_I64(vm, 0x3456436598bacdef, 0xffffffff98bacdef);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_u");
    TEST_IN_I64_OUT_I64(vm, 0xfedcba9856346543, 0x56346543);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64_load32_u");
    TEST_IN_I64_OUT_I64(vm, 0x3456436598bacdef, 0x98bacdef);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory_redundancy.0.wasm");

    START_FUNC_TESTS(vm, "test_store_to_load");
    TEST_OUT_I32(vm, 0x00000080);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "zero_everything");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "test_redundant_load");
    TEST_OUT_I32(vm, 0x00000080);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "zero_everything");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "test_dead_store");
    TEST_OUT_F32(vm, 0x1.18p-144);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "zero_everything");
    TEST_EMPTY(vm);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "malloc_aliasing");
    TEST_OUT_I32(vm, 43);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory_trap.0.wasm");

    START_FUNC_TESTS(vm, "store");
    TEST_IN_I32_I32(vm, -4, 42);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "load");
    TEST_IN_I32_OUT_I32(vm, -4, 42);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "store");
    TEST_IN_I32_I32_TRAP(vm, -3, 13, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "load");
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "store");
    TEST_IN_I32_I32_TRAP(vm, -2, 13, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "load");
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "store");
    TEST_IN_I32_I32_TRAP(vm, -1, 13, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "load");
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "store");
    TEST_IN_I32_I32_TRAP(vm, 0, 13, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "load");
    TEST_IN_I32_TRAP(vm, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "store");
    TEST_IN_I32_I32_TRAP(vm, 0x80000000, 13, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "load");
    TEST_IN_I32_TRAP(vm, 0x80000000, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "grow_memory");
    TEST_IN_I32_OUT_I32(vm, 0x10001, -1);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);

    load_mdle(vm, dir, path_buf, path_buf_sz, "memory_trap.1.wasm");


    START_FUNC_TESTS(vm, "i32.store");
    TEST_IN_I32_I32_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, 0xfffe, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, 0xfffd, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -3, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -4, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.store");
    TEST_IN_I32_I64_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffe, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffd, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffc, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffb, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffa, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfff9, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -3, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -4, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -5, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -6, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -7, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -8, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "f32.store");
    TEST_IN_I32_F32_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, 0xfffe, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, 0xfffd, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, -3, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F32_TRAP(vm, -4, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "f64.store");
    TEST_IN_I32_F64_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xfffe, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xfffd, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xfffc, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xfffb, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xfffa, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, 0xfff9, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -3, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -4, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -5, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -6, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -7, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_F64_TRAP(vm, -8, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.store8");
    TEST_IN_I32_I32_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.store16");
    TEST_IN_I32_I32_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I32_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.store8");
    TEST_IN_I32_I64_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.store16");
    TEST_IN_I32_I64_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.store32");
    TEST_IN_I32_I64_TRAP(vm, 0x10000, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xffff, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffe, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, 0xfffd, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -1, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -2, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -3, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_I64_TRAP(vm, -4, 0, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.load");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffe, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffd, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -4, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffe, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffd, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffc, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffb, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffa, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfff9, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -4, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -5, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -6, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -7, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -8, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "f32.load");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffe, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffd, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -4, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "f64.load");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffe, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffd, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffc, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffb, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffa, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfff9, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -4, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -5, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -6, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -7, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -8, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.load8_s");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.load8_u");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.load16_s");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i32.load16_u");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load8_s");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load8_u");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load16_s");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load16_u");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load32_s");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffe, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffd, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -4, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load32_u");
    TEST_IN_I32_TRAP(vm, 0x10000, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xffff, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffe, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, 0xfffd, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -1, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -2, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -3, WRP_ERR_INVALID_MEMORY_ACCESS);
    TEST_IN_I32_TRAP(vm, -4, WRP_ERR_INVALID_MEMORY_ACCESS);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "i64.load");
    TEST_IN_I32_OUT_I64(vm, 0xfff8, 0x6867666564636261);
    TEST_IN_I32_OUT_I64(vm, 0, 0x6867666564636261);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);
}
