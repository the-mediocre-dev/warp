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

#include "i32-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_i32_tests(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "i32.wasm");

    START_FUNC_TESTS(vm, "add");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, -2);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0x7fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x3fffffff, 1, 0x40000000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "sub");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, -1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 1, 0x7fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x3fffffff, -1, 0x40000000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "mul");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x10000000, 4096, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, -1, 0x80000001);
    TEST_IN_I32_I32_OUT_I32(vm, 0x01234567, 0x76543210, 0x358e7470);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "div_s");
    TEST_IN_I32_I32_TRAP(vm, 1, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_TRAP(vm, 0, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_TRAP(vm, 0x80000000, -1, WRP_ERR_I32_OVERFLOW);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 2, 0xc0000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000001, 1000, 0xffdf3b65);
    TEST_IN_I32_I32_OUT_I32(vm, 5, 2, 2);
    TEST_IN_I32_I32_OUT_I32(vm, -5, 2, -2);
    TEST_IN_I32_I32_OUT_I32(vm, 5, -2, -2);
    TEST_IN_I32_I32_OUT_I32(vm, -5, -2, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 7, 3, 2);
    TEST_IN_I32_I32_OUT_I32(vm, -7, 3, -2);
    TEST_IN_I32_I32_OUT_I32(vm, 7, -3, -2);
    TEST_IN_I32_I32_OUT_I32(vm, -7, -3, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 11, 5, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 17, 7, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "div_u");
    TEST_IN_I32_I32_TRAP(vm, 1, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_TRAP(vm, 0, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 2, 0x40000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x8ff00ff0, 0x10001, 0x8fef);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000001, 1000, 0x20c49b);
    TEST_IN_I32_I32_OUT_I32(vm, 5, 2, 2);
    TEST_IN_I32_I32_OUT_I32(vm, -5, 2, 0x7ffffffd);
    TEST_IN_I32_I32_OUT_I32(vm, 5, -2, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -5, -2, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 7, 3, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 11, 5, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 17, 7, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rem_s");
    TEST_IN_I32_I32_TRAP(vm, 1, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_TRAP(vm, 0, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 2, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000001, 1000, -647);
    TEST_IN_I32_I32_OUT_I32(vm, 5, 2, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -5, 2, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 5, -2, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -5, -2, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 7, 3, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -7, 3, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 7, -3, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -7, -3, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 11, 5, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 17, 7, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rem_u");
    TEST_IN_I32_I32_TRAP(vm, 1, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_TRAP(vm, 0, 0, WRP_ERR_I32_DIVIDE_BY_ZERO);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 2, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x8ff00ff0, 0x10001, 0x8001);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000001, 1000, 649);
    TEST_IN_I32_I32_OUT_I32(vm, 5, 2, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -5, 2, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 5, -2, 5);
    TEST_IN_I32_I32_OUT_I32(vm, -5, -2, -5);
    TEST_IN_I32_I32_OUT_I32(vm, 7, 3, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 11, 5, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 17, 7, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "and");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, -1, 0x7fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0xf0f0ffff, 0xfffff0f0, 0xf0f0f0f0);
    TEST_IN_I32_I32_OUT_I32(vm, 0xffffffff, 0xffffffff, 0xffffffff);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "or");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0xf0f0ffff, 0xfffff0f0, 0xffffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0xffffffff, 0xffffffff, 0xffffffff);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "xor");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 0x7fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x7fffffff, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0xf0f0ffff, 0xfffff0f0, 0x0f0f0f0f);
    TEST_IN_I32_I32_OUT_I32(vm, 0xffffffff, 0xffffffff, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "shl");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 1, 0xfffffffe);
    TEST_IN_I32_I32_OUT_I32(vm, 0xffffffff, 1, 0xfffffffe);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x40000000, 1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 31, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 32, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 33, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 1, -1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0x7fffffff, 0x80000000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "shr_s");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 1, 0x3fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 1, 0xc0000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x40000000, 1, 0x20000000);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 32, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 33, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 31, -1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 32, -1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 33, -1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, -1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x7fffffff, -1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "shr_u");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0x7fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 1, 0x3fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 1, 0x40000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x40000000, 1, 0x20000000);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 32, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 33, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 31, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 32, -1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 33, 0x7fffffff);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rotl");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 32, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0xabcd9876, 1, 0x579b30ed);
    TEST_IN_I32_I32_OUT_I32(vm, 0xfe00dc00, 4, 0xe00dc00f);
    TEST_IN_I32_I32_OUT_I32(vm, 0xb0c1d2e3, 5, 0x183a5c76);
    TEST_IN_I32_I32_OUT_I32(vm, 0x00008000, 37, 0x00100000);
    TEST_IN_I32_I32_OUT_I32(vm, 0xb0c1d2e3, 0xff05, 0x183a5c76);
    TEST_IN_I32_I32_OUT_I32(vm, 0x769abcdf, 0xffffffed, 0x579beed3);
    TEST_IN_I32_I32_OUT_I32(vm, 0x769abcdf, 0x8000000d, 0x579beed3);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 31, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 1, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rotr");
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0x80000000);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, -1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 32, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0xff00cc00, 1, 0x7f806600);
    TEST_IN_I32_I32_OUT_I32(vm, 0x00080000, 4, 0x00008000);
    TEST_IN_I32_I32_OUT_I32(vm, 0xb0c1d2e3, 5, 0x1d860e97);
    TEST_IN_I32_I32_OUT_I32(vm, 0x00008000, 37, 0x00000400);
    TEST_IN_I32_I32_OUT_I32(vm, 0xb0c1d2e3, 0xff05, 0x1d860e97);
    TEST_IN_I32_I32_OUT_I32(vm, 0x769abcdf, 0xffffffed, 0xe6fbb4d5);
    TEST_IN_I32_I32_OUT_I32(vm, 0x769abcdf, 0x8000000d, 0xe6fbb4d5);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 31, 2);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 31, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "clz");
    TEST_IN_I32_OUT_I32(vm, 0xffffffff, 0);
    TEST_IN_I32_OUT_I32(vm, 0, 32);
    TEST_IN_I32_OUT_I32(vm, 0x00008000, 16);
    TEST_IN_I32_OUT_I32(vm, 0xff, 24);
    TEST_IN_I32_OUT_I32(vm, 0x80000000, 0);
    TEST_IN_I32_OUT_I32(vm, 1, 31);
    TEST_IN_I32_OUT_I32(vm, 2, 30);
    TEST_IN_I32_OUT_I32(vm, 0x7fffffff, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ctz");
    TEST_IN_I32_OUT_I32(vm, -1, 0);
    TEST_IN_I32_OUT_I32(vm, 0, 32);
    TEST_IN_I32_OUT_I32(vm, 0x00008000, 15);
    TEST_IN_I32_OUT_I32(vm, 0x00010000, 16);
    TEST_IN_I32_OUT_I32(vm, 0x80000000, 31);
    TEST_IN_I32_OUT_I32(vm, 0x7fffffff, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "popcnt");
    TEST_IN_I32_OUT_I32(vm, -1, 32);
    TEST_IN_I32_OUT_I32(vm, 0, 0);
    TEST_IN_I32_OUT_I32(vm, 0x00008000, 1);
    TEST_IN_I32_OUT_I32(vm, 0x80008000, 2);
    TEST_IN_I32_OUT_I32(vm, 0x7fffffff, 31);
    TEST_IN_I32_OUT_I32(vm, 0xAAAAAAAA, 16);
    TEST_IN_I32_OUT_I32(vm, 0x55555555, 16);
    TEST_IN_I32_OUT_I32(vm, 0xDEADBEEF, 24);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "eqz");
    TEST_IN_I32_OUT_I32(vm, 0, 1);
    TEST_IN_I32_OUT_I32(vm, 1, 0);
    TEST_IN_I32_OUT_I32(vm, 0x80000000, 0);
    TEST_IN_I32_OUT_I32(vm, 0x7fffffff, 0);
    TEST_IN_I32_OUT_I32(vm, 0xffffffff, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "eq");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ne");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "lt_s");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "lt_u");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "le_s");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "le_u");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "gt_s");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "gt_u");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ge_s");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ge_u");
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0, 0x80000000, 0);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, -1, 0);
    TEST_IN_I32_I32_OUT_I32(vm, -1, 0x80000000, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x80000000, 0x7fffffff, 1);
    TEST_IN_I32_I32_OUT_I32(vm, 0x7fffffff, 0x80000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);
}
