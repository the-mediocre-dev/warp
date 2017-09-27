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

#include "i64-tests.h"
#include "test-builder.h"
#include "test-common.h"

void run_i64_tests(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    load_mdle(vm, dir, path_buf, path_buf_sz, "i64.wasm");

    START_FUNC_TESTS(vm, "add");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, -2);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, -1, 0x7fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x3fffffff, 1, 0x40000000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "sub");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, -1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 1, 0x7fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x3fffffff, -1, 0x40000000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "mul");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x1000000000000000, 4096, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 0, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, -1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, -1, 0x8000000000000001);
    TEST_IN_I64_I64_OUT_I64(vm, 0x0123456789abcdef, 0xfedcba9876543210, 0x2236d88fe5618cf0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "div_s");
    TEST_IN_I64_I64_TRAP(vm, 1, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_TRAP(vm, 0, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_TRAP(vm, 0x8000000000000000, -1, WRP_ERR_I64_OVERFLOW);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 2, 0xc000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000001, 1000, 0xffdf3b645a1cac09);
    TEST_IN_I64_I64_OUT_I64(vm, 5, 2, 2);
    TEST_IN_I64_I64_OUT_I64(vm, -5, 2, -2);
    TEST_IN_I64_I64_OUT_I64(vm, 5, -2, -2);
    TEST_IN_I64_I64_OUT_I64(vm, -5, -2, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 7, 3, 2);
    TEST_IN_I64_I64_OUT_I64(vm, -7, 3, -2);
    TEST_IN_I64_I64_OUT_I64(vm, 7, -3, -2);
    TEST_IN_I64_I64_OUT_I64(vm, -7, -3, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 11, 5, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 17, 7, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "div_u");
    TEST_IN_I64_I64_TRAP(vm, 1, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_TRAP(vm, 0, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 2, 0x4000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8ff00ff00ff00ff0, 0x100000001, 0x8ff00fef);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000001, 1000, 0x20c49ba5e353f7);
    TEST_IN_I64_I64_OUT_I64(vm, 5, 2, 2);
    TEST_IN_I64_I64_OUT_I64(vm, -5, 2, 0x7ffffffffffffffd);
    TEST_IN_I64_I64_OUT_I64(vm, 5, -2, 0);
    TEST_IN_I64_I64_OUT_I64(vm, -5, -2, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 7, 3, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 11, 5, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 17, 7, 2);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rem_s");
    TEST_IN_I64_I64_TRAP(vm, 1, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_TRAP(vm, 0, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 2, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000001, 1000, -807);
    TEST_IN_I64_I64_OUT_I64(vm, 5, 2, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -5, 2, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 5, -2, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -5, -2, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 7, 3, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -7, 3, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 7, -3, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -7, -3, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 11, 5, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 17, 7, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rem_u");
    TEST_IN_I64_I64_TRAP(vm, 1, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_TRAP(vm, 0, 0, WRP_ERR_I64_DIVIDE_BY_ZERO);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, -1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 2, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8ff00ff00ff00ff0, 0x100000001, 0x80000001);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000001, 1000, 809);
    TEST_IN_I64_I64_OUT_I64(vm, 5, 2, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -5, 2, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 5, -2, 5);
    TEST_IN_I64_I64_OUT_I64(vm, -5, -2, -5);
    TEST_IN_I64_I64_OUT_I64(vm, 7, 3, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 11, 5, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 17, 7, 3);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "and");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, -1, 0x7fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0xf0f0ffff, 0xfffff0f0, 0xf0f0f0f0);
    TEST_IN_I64_I64_OUT_I64(vm, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "or");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 0x8000000000000000, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 0, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0xf0f0ffff, 0xfffff0f0, 0xffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "xor");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 0x8000000000000000, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 0, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 0x8000000000000000, 0x7fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 0x7fffffffffffffff, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0xf0f0ffff, 0xfffff0f0, 0x0f0f0f0f);
    TEST_IN_I64_I64_OUT_I64(vm, 0xffffffffffffffff, 0xffffffffffffffff, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "shl");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 1, 0xfffffffffffffffe);
    TEST_IN_I64_I64_OUT_I64(vm, 0xffffffffffffffff, 1, 0xfffffffffffffffe);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 0x4000000000000000, 1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 63, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 64, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 65, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 1, -1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0x7fffffffffffffff, 0x8000000000000000);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "shr_s");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 1, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 1, 0x3fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 1, 0xc000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x4000000000000000, 1, 0x2000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 64, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 65, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 63, -1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 64, -1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 65, -1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, -1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 0x7fffffffffffffff, -1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 0x8000000000000000, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "shr_u");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 1, 0x7fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0x7fffffffffffffff, 1, 0x3fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 1, 0x4000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x4000000000000000, 1, 0x2000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 64, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 65, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, -1, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 63, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 64, -1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 65, 0x7fffffffffffffff);
    TEST_IN_I64_I64_OUT_I64(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 0x8000000000000000, -1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rotl");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 1, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 64, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd987602468ace, 1, 0x579b30ec048d159d);
    TEST_IN_I64_I64_OUT_I64(vm, 0xfe000000dc000000, 4, 0xe000000dc000000f);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd1234ef567809, 53, 0x013579a2469deacf);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabd1234ef567809c, 63, 0x55e891a77ab3c04e);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd1234ef567809, 0xf5, 0x013579a2469deacf);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd7294ef567809, 0xffffffffffffffed, 0xcf013579ae529dea);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabd1234ef567809c, 0x800000000000003f, 0x55e891a77ab3c04e);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 63, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 1, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "rotr");
    TEST_IN_I64_I64_OUT_I64(vm, 1, 1, 0x8000000000000000);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I64(vm, -1, 1, -1);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 64, 1);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd987602468ace, 1, 0x55e6cc3b01234567);
    TEST_IN_I64_I64_OUT_I64(vm, 0xfe000000dc000000, 4, 0x0fe000000dc00000);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd1234ef567809, 53, 0x6891a77ab3c04d5e);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabd1234ef567809c, 63, 0x57a2469deacf0139);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd1234ef567809, 0xf5, 0x6891a77ab3c04d5e);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabcd7294ef567809, 0xffffffffffffffed, 0x94a77ab3c04d5e6b);
    TEST_IN_I64_I64_OUT_I64(vm, 0xabd1234ef567809c, 0x800000000000003f, 0x57a2469deacf0139);
    TEST_IN_I64_I64_OUT_I64(vm, 1, 63, 2);
    TEST_IN_I64_I64_OUT_I64(vm, 0x8000000000000000, 63, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "clz");
    TEST_IN_I64_OUT_I64(vm, 0xffffffffffffffff, 0);
    TEST_IN_I64_OUT_I64(vm, 0, 64);
    TEST_IN_I64_OUT_I64(vm, 0x00008000, 48);
    TEST_IN_I64_OUT_I64(vm, 0xff, 56);
    TEST_IN_I64_OUT_I64(vm, 0x8000000000000000, 0);
    TEST_IN_I64_OUT_I64(vm, 1, 63);
    TEST_IN_I64_OUT_I64(vm, 2, 62);
    TEST_IN_I64_OUT_I64(vm, 0x7fffffffffffffff, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ctz");
    TEST_IN_I64_OUT_I64(vm, -1, 0);
    TEST_IN_I64_OUT_I64(vm, 0, 64);
    TEST_IN_I64_OUT_I64(vm, 0x00008000, 15);
    TEST_IN_I64_OUT_I64(vm, 0x00010000, 16);
    TEST_IN_I64_OUT_I64(vm, 0x8000000000000000, 63);
    TEST_IN_I64_OUT_I64(vm, 0x7fffffffffffffff, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "popcnt");
    TEST_IN_I64_OUT_I64(vm, -1, 64);
    TEST_IN_I64_OUT_I64(vm, 0, 0);
    TEST_IN_I64_OUT_I64(vm, 0x00008000, 1);
    TEST_IN_I64_OUT_I64(vm, 0x8000800080008000, 4);
    TEST_IN_I64_OUT_I64(vm, 0x7fffffffffffffff, 63);
    TEST_IN_I64_OUT_I64(vm, 0xAAAAAAAA55555555, 32);
    TEST_IN_I64_OUT_I64(vm, 0x99999999AAAAAAAA, 32);
    TEST_IN_I64_OUT_I64(vm, 0xDEADBEEFDEADBEEF, 48);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "eqz");
    TEST_IN_I64_OUT_I32(vm, 0, 1);
    TEST_IN_I64_OUT_I32(vm, 1, 0);
    TEST_IN_I64_OUT_I32(vm, 0x8000000000000000, 0);
    TEST_IN_I64_OUT_I32(vm, 0x7fffffffffffffff, 0);
    TEST_IN_I64_OUT_I32(vm, 0xffffffffffffffff, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "eq");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ne");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "lt_s");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "lt_u");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "le_s");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "le_u");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "gt_s");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "gt_u");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ge_s");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 1);
    END_FUNC_TESTS((*passed), (*failed));

    START_FUNC_TESTS(vm, "ge_u");
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, -1, -1, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 1, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0, 0x8000000000000000, 0);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, -1, 0);
    TEST_IN_I64_I64_OUT_I32(vm, -1, 0x8000000000000000, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x8000000000000000, 0x7fffffffffffffff, 1);
    TEST_IN_I64_I64_OUT_I32(vm, 0x7fffffffffffffff, 0x8000000000000000, 0);
    END_FUNC_TESTS((*passed), (*failed));

    unload_mdle(vm);
}
