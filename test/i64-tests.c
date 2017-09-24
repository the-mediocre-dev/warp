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

/* add test 01 */
START_TEST(vm, i64_add_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "add")
CHECK_I64(vm, 2)
END_TEST()

/* add test 02 */
START_TEST(vm, i64_add_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "add")
CHECK_I64(vm, 1)
END_TEST()

/* add test 03 */
START_TEST(vm, i64_add_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "add")
CHECK_I64(vm, -2)
END_TEST()

/* add test 04 */
START_TEST(vm, i64_add_test_04)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "add")
CHECK_I64(vm, 0)
END_TEST()

/* add test 05 */
START_TEST(vm, i64_add_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 1)
CALL(vm, "add")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* add test 06 */
START_TEST(vm, i64_add_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "add")
CHECK_I64(vm, 0x7fffffffffffffff)
END_TEST()

/* add test 07 */
START_TEST(vm, i64_add_test_07)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "add")
CHECK_I64(vm, 0)
END_TEST()

/* add test 07 */
START_TEST(vm, i64_add_test_08)
PUSH_I64(vm, 0x3fffffff)
PUSH_I64(vm, 1)
CALL(vm, "add")
CHECK_I64(vm, 0x40000000)
END_TEST()

/* sub test 01 */
START_TEST(vm, i64_sub_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "sub")
CHECK_I64(vm, 0)
END_TEST()

/* sub test 02 */
START_TEST(vm, i64_sub_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "sub")
CHECK_I64(vm, 1)
END_TEST()

/* sub test 03 */
START_TEST(vm, i64_sub_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "sub")
CHECK_I64(vm, 0)
END_TEST()

/* sub test 04 */
START_TEST(vm, i64_sub_test_04)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, -1)
CALL(vm, "sub")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* sub test 05 */
START_TEST(vm, i64_sub_test_05)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "sub")
CHECK_I64(vm, 0x7fffffffffffffff)
END_TEST()

/* sub test 06 */
START_TEST(vm, i64_sub_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "sub")
CHECK_I64(vm, 0)
END_TEST()

/* sub test 07 */
START_TEST(vm, i64_sub_test_07)
PUSH_I64(vm, 0x3fffffff)
PUSH_I64(vm, -1)
CALL(vm, "sub")
CHECK_I64(vm, 0x40000000)
END_TEST()

/* mul test 01 */
START_TEST(vm, i64_mul_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "mul")
CHECK_I64(vm, 1)
END_TEST()

/* mul test 02 */
START_TEST(vm, i64_mul_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "mul")
CHECK_I64(vm, 0)
END_TEST()

/* mul test 03 */
START_TEST(vm, i64_mul_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "mul")
CHECK_I64(vm, 1)
END_TEST()

/* mul test 04 */
START_TEST(vm, i64_mul_test_04)
PUSH_I64(vm, 0x1000000000000000)
PUSH_I64(vm, 4096)
CALL(vm, "mul")
CHECK_I64(vm, 0)
END_TEST()

/* mul test 05 */
START_TEST(vm, i64_mul_test_05)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "mul")
CHECK_I64(vm, 0)
END_TEST()

/* mul test 06 */
START_TEST(vm, i64_mul_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "mul")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* mul test 07 */
START_TEST(vm, i64_mul_test_07)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, -1)
CALL(vm, "mul")
CHECK_I64(vm, 0x8000000000000001)
END_TEST()

/* mul test 08 */
START_TEST(vm, i64_mul_test_08)
PUSH_I64(vm, 0x0123456789abcdef)
PUSH_I64(vm, 0xfedcba9876543210)
CALL(vm, "mul")
CHECK_I64(vm, 0x2236d88fe5618cf0)
END_TEST()

/* mul test 09 */
START_TEST(vm, i64_mul_test_09)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "mul")
CHECK_I64(vm, 1)
END_TEST()

/* div_s test 01 */
START_TEST(vm, i64_div_s_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "div_s", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* div_s test 02 */
START_TEST(vm, i64_div_s_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "div_s", WRP_ERR_I64_DIVIDE_BY_ZERO)
END_TEST()

/* div_s test 03 */
START_TEST(vm, i64_div_s_test_03)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL_AND_TRAP(vm, "div_s", WRP_ERR_I64_OVERFLOW)
END_TEST()

/* div_s test 04 */
START_TEST(vm, i64_div_s_test_04)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "div_s")
CHECK_I64(vm, 1)
END_TEST()

/* div_s test 05 */
START_TEST(vm, i64_div_s_test_05)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "div_s")
CHECK_I64(vm, 0)
END_TEST()

/* div_s test 06 */
START_TEST(vm, i64_div_s_test_06)
PUSH_I64(vm, 0)
PUSH_I64(vm, -1)
CALL(vm, "div_s")
CHECK_I64(vm, 0)
END_TEST()

/* div_s test 07 */
START_TEST(vm, i64_div_s_test_07)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "div_s")
CHECK_I64(vm, 1)
END_TEST()

/* div_s test 08 */
START_TEST(vm, i64_div_s_test_08)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 2)
CALL(vm, "div_s")
CHECK_I64(vm, 0xc000000000000000)
END_TEST()

/* div_s test 09 */
START_TEST(vm, i64_div_s_test_09)
PUSH_I64(vm, 0x8000000000000001)
PUSH_I64(vm, 1000)
CALL(vm, "div_s")
CHECK_I64(vm, 0xffdf3b645a1cac09)
END_TEST()

/* div_s test 10 */
START_TEST(vm, i64_div_s_test_10)
PUSH_I64(vm, 5)
PUSH_I64(vm, 2)
CALL(vm, "div_s")
CHECK_I64(vm, 2)
END_TEST()

/* div_s test 11 */
START_TEST(vm, i64_div_s_test_11)
PUSH_I64(vm, -5)
PUSH_I64(vm, 2)
CALL(vm, "div_s")
CHECK_I64(vm, -2)
END_TEST()

/* div_s test 12 */
START_TEST(vm, i64_div_s_test_12)
PUSH_I64(vm, 5)
PUSH_I64(vm, -2)
CALL(vm, "div_s")
CHECK_I64(vm, -2)
END_TEST()

/* div_s test 13 */
START_TEST(vm, i64_div_s_test_13)
PUSH_I64(vm, -5)
PUSH_I64(vm, -2)
CALL(vm, "div_s")
CHECK_I64(vm, 2)
END_TEST()

/* div_s test 14 */
START_TEST(vm, i64_div_s_test_14)
PUSH_I64(vm, 7)
PUSH_I64(vm, 3)
CALL(vm, "div_s")
CHECK_I64(vm, 2)
END_TEST()

/* div_s test 15 */
START_TEST(vm, i64_div_s_test_15)
PUSH_I64(vm, -7)
PUSH_I64(vm, 3)
CALL(vm, "div_s")
CHECK_I64(vm, -2)
END_TEST()

/* div_s test 16 */
START_TEST(vm, i64_div_s_test_16)
PUSH_I64(vm, 7)
PUSH_I64(vm, -3)
CALL(vm, "div_s")
CHECK_I64(vm, -2)
END_TEST()

/* div_s test 17 */
START_TEST(vm, i64_div_s_test_17)
PUSH_I64(vm, -7)
PUSH_I64(vm, -3)
CALL(vm, "div_s")
CHECK_I64(vm, 2)
END_TEST()

/* div_s test 18 */
START_TEST(vm, i64_div_s_test_18)
PUSH_I64(vm, 11)
PUSH_I64(vm, 5)
CALL(vm, "div_s")
CHECK_I64(vm, 2)
END_TEST()

/* div_s test 19 */
START_TEST(vm, i64_div_s_test_19)
PUSH_I64(vm, 17)
PUSH_I64(vm, 7)
CALL(vm, "div_s")
CHECK_I64(vm, 2)
END_TEST()

/* div_u test 01 */
START_TEST(vm, i64_div_u_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "div_u", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* div_u test 02 */
START_TEST(vm, i64_div_u_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "div_u", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* div_u test 03 */
START_TEST(vm, i64_div_u_test_03)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "div_u")
CHECK_I64(vm, 1)
END_TEST()

/* div_u test 04 */
START_TEST(vm, i64_div_u_test_04)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "div_u")
CHECK_I64(vm, 0)
END_TEST()

/* div_u test 05 */
START_TEST(vm, i64_div_u_test_05)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "div_u")
CHECK_I64(vm, 1)
END_TEST()

/* div_u test 06 */
START_TEST(vm, i64_div_u_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "div_u")
CHECK_I64(vm, 0)
END_TEST()

/* div_u test 07 */
START_TEST(vm, i64_div_u_test_07)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 2)
CALL(vm, "div_u")
CHECK_I64(vm, 0x4000000000000000)
END_TEST()

/* div_u test 08 */
START_TEST(vm, i64_div_u_test_08)
PUSH_I64(vm, 0x8ff00ff00ff00ff0)
PUSH_I64(vm, 0x100000001)
CALL(vm, "div_u")
CHECK_I64(vm, 0x8ff00fef)
END_TEST()

/* div_u test 09 */
START_TEST(vm, i64_div_u_test_09)
PUSH_I64(vm, 0x8000000000000001)
PUSH_I64(vm, 1000)
CALL(vm, "div_u")
CHECK_I64(vm, 0x20c49ba5e353f7)
END_TEST()

/* div_u test 10 */
START_TEST(vm, i64_div_u_test_10)
PUSH_I64(vm, 5)
PUSH_I64(vm, 2)
CALL(vm, "div_u")
CHECK_I64(vm, 2)
END_TEST()

/* div_u test 11 */
START_TEST(vm, i64_div_u_test_11)
PUSH_I64(vm, -5)
PUSH_I64(vm, 2)
CALL(vm, "div_u")
CHECK_I64(vm, 0x7ffffffffffffffd)
END_TEST()

/* div_u test 12 */
START_TEST(vm, i64_div_u_test_12)
PUSH_I64(vm, 5)
PUSH_I64(vm, -2)
CALL(vm, "div_u")
CHECK_I64(vm, 0)
END_TEST()

/* div_u test 13 */
START_TEST(vm, i64_div_u_test_13)
PUSH_I64(vm, -5)
PUSH_I64(vm, -2)
CALL(vm, "div_u")
CHECK_I64(vm, 0)
END_TEST()

/* div_u test 14 */
START_TEST(vm, i64_div_u_test_14)
PUSH_I64(vm, 7)
PUSH_I64(vm, 3)
CALL(vm, "div_u")
CHECK_I64(vm, 2)
END_TEST()

/* div_u test 15 */
START_TEST(vm, i64_div_u_test_15)
PUSH_I64(vm, 11)
PUSH_I64(vm, 5)
CALL(vm, "div_u")
CHECK_I64(vm, 2)
END_TEST()

/* div_u test 16 */
START_TEST(vm, i64_div_u_test_16)
PUSH_I64(vm, 17)
PUSH_I64(vm, 7)
CALL(vm, "div_u")
CHECK_I64(vm, 2)
END_TEST()

/* rem_s test 01 */
START_TEST(vm, i64_rem_s_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "rem_s", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* rem_s test 02 */
START_TEST(vm, i64_rem_s_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "rem_s", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* rem_s test 03 */
START_TEST(vm, i64_rem_s_test_03)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, -1)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 04 */
START_TEST(vm, i64_rem_s_test_04)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 05 */
START_TEST(vm, i64_rem_s_test_05)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 06 */
START_TEST(vm, i64_rem_s_test_06)
PUSH_I64(vm, 0)
PUSH_I64(vm, -1)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 07 */
START_TEST(vm, i64_rem_s_test_07)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 08 */
START_TEST(vm, i64_rem_s_test_08)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 09 */
START_TEST(vm, i64_rem_s_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 2)
CALL(vm, "rem_s")
CHECK_I64(vm, 0)
END_TEST()

/* rem_s test 10 */
START_TEST(vm, i64_rem_s_test_10)
PUSH_I64(vm, 0x8000000000000001)
PUSH_I64(vm, 1000)
CALL(vm, "rem_s")
CHECK_I64(vm, -807)
END_TEST()

/* rem_s test 11 */
START_TEST(vm, i64_rem_s_test_11)
PUSH_I64(vm, 5)
PUSH_I64(vm, 2)
CALL(vm, "rem_s")
CHECK_I64(vm, 1)
END_TEST()

/* rem_s test 12 */
START_TEST(vm, i64_rem_s_test_12)
PUSH_I64(vm, -5)
PUSH_I64(vm, 2)
CALL(vm, "rem_s")
CHECK_I64(vm, -1)
END_TEST()

/* rem_s test 13 */
START_TEST(vm, i64_rem_s_test_13)
PUSH_I64(vm, 5)
PUSH_I64(vm, -2)
CALL(vm, "rem_s")
CHECK_I64(vm, 1)
END_TEST()

/* rem_s test 14 */
START_TEST(vm, i64_rem_s_test_14)
PUSH_I64(vm, -5)
PUSH_I64(vm, -2)
CALL(vm, "rem_s")
CHECK_I64(vm, -1)
END_TEST()

/* rem_s test 15 */
START_TEST(vm, i64_rem_s_test_15)
PUSH_I64(vm, 7)
PUSH_I64(vm, 3)
CALL(vm, "rem_s")
CHECK_I64(vm, 1)
END_TEST()

/* rem_s test 16 */
START_TEST(vm, i64_rem_s_test_16)
PUSH_I64(vm, -7)
PUSH_I64(vm, 3)
CALL(vm, "rem_s")
CHECK_I64(vm, -1)
END_TEST()

/* rem_s test 17 */
START_TEST(vm, i64_rem_s_test_17)
PUSH_I64(vm, 7)
PUSH_I64(vm, -3)
CALL(vm, "rem_s")
CHECK_I64(vm, 1)
END_TEST()

/* rem_s test 18 */
START_TEST(vm, i64_rem_s_test_18)
PUSH_I64(vm, -7)
PUSH_I64(vm, -3)
CALL(vm, "rem_s")
CHECK_I64(vm, -1)
END_TEST()

/* rem_s test 19 */
START_TEST(vm, i64_rem_s_test_19)
PUSH_I64(vm, 11)
PUSH_I64(vm, 5)
CALL(vm, "rem_s")
CHECK_I64(vm, 1)
END_TEST()

/* rem_s test 20 */
START_TEST(vm, i64_rem_s_test_20)
PUSH_I64(vm, 17)
PUSH_I64(vm, 7)
CALL(vm, "rem_s")
CHECK_I64(vm, 3)
END_TEST()

/* rem_u test 01 */
START_TEST(vm, i64_rem_u_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "rem_u", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* rem_u test 02 */
START_TEST(vm, i64_rem_u_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL_AND_TRAP(vm, "rem_u", WRP_ERR_I64_DIVIDE_BY_ZERO);
END_TEST()

/* rem_u test 03 */
START_TEST(vm, i64_rem_u_test_03)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "rem_u")
CHECK_I64(vm, 0)
END_TEST()

/* rem_u test 04 */
START_TEST(vm, i64_rem_u_test_04)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "rem_u")
CHECK_I64(vm, 0)
END_TEST()

/* rem_u test 05 */
START_TEST(vm, i64_rem_u_test_05)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "rem_u")
CHECK_I64(vm, 0)
END_TEST()

/* rem_u test 06 */
START_TEST(vm, i64_rem_u_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "rem_u")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* rem_u test 07 */
START_TEST(vm, i64_rem_u_test_07)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 2)
CALL(vm, "rem_u")
CHECK_I64(vm, 0)
END_TEST()

/* rem_u test 08 */
START_TEST(vm, i64_rem_u_test_08)
PUSH_I64(vm, 0x8ff00ff00ff00ff0)
PUSH_I64(vm, 0x100000001)
CALL(vm, "rem_u")
CHECK_I64(vm, 0x80000001)
END_TEST()

/* rem_u test 09 */
START_TEST(vm, i64_rem_u_test_09)
PUSH_I64(vm, 0x8000000000000001)
PUSH_I64(vm, 1000)
CALL(vm, "rem_u")
CHECK_I64(vm, 809)
END_TEST()

/* rem_u test 10 */
START_TEST(vm, i64_rem_u_test_10)
PUSH_I64(vm, 5)
PUSH_I64(vm, 2)
CALL(vm, "rem_u")
CHECK_I64(vm, 1)
END_TEST()

/* rem_u test 11 */
START_TEST(vm, i64_rem_u_test_11)
PUSH_I64(vm, -5)
PUSH_I64(vm, 2)
CALL(vm, "rem_u")
CHECK_I64(vm, 1)
END_TEST()

/* rem_u test 12 */
START_TEST(vm, i64_rem_u_test_12)
PUSH_I64(vm, 5)
PUSH_I64(vm, -2)
CALL(vm, "rem_u")
CHECK_I64(vm, 5)
END_TEST()

/* rem_u test 13 */
START_TEST(vm, i64_rem_u_test_13)
PUSH_I64(vm, -5)
PUSH_I64(vm, -2)
CALL(vm, "rem_u")
CHECK_I64(vm, -5)
END_TEST()

/* rem_u test 14 */
START_TEST(vm, i64_rem_u_test_14)
PUSH_I64(vm, 7)
PUSH_I64(vm, 3)
CALL(vm, "rem_u")
CHECK_I64(vm, 1)
END_TEST()

/* rem_u test 15 */
START_TEST(vm, i64_rem_u_test_15)
PUSH_I64(vm, 11)
PUSH_I64(vm, 5)
CALL(vm, "rem_u")
CHECK_I64(vm, 1)
END_TEST()

/* rem_u test 16 */
START_TEST(vm, i64_rem_u_test_16)
PUSH_I64(vm, 17)
PUSH_I64(vm, 7)
CALL(vm, "rem_u")
CHECK_I64(vm, 3)
END_TEST()

/* and test 01 */
START_TEST(vm, i64_and_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "and")
CHECK_I64(vm, 0)
END_TEST()

/* and test 02 */
START_TEST(vm, i64_and_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "and")
CHECK_I64(vm, 0)
END_TEST()

/* and test 03 */
START_TEST(vm, i64_and_test_03)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "and")
CHECK_I64(vm, 1)
END_TEST()

/* and test 04 */
START_TEST(vm, i64_and_test_04)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "and")
CHECK_I64(vm, 0)
END_TEST()

/* and test 05 */
START_TEST(vm, i64_and_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "and")
CHECK_I64(vm, 0)
END_TEST()

/* and test 06 */
START_TEST(vm, i64_and_test_06)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, -1)
CALL(vm, "and")
CHECK_I64(vm, 0x7fffffffffffffff)
END_TEST()

/* and test 07 */
START_TEST(vm, i64_and_test_07)
PUSH_I64(vm, 0xf0f0ffff)
PUSH_I64(vm, 0xfffff0f0)
CALL(vm, "and")
CHECK_I64(vm, 0xf0f0f0f0)
END_TEST()

/* and test 08 */
START_TEST(vm, i64_and_test_08)
PUSH_I64(vm, 0xffffffffffffffff)
PUSH_I64(vm, 0xffffffffffffffff)
CALL(vm, "and")
CHECK_I64(vm, 0xffffffffffffffff)
END_TEST()

/* or test 01 */
START_TEST(vm, i64_or_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "or")
CHECK_I64(vm, 1)
END_TEST()

/* or test 02 */
START_TEST(vm, i64_or_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "or")
CHECK_I64(vm, 1)
END_TEST()

/* or test 03 */
START_TEST(vm, i64_or_test_03)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "or")
CHECK_I64(vm, 1)
END_TEST()

/* or test 04 */
START_TEST(vm, i64_or_test_04)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "or")
CHECK_I64(vm, 0)
END_TEST()

/* or test 05 */
START_TEST(vm, i64_or_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "or")
CHECK_I64(vm, -1)
END_TEST()

/* or test 06 */
START_TEST(vm, i64_or_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "or")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* or test 07 */
START_TEST(vm, i64_or_test_07)
PUSH_I64(vm, 0xf0f0ffff)
PUSH_I64(vm, 0xfffff0f0)
CALL(vm, "or")
CHECK_I64(vm, 0xffffffff)
END_TEST()

/* or test 08 */
START_TEST(vm, i64_or_test_08)
PUSH_I64(vm, 0xffffffffffffffff)
PUSH_I64(vm, 0xffffffffffffffff)
CALL(vm, "or")
CHECK_I64(vm, 0xffffffffffffffff)
END_TEST()

/* xor test 01 */
START_TEST(vm, i64_xor_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "xor")
CHECK_I64(vm, 1)
END_TEST()

/* xor test 02 */
START_TEST(vm, i64_xor_test_02)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "xor")
CHECK_I64(vm, 1)
END_TEST()

/* xor test 03 */
START_TEST(vm, i64_xor_test_03)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "xor")
CHECK_I64(vm, 0)
END_TEST()

/* xor test 04 */
START_TEST(vm, i64_xor_test_04)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "xor")
CHECK_I64(vm, 0)
END_TEST()

/* xor test 05 */
START_TEST(vm, i64_xor_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "xor")
CHECK_I64(vm, -1)
END_TEST()

/* xor test 06 */
START_TEST(vm, i64_xor_test_06)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "xor")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* xor test 07 */
START_TEST(vm, i64_xor_test_07)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "xor")
CHECK_I64(vm, 0x7fffffffffffffff)
END_TEST()

/* xor test 08 */
START_TEST(vm, i64_xor_test_08)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "xor")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* xor test 09 */
START_TEST(vm, i64_xor_test_09)
PUSH_I64(vm, 0xf0f0ffff)
PUSH_I64(vm, 0xfffff0f0)
CALL(vm, "xor")
CHECK_I64(vm, 0x0f0f0f0f)
END_TEST()

/* xor test 10 */
START_TEST(vm, i64_xor_test_10)
PUSH_I64(vm, 0xffffffffffffffff)
PUSH_I64(vm, 0xffffffffffffffff)
CALL(vm, "xor")
CHECK_I64(vm, 0)
END_TEST()

/* shl test 01 */
START_TEST(vm, i64_shl_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "shl")
CHECK_I64(vm, 2)
END_TEST()

/* shl test 02 */
START_TEST(vm, i64_shl_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "shl")
CHECK_I64(vm, 1)
END_TEST()

/* shl test 03 */
START_TEST(vm, i64_shl_test_03)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 1)
CALL(vm, "shl")
CHECK_I64(vm, 0xfffffffffffffffe)
END_TEST()

/* shl test 04 */
START_TEST(vm, i64_shl_test_04)
PUSH_I64(vm, 0xffffffffffffffff)
PUSH_I64(vm, 1)
CALL(vm, "shl")
CHECK_I64(vm, 0xfffffffffffffffe)
END_TEST()

/* shl test 05 */
START_TEST(vm, i64_shl_test_05)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "shl")
CHECK_I64(vm, 0)
END_TEST()

/* shl test 06 */
START_TEST(vm, i64_shl_test_06)
PUSH_I64(vm, 0x4000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "shl")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* shl test 07 */
START_TEST(vm, i64_shl_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 63)
CALL(vm, "shl")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* shl test 08 */
START_TEST(vm, i64_shl_test_08)
PUSH_I64(vm, 1)
PUSH_I64(vm, 64)
CALL(vm, "shl")
CHECK_I64(vm, 1)
END_TEST()

/* shl test 09 */
START_TEST(vm, i64_shl_test_09)
PUSH_I64(vm, 1)
PUSH_I64(vm, 65)
CALL(vm, "shl")
CHECK_I64(vm, 2)
END_TEST()

/* shl test 10 */
START_TEST(vm, i64_shl_test_10)
PUSH_I64(vm, 1)
PUSH_I64(vm, -1)
CALL(vm, "shl")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* shl test 11 */
START_TEST(vm, i64_shl_test_11)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "shl")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* shr_s test 01 */
START_TEST(vm, i64_shr_s_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "shr_s")
CHECK_I64(vm, 0)
END_TEST()

/* shr_s test 02 */
START_TEST(vm, i64_shr_s_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "shr_s")
CHECK_I64(vm, 1)
END_TEST()

/* shr_s test 03 */
START_TEST(vm, i64_shr_s_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_s test 04 */
START_TEST(vm, i64_shr_s_test_04)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 1)
CALL(vm, "shr_s")
CHECK_I64(vm, 0x3fffffffffffffff)
END_TEST()

/* shr_s test 05 */
START_TEST(vm, i64_shr_s_test_05)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "shr_s")
CHECK_I64(vm, 0xc000000000000000)
END_TEST()

/* shr_s test 06 */
START_TEST(vm, i64_shr_s_test_06)
PUSH_I64(vm, 0x4000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "shr_s")
CHECK_I64(vm, 0x2000000000000000)
END_TEST()

/* shr_s test 07 */
START_TEST(vm, i64_shr_s_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 64)
CALL(vm, "shr_s")
CHECK_I64(vm, 1)
END_TEST()

/* shr_s test 08 */
START_TEST(vm, i64_shr_s_test_08)
PUSH_I64(vm, 1)
PUSH_I64(vm, 33)
CALL(vm, "shr_s")
CHECK_I64(vm, 0)
END_TEST()

/* shr_s test 09 */
START_TEST(vm, i64_shr_s_test_09)
PUSH_I64(vm, 1)
PUSH_I64(vm, -1)
CALL(vm, "shr_s")
CHECK_I64(vm, 0)
END_TEST()

/* shr_s test 10 */
START_TEST(vm, i64_shr_s_test_10)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "shr_s")
CHECK_I64(vm, 0)
END_TEST()

/* shr_s test 11 */
START_TEST(vm, i64_shr_s_test_11)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "shr_s")
CHECK_I64(vm, 1)
END_TEST()

/* shr_s test 12 */
START_TEST(vm, i64_shr_s_test_12)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 63)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_s test 13 */
START_TEST(vm, i64_shr_s_test_13)
PUSH_I64(vm, -1)
PUSH_I64(vm, 32)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_s test 14 */
START_TEST(vm, i64_shr_s_test_14)
PUSH_I64(vm, -1)
PUSH_I64(vm, 33)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_s test 15 */
START_TEST(vm, i64_shr_s_test_15)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_s test 16 */
START_TEST(vm, i64_shr_s_test_16)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_s test 17 */
START_TEST(vm, i64_shr_s_test_17)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "shr_s")
CHECK_I64(vm, -1)
END_TEST()

/* shr_u test 01 */
START_TEST(vm, i64_shr_u_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "shr_u")
CHECK_I64(vm, 0)
END_TEST()

/* shr_u test 02 */
START_TEST(vm, i64_shr_u_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "shr_u")
CHECK_I64(vm, 1)
END_TEST()

/* shr_u test 03 */
START_TEST(vm, i64_shr_u_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "shr_u")
CHECK_I64(vm, 0x7fffffffffffffff)
END_TEST()

/* shr_u test 04 */
START_TEST(vm, i64_shr_u_test_04)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 1)
CALL(vm, "shr_u")
CHECK_I64(vm, 0x3fffffffffffffff)
END_TEST()

/* shr_u test 05 */
START_TEST(vm, i64_shr_u_test_05)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "shr_u")
CHECK_I64(vm, 0x4000000000000000)
END_TEST()

/* shr_u test 06 */
START_TEST(vm, i64_shr_u_test_06)
PUSH_I64(vm, 0x4000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "shr_u")
CHECK_I64(vm, 0x2000000000000000)
END_TEST()

/* shr_u test 07 */
START_TEST(vm, i64_shr_u_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 64)
CALL(vm, "shr_u")
CHECK_I64(vm, 1)
END_TEST()

/* shr_u test 08 */
START_TEST(vm, i64_shr_u_test_08)
PUSH_I64(vm, 1)
PUSH_I64(vm, 33)
CALL(vm, "shr_u")
CHECK_I64(vm, 0)
END_TEST()

/* shr_u test 09 */
START_TEST(vm, i64_shr_u_test_09)
PUSH_I64(vm, 1)
PUSH_I64(vm, -1)
CALL(vm, "shr_u")
CHECK_I64(vm, 0)
END_TEST()

/* shr_u test 10 */
START_TEST(vm, i64_shr_u_test_10)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "shr_u")
CHECK_I64(vm, 0)
END_TEST()

/* shr_u test 11 */
START_TEST(vm, i64_shr_u_test_11)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "shr_u")
CHECK_I64(vm, 1)
END_TEST()

/* shr_u test 12 */
START_TEST(vm, i64_shr_u_test_12)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 63)
CALL(vm, "shr_u")
CHECK_I64(vm, 1)
END_TEST()

/* shr_u test 13 */
START_TEST(vm, i64_shr_u_test_13)
PUSH_I64(vm, -1)
PUSH_I64(vm, 64)
CALL(vm, "shr_u")
CHECK_I64(vm, -1)
END_TEST()

/* shr_u test 14 */
START_TEST(vm, i64_shr_u_test_14)
PUSH_I64(vm, -1)
PUSH_I64(vm, 65)
CALL(vm, "shr_u")
CHECK_I64(vm, 0x7fffffffffffffff)
END_TEST()

/* shr_u test 15 */
START_TEST(vm, i64_shr_u_test_15)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "shr_u")
CHECK_I64(vm, 1)
END_TEST()

/* shr_u test 16 */
START_TEST(vm, i64_shr_u_test_16)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "shr_u")
CHECK_I64(vm, 1)
END_TEST()

/* shr_u test 17 */
START_TEST(vm, i64_shr_u_test_17)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "shr_u")
CHECK_I64(vm, -1)
END_TEST()

/* rotl test 01 */
START_TEST(vm, i64_rotl_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "rotl")
CHECK_I64(vm, 2)
END_TEST()

/* rotl test 02 */
START_TEST(vm, i64_rotl_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "rotl")
CHECK_I64(vm, 1)
END_TEST()

/* rotl test 03 */
START_TEST(vm, i64_rotl_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "rotl")
CHECK_I64(vm, -1)
END_TEST()

/* rotl test 04 */
START_TEST(vm, i64_rotl_test_04)
PUSH_I64(vm, 1)
PUSH_I64(vm, 64)
CALL(vm, "rotl")
CHECK_I64(vm, 1)
END_TEST()

/* rotl test 05 */
START_TEST(vm, i64_rotl_test_05)
PUSH_I64(vm, 0xabcd987602468ace)
PUSH_I64(vm, 1)
CALL(vm, "rotl")
CHECK_I64(vm, 0x579b30ec048d159d)
END_TEST()

/* rotl test 06 */
START_TEST(vm, i64_rotl_test_06)
PUSH_I64(vm, 0xfe000000dc000000)
PUSH_I64(vm, 4)
CALL(vm, "rotl")
CHECK_I64(vm, 0xe000000dc000000f)
END_TEST()

/* rotl test 07 */
START_TEST(vm, i64_rotl_test_07)
PUSH_I64(vm, 0xabcd1234ef567809)
PUSH_I64(vm, 53)
CALL(vm, "rotl")
CHECK_I64(vm, 0x013579a2469deacf)
END_TEST()

/* rotl test 08 */
START_TEST(vm, i64_rotl_test_08)
PUSH_I64(vm, 0xabd1234ef567809c)
PUSH_I64(vm, 63)
CALL(vm, "rotl")
CHECK_I64(vm, 0x55e891a77ab3c04e)
END_TEST()

/* rotl test 09 */
START_TEST(vm, i64_rotl_test_09)
PUSH_I64(vm, 0xabcd1234ef567809)
PUSH_I64(vm, 0xf5)
CALL(vm, "rotl")
CHECK_I64(vm, 0x013579a2469deacf)
END_TEST()

/* rotl test 10 */
START_TEST(vm, i64_rotl_test_10)
PUSH_I64(vm, 0xabcd7294ef567809)
PUSH_I64(vm, 0xffffffffffffffed)
CALL(vm, "rotl")
CHECK_I64(vm, 0xcf013579ae529dea)
END_TEST()

/* rotl test 11 */
START_TEST(vm, i64_rotl_test_11)
PUSH_I64(vm, 0xabd1234ef567809c)
PUSH_I64(vm, 0x800000000000003f)
CALL(vm, "rotl")
CHECK_I64(vm, 0x55e891a77ab3c04e)
END_TEST()

/* rotl test 12 */
START_TEST(vm, i64_rotl_test_12)
PUSH_I64(vm, 1)
PUSH_I64(vm, 63)
CALL(vm, "rotl")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* rotl test 13 */
START_TEST(vm, i64_rotl_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 1)
CALL(vm, "rotl")
CHECK_I64(vm, 1)
END_TEST()

/* rotr test 01 */
START_TEST(vm, i64_rotr_test_01)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "rotr")
CHECK_I64(vm, 0x8000000000000000)
END_TEST()

/* rotr test 02 */
START_TEST(vm, i64_rotr_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "rotr")
CHECK_I64(vm, 1)
END_TEST()

/* rotr test 03 */
START_TEST(vm, i64_rotr_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "rotr")
CHECK_I64(vm, -1)
END_TEST()

/* rotr test 04 */
START_TEST(vm, i64_rotr_test_04)
PUSH_I64(vm, 1)
PUSH_I64(vm, 64)
CALL(vm, "rotr")
CHECK_I64(vm, 1)
END_TEST()

/* rotr test 05 */
START_TEST(vm, i64_rotr_test_05)
PUSH_I64(vm, 0xabcd987602468ace)
PUSH_I64(vm, 1)
CALL(vm, "rotr")
CHECK_I64(vm, 0x55e6cc3b01234567)
END_TEST()

/* rotr test 06 */
START_TEST(vm, i64_rotr_test_06)
PUSH_I64(vm, 0xfe000000dc000000)
PUSH_I64(vm, 4)
CALL(vm, "rotr")
CHECK_I64(vm, 0x0fe000000dc00000)
END_TEST()

/* rotr test 07 */
START_TEST(vm, i64_rotr_test_07)
PUSH_I64(vm, 0xabcd1234ef567809)
PUSH_I64(vm, 53)
CALL(vm, "rotr")
CHECK_I64(vm, 0x6891a77ab3c04d5e)
END_TEST()

/* rotr test 08 */
START_TEST(vm, i64_rotr_test_08)
PUSH_I64(vm, 0xabd1234ef567809c)
PUSH_I64(vm, 63)
CALL(vm, "rotr")
CHECK_I64(vm, 0x57a2469deacf0139)
END_TEST()

/* rotr test 09 */
START_TEST(vm, i64_rotr_test_09)
PUSH_I64(vm, 0xabcd1234ef567809)
PUSH_I64(vm, 0xf5)
CALL(vm, "rotr")
CHECK_I64(vm, 0x6891a77ab3c04d5e)
END_TEST()

/* rotr test 10 */
START_TEST(vm, i64_rotr_test_10)
PUSH_I64(vm, 0xabcd7294ef567809)
PUSH_I64(vm, 0xffffffffffffffed)
CALL(vm, "rotr")
CHECK_I64(vm, 0x94a77ab3c04d5e6b)
END_TEST()

/* rotr test 11 */
START_TEST(vm, i64_rotr_test_11)
PUSH_I64(vm, 0xabd1234ef567809c)
PUSH_I64(vm, 0x800000000000003f)
CALL(vm, "rotr")
CHECK_I64(vm, 0x57a2469deacf0139)
END_TEST()

/* rotr test 12 */
START_TEST(vm, i64_rotr_test_12)
PUSH_I64(vm, 1)
PUSH_I64(vm, 63)
CALL(vm, "rotr")
CHECK_I64(vm, 2)
END_TEST()

/* rotr test 13 */
START_TEST(vm, i64_rotr_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 63)
CALL(vm, "rotr")
CHECK_I64(vm, 1)
END_TEST()

/* clz test 01 */
START_TEST(vm, i64_clz_test_01)
PUSH_I64(vm, 0xffffffffffffffff)
CALL(vm, "clz")
CHECK_I64(vm, 0)
END_TEST()

/* clz test 02 */
START_TEST(vm, i64_clz_test_02)
PUSH_I64(vm, 0)
CALL(vm, "clz")
CHECK_I64(vm, 64)
END_TEST()

/* clz test 03 */
START_TEST(vm, i64_clz_test_03)
PUSH_I64(vm, 0x00008000)
CALL(vm, "clz")
CHECK_I64(vm, 48)
END_TEST()

/* clz test 04 */
START_TEST(vm, i64_clz_test_04)
PUSH_I64(vm, 0xff)
CALL(vm, "clz")
CHECK_I64(vm, 56)
END_TEST()

/* clz test 05 */
START_TEST(vm, i64_clz_test_05)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "clz")
CHECK_I64(vm, 0)
END_TEST()

/* clz test 06 */
START_TEST(vm, i64_clz_test_06)
PUSH_I64(vm, 1)
CALL(vm, "clz")
CHECK_I64(vm, 63)
END_TEST()

/* clz test 07 */
START_TEST(vm, i64_clz_test_07)
PUSH_I64(vm, 2)
CALL(vm, "clz")
CHECK_I64(vm, 62)
END_TEST()

/* clz test 08 */
START_TEST(vm, i64_clz_test_08)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "clz")
CHECK_I64(vm, 1)
END_TEST()

/* ctz test 01 */
START_TEST(vm, i64_ctz_test_01)
PUSH_I64(vm, -1)
CALL(vm, "ctz")
CHECK_I64(vm, 0)
END_TEST()

/* ctz test 02 */
START_TEST(vm, i64_ctz_test_02)
PUSH_I64(vm, 0)
CALL(vm, "ctz")
CHECK_I64(vm, 64)
END_TEST()

/* ctz test 03 */
START_TEST(vm, i64_ctz_test_03)
PUSH_I64(vm, 0x00008000)
CALL(vm, "ctz")
CHECK_I64(vm, 15)
END_TEST()

/* ctz test 04 */
START_TEST(vm, i64_ctz_test_04)
PUSH_I64(vm, 0x00010000)
CALL(vm, "ctz")
CHECK_I64(vm, 16)
END_TEST()

/* ctz test 05 */
START_TEST(vm, i64_ctz_test_05)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ctz")
CHECK_I64(vm, 63)
END_TEST()

/* ctz test 06 */
START_TEST(vm, i64_ctz_test_06)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ctz")
CHECK_I64(vm, 0)
END_TEST()

/* popcnt test 01 */
START_TEST(vm, i64_popcnt_test_01)
PUSH_I64(vm, -1)
CALL(vm, "popcnt")
CHECK_I64(vm, 64)
END_TEST()

/* popcnt test 02 */
START_TEST(vm, i64_popcnt_test_02)
PUSH_I64(vm, 0)
CALL(vm, "popcnt")
CHECK_I64(vm, 0)
END_TEST()

/* popcnt test 03 */
START_TEST(vm, i64_popcnt_test_03)
PUSH_I64(vm, 0x00008000)
CALL(vm, "popcnt")
CHECK_I64(vm, 1)
END_TEST()

/* popcnt test 04 */
START_TEST(vm, i64_popcnt_test_04)
PUSH_I64(vm, 0x8000800080008000)
CALL(vm, "popcnt")
CHECK_I64(vm, 4)
END_TEST()

/* popcnt test 05 */
START_TEST(vm, i64_popcnt_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "popcnt")
CHECK_I64(vm, 63)
END_TEST()

/* popcnt test 06 */
START_TEST(vm, i64_popcnt_test_06)
PUSH_I64(vm, 0xAAAAAAAA55555555)
CALL(vm, "popcnt")
CHECK_I64(vm, 32)
END_TEST()

/* popcnt test 07 */
START_TEST(vm, i64_popcnt_test_07)
PUSH_I64(vm, 0x99999999AAAAAAAA)
CALL(vm, "popcnt")
CHECK_I64(vm, 32)
END_TEST()

/* popcnt test 08 */
START_TEST(vm, i64_popcnt_test_08)
PUSH_I64(vm, 0xDEADBEEFDEADBEEF)
CALL(vm, "popcnt")
CHECK_I64(vm, 48)
END_TEST()

/* eqz test 01 */
START_TEST(vm, i64_eqz_test_01)
PUSH_I64(vm, 0)
CALL(vm, "eqz")
CHECK_I32(vm, 1)
END_TEST()

/* eqz test 02 */
START_TEST(vm, i64_eqz_test_02)
PUSH_I64(vm, 1)
CALL(vm, "eqz")
CHECK_I32(vm, 0)
END_TEST()

/* eqz test 03 */
START_TEST(vm, i64_eqz_test_03)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "eqz")
CHECK_I32(vm, 0)
END_TEST()

/* eqz test 04 */
START_TEST(vm, i64_eqz_test_04)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "eqz")
CHECK_I32(vm, 0)
END_TEST()

/* eqz test 05 */
START_TEST(vm, i64_eqz_test_05)
PUSH_I64(vm, 0xffffffffffffffff)
CALL(vm, "eqz")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 01 */
START_TEST(vm, i64_eq_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "eq")
CHECK_I32(vm, 1)
END_TEST()

/* eq test 02 */
START_TEST(vm, i64_eq_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "eq")
CHECK_I32(vm, 1)
END_TEST()

/* eq test 03 */
START_TEST(vm, i64_eq_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 04 */
START_TEST(vm, i64_eq_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "eq")
CHECK_I32(vm, 1)
END_TEST()

/* eq test 05 */
START_TEST(vm, i64_eq_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "eq")
CHECK_I32(vm, 1)
END_TEST()

/* eq test 06 */
START_TEST(vm, i64_eq_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "eq")
CHECK_I32(vm, 1)
END_TEST()

/* eq test 07 */
START_TEST(vm, i64_eq_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 08 */
START_TEST(vm, i64_eq_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 09 */
START_TEST(vm, i64_eq_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 10 */
START_TEST(vm, i64_eq_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 11 */
START_TEST(vm, i64_eq_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 12 */
START_TEST(vm, i64_eq_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 13 */
START_TEST(vm, i64_eq_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* eq test 14 */
START_TEST(vm, i64_eq_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "eq")
CHECK_I32(vm, 0)
END_TEST()

/* ne test 01 */
START_TEST(vm, i64_ne_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "ne")
CHECK_I32(vm, 0)
END_TEST()

/* ne test 02 */
START_TEST(vm, i64_ne_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "ne")
CHECK_I32(vm, 0)
END_TEST()

/* ne test 03 */
START_TEST(vm, i64_ne_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 04 */
START_TEST(vm, i64_ne_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ne")
CHECK_I32(vm, 0)
END_TEST()

/* ne test 05 */
START_TEST(vm, i64_ne_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ne")
CHECK_I32(vm, 0)
END_TEST()

/* ne test 06 */
START_TEST(vm, i64_ne_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "ne")
CHECK_I32(vm, 0)
END_TEST()

/* ne test 07 */
START_TEST(vm, i64_ne_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 08 */
START_TEST(vm, i64_ne_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 09 */
START_TEST(vm, i64_ne_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 10 */
START_TEST(vm, i64_ne_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 11 */
START_TEST(vm, i64_ne_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 12 */
START_TEST(vm, i64_ne_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 13 */
START_TEST(vm, i64_ne_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* ne test 14 */
START_TEST(vm, i64_ne_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ne")
CHECK_I32(vm, 1)
END_TEST()

/* lt_s test 01 */
START_TEST(vm, i64_lt_s_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 02 */
START_TEST(vm, i64_lt_s_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 03 */
START_TEST(vm, i64_lt_s_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "lt_s")
CHECK_I32(vm, 1)
END_TEST()

/* lt_s test 04 */
START_TEST(vm, i64_lt_s_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 05 */
START_TEST(vm, i64_lt_s_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 06 */
START_TEST(vm, i64_lt_s_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 07 */
START_TEST(vm, i64_lt_s_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 08 */
START_TEST(vm, i64_lt_s_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "lt_s")
CHECK_I32(vm, 1)
END_TEST()

/* lt_s test 09 */
START_TEST(vm, i64_lt_s_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "lt_s")
CHECK_I32(vm, 1)
END_TEST()

/* lt_s test 10 */
START_TEST(vm, i64_lt_s_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 11 */
START_TEST(vm, i64_lt_s_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "lt_s")
CHECK_I32(vm, 1)
END_TEST()

/* lt_s test 12 */
START_TEST(vm, i64_lt_s_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_s test 13 */
START_TEST(vm, i64_lt_s_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "lt_s")
CHECK_I32(vm, 1)
END_TEST()

/* lt_s test 14 */
START_TEST(vm, i64_lt_s_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_s")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 01 */
START_TEST(vm, i64_lt_u_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 02 */
START_TEST(vm, i64_lt_u_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 03 */
START_TEST(vm, i64_lt_u_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 04 */
START_TEST(vm, i64_lt_u_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 05 */
START_TEST(vm, i64_lt_u_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 06 */
START_TEST(vm, i64_lt_u_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 07 */
START_TEST(vm, i64_lt_u_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 08 */
START_TEST(vm, i64_lt_u_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "lt_u")
CHECK_I32(vm, 1)
END_TEST()

/* lt_u test 09 */
START_TEST(vm, i64_lt_u_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 10 */
START_TEST(vm, i64_lt_u_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_u")
CHECK_I32(vm, 1)
END_TEST()

/* lt_u test 11 */
START_TEST(vm, i64_lt_u_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "lt_u")
CHECK_I32(vm, 1)
END_TEST()

/* lt_u test 12 */
START_TEST(vm, i64_lt_u_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 13 */
START_TEST(vm, i64_lt_u_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "lt_u")
CHECK_I32(vm, 0)
END_TEST()

/* lt_u test 14 */
START_TEST(vm, i64_lt_u_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "lt_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 01 */
START_TEST(vm, i64_le_s_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 02 */
START_TEST(vm, i64_le_s_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 03 */
START_TEST(vm, i64_le_s_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 04 */
START_TEST(vm, i64_le_s_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 05 */
START_TEST(vm, i64_le_s_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 06 */
START_TEST(vm, i64_le_s_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 07 */
START_TEST(vm, i64_le_s_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "le_s")
CHECK_I32(vm, 0)
END_TEST()

/* le_s test 08 */
START_TEST(vm, i64_le_s_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 09 */
START_TEST(vm, i64_le_s_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 10 */
START_TEST(vm, i64_le_s_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_s")
CHECK_I32(vm, 0)
END_TEST()

/* le_s test 11 */
START_TEST(vm, i64_le_s_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 12 */
START_TEST(vm, i64_le_s_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_s")
CHECK_I32(vm, 0)
END_TEST()

/* le_s test 13 */
START_TEST(vm, i64_le_s_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "le_s")
CHECK_I32(vm, 1)
END_TEST()

/* le_s test 14 */
START_TEST(vm, i64_le_s_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_s")
CHECK_I32(vm, 0)
END_TEST()

/* le_u test 01 */
START_TEST(vm, i64_le_u_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 02 */
START_TEST(vm, i64_le_u_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 03 */
START_TEST(vm, i64_le_u_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "le_u")
CHECK_I32(vm, 0)
END_TEST()

/* le_u test 04 */
START_TEST(vm, i64_le_u_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 05 */
START_TEST(vm, i64_le_u_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 06 */
START_TEST(vm, i64_le_u_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 07 */
START_TEST(vm, i64_le_u_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "le_u")
CHECK_I32(vm, 0)
END_TEST()

/* le_u test 08 */
START_TEST(vm, i64_le_u_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 09 */
START_TEST(vm, i64_le_u_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "le_u")
CHECK_I32(vm, 0)
END_TEST()

/* le_u test 10 */
START_TEST(vm, i64_le_u_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 11 */
START_TEST(vm, i64_le_u_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* le_u test 12 */
START_TEST(vm, i64_le_u_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_u")
CHECK_I32(vm, 0)
END_TEST()

/* le_u test 13 */
START_TEST(vm, i64_le_u_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "le_u")
CHECK_I32(vm, 0)
END_TEST()

/* le_u test 14 */
START_TEST(vm, i64_le_u_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "le_u")
CHECK_I32(vm, 1)
END_TEST()

/* gt_s test 01 */
START_TEST(vm, i64_gt_s_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 02 */
START_TEST(vm, i64_gt_s_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 03 */
START_TEST(vm, i64_gt_s_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 04 */
START_TEST(vm, i64_gt_s_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 05 */
START_TEST(vm, i64_gt_s_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 06 */
START_TEST(vm, i64_gt_s_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 07 */
START_TEST(vm, i64_gt_s_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "gt_s")
CHECK_I32(vm, 1)
END_TEST()

/* gt_s test 08 */
START_TEST(vm, i64_gt_s_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 09 */
START_TEST(vm, i64_gt_s_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 10 */
START_TEST(vm, i64_gt_s_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_s")
CHECK_I32(vm, 1)
END_TEST()

/* gt_s test 11 */
START_TEST(vm, i64_gt_s_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 12 */
START_TEST(vm, i64_gt_s_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_s")
CHECK_I32(vm, 1)
END_TEST()

/* gt_s test 13 */
START_TEST(vm, i64_gt_s_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "gt_s")
CHECK_I32(vm, 0)
END_TEST()

/* gt_s test 14 */
START_TEST(vm, i64_gt_s_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_s")
CHECK_I32(vm, 1)
END_TEST()

/* gt_u test 01 */
START_TEST(vm, i64_gt_u_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 02 */
START_TEST(vm, i64_gt_u_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 03 */
START_TEST(vm, i64_gt_u_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "gt_u")
CHECK_I32(vm, 1)
END_TEST()

/* gt_u test 04 */
START_TEST(vm, i64_gt_u_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 05 */
START_TEST(vm, i64_gt_u_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 06 */
START_TEST(vm, i64_gt_u_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 07 */
START_TEST(vm, i64_gt_u_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "gt_u")
CHECK_I32(vm, 1)
END_TEST()

/* gt_u test 08 */
START_TEST(vm, i64_gt_u_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 09 */
START_TEST(vm, i64_gt_u_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "gt_u")
CHECK_I32(vm, 1)
END_TEST()

/* gt_u test 10 */
START_TEST(vm, i64_gt_u_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 11 */
START_TEST(vm, i64_gt_u_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* gt_u test 12 */
START_TEST(vm, i64_gt_u_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_u")
CHECK_I32(vm, 1)
END_TEST()

/* gt_u test 13 */
START_TEST(vm, i64_gt_u_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "gt_u")
CHECK_I32(vm, 1)
END_TEST()

/* gt_u test 14 */
START_TEST(vm, i64_gt_u_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "gt_u")
CHECK_I32(vm, 0)
END_TEST()

/* ge_s test 01 */
START_TEST(vm, i64_ge_s_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 02 */
START_TEST(vm, i64_ge_s_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 03 */
START_TEST(vm, i64_ge_s_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "ge_s")
CHECK_I32(vm, 0)
END_TEST()

/* ge_s test 04 */
START_TEST(vm, i64_ge_s_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 05 */
START_TEST(vm, i64_ge_s_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 06 */
START_TEST(vm, i64_ge_s_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 07 */
START_TEST(vm, i64_ge_s_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 08 */
START_TEST(vm, i64_ge_s_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "ge_s")
CHECK_I32(vm, 0)
END_TEST()

/* ge_s test 09 */
START_TEST(vm, i64_ge_s_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "ge_s")
CHECK_I32(vm, 0)
END_TEST()

/* ge_s test 10 */
START_TEST(vm, i64_ge_s_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 11 */
START_TEST(vm, i64_ge_s_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "ge_s")
CHECK_I32(vm, 0)
END_TEST()

/* ge_s test 12 */
START_TEST(vm, i64_ge_s_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_s test 13 */
START_TEST(vm, i64_ge_s_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ge_s")
CHECK_I32(vm, 0)
END_TEST()

/* ge_s test 14 */
START_TEST(vm, i64_ge_s_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_s")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 01 */
START_TEST(vm, i64_ge_u_test_01)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 02 */
START_TEST(vm, i64_ge_u_test_02)
PUSH_I64(vm, 1)
PUSH_I64(vm, 1)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 03 */
START_TEST(vm, i64_ge_u_test_03)
PUSH_I64(vm, -1)
PUSH_I64(vm, 1)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 04 */
START_TEST(vm, i64_ge_u_test_04)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 05 */
START_TEST(vm, i64_ge_u_test_05)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 06 */
START_TEST(vm, i64_ge_u_test_06)
PUSH_I64(vm, -1)
PUSH_I64(vm, -1)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 07 */
START_TEST(vm, i64_ge_u_test_07)
PUSH_I64(vm, 1)
PUSH_I64(vm, 0)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 08 */
START_TEST(vm, i64_ge_u_test_08)
PUSH_I64(vm, 0)
PUSH_I64(vm, 1)
CALL(vm, "ge_u")
CHECK_I32(vm, 0)
END_TEST()

/* ge_u test 09 */
START_TEST(vm, i64_ge_u_test_09)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 10 */
START_TEST(vm, i64_ge_u_test_10)
PUSH_I64(vm, 0)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_u")
CHECK_I32(vm, 0)
END_TEST()

/* ge_u test 11 */
START_TEST(vm, i64_ge_u_test_11)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, -1)
CALL(vm, "ge_u")
CHECK_I32(vm, 0)
END_TEST()

/* ge_u test 12 */
START_TEST(vm, i64_ge_u_test_12)
PUSH_I64(vm, -1)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 13 */
START_TEST(vm, i64_ge_u_test_13)
PUSH_I64(vm, 0x8000000000000000)
PUSH_I64(vm, 0x7fffffffffffffff)
CALL(vm, "ge_u")
CHECK_I32(vm, 1)
END_TEST()

/* ge_u test 14 */
START_TEST(vm, i64_ge_u_test_14)
PUSH_I64(vm, 0x7fffffffffffffff)
PUSH_I64(vm, 0x8000000000000000)
CALL(vm, "ge_u")
CHECK_I32(vm, 0)
END_TEST()

void run_i64_tests(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    LOAD_MODULE(vm, dir, path_buf, path_buf_sz, "i64.wasm");

    //run add tests
    RUN_TEST(vm, i64_add_test_01, *passed, *failed);
    RUN_TEST(vm, i64_add_test_02, *passed, *failed);
    RUN_TEST(vm, i64_add_test_03, *passed, *failed);
    RUN_TEST(vm, i64_add_test_04, *passed, *failed);
    RUN_TEST(vm, i64_add_test_05, *passed, *failed);
    RUN_TEST(vm, i64_add_test_06, *passed, *failed);
    RUN_TEST(vm, i64_add_test_07, *passed, *failed);
    RUN_TEST(vm, i64_add_test_08, *passed, *failed);

    //run sub tests
    RUN_TEST(vm, i64_sub_test_01, *passed, *failed);
    RUN_TEST(vm, i64_sub_test_02, *passed, *failed);
    RUN_TEST(vm, i64_sub_test_03, *passed, *failed);
    RUN_TEST(vm, i64_sub_test_04, *passed, *failed);
    RUN_TEST(vm, i64_sub_test_05, *passed, *failed);
    RUN_TEST(vm, i64_sub_test_06, *passed, *failed);
    RUN_TEST(vm, i64_sub_test_07, *passed, *failed);

    //run mul tests
    RUN_TEST(vm, i64_mul_test_01, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_02, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_03, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_04, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_05, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_06, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_07, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_08, *passed, *failed);
    RUN_TEST(vm, i64_mul_test_09, *passed, *failed);

    // //run div_s tests
    RUN_TEST(vm, i64_div_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_14, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_15, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_16, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_17, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_18, *passed, *failed);
    RUN_TEST(vm, i64_div_s_test_19, *passed, *failed);

    // //run div_u tests
    RUN_TEST(vm, i64_div_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_14, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_15, *passed, *failed);
    RUN_TEST(vm, i64_div_u_test_16, *passed, *failed);

    // //run rem_s tests
    RUN_TEST(vm, i64_rem_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_14, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_15, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_16, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_17, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_18, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_19, *passed, *failed);
    RUN_TEST(vm, i64_rem_s_test_20, *passed, *failed);

    //run rem_u tests
    RUN_TEST(vm, i64_rem_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_14, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_15, *passed, *failed);
    RUN_TEST(vm, i64_rem_u_test_16, *passed, *failed);

    // //run and tests
    RUN_TEST(vm, i64_and_test_01, *passed, *failed);
    RUN_TEST(vm, i64_and_test_02, *passed, *failed);
    RUN_TEST(vm, i64_and_test_03, *passed, *failed);
    RUN_TEST(vm, i64_and_test_04, *passed, *failed);
    RUN_TEST(vm, i64_and_test_05, *passed, *failed);
    RUN_TEST(vm, i64_and_test_06, *passed, *failed);
    RUN_TEST(vm, i64_and_test_07, *passed, *failed);
    RUN_TEST(vm, i64_and_test_08, *passed, *failed);

    //run or tests
    RUN_TEST(vm, i64_or_test_01, *passed, *failed);
    RUN_TEST(vm, i64_or_test_02, *passed, *failed);
    RUN_TEST(vm, i64_or_test_03, *passed, *failed);
    RUN_TEST(vm, i64_or_test_04, *passed, *failed);
    RUN_TEST(vm, i64_or_test_05, *passed, *failed);
    RUN_TEST(vm, i64_or_test_06, *passed, *failed);
    RUN_TEST(vm, i64_or_test_07, *passed, *failed);
    RUN_TEST(vm, i64_or_test_08, *passed, *failed);

    //run xor tests
    RUN_TEST(vm, i64_xor_test_01, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_02, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_03, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_04, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_05, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_06, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_07, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_08, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_09, *passed, *failed);
    RUN_TEST(vm, i64_xor_test_10, *passed, *failed);

    //run shl tests
    RUN_TEST(vm, i64_shl_test_01, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_02, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_03, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_04, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_05, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_06, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_07, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_08, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_09, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_10, *passed, *failed);
    RUN_TEST(vm, i64_shl_test_11, *passed, *failed);

    //run shr_s tests
    RUN_TEST(vm, i64_shr_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_14, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_15, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_16, *passed, *failed);
    RUN_TEST(vm, i64_shr_s_test_17, *passed, *failed);

    //run shr_u tests
    RUN_TEST(vm, i64_shr_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_14, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_15, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_16, *passed, *failed);
    RUN_TEST(vm, i64_shr_u_test_17, *passed, *failed);

    //run rotl tests
    RUN_TEST(vm, i64_rotl_test_01, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_02, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_03, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_04, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_05, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_06, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_07, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_08, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_09, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_10, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_11, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_12, *passed, *failed);
    RUN_TEST(vm, i64_rotl_test_13, *passed, *failed);

    //run rotr tests
    RUN_TEST(vm, i64_rotr_test_01, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_02, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_03, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_04, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_05, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_06, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_07, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_08, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_09, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_10, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_11, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_12, *passed, *failed);
    RUN_TEST(vm, i64_rotr_test_13, *passed, *failed);

    //run clz tests
    RUN_TEST(vm, i64_clz_test_01, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_02, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_03, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_04, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_05, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_06, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_07, *passed, *failed);
    RUN_TEST(vm, i64_clz_test_08, *passed, *failed);

    //run ctz tests
    RUN_TEST(vm, i64_ctz_test_01, *passed, *failed);
    RUN_TEST(vm, i64_ctz_test_02, *passed, *failed);
    RUN_TEST(vm, i64_ctz_test_03, *passed, *failed);
    RUN_TEST(vm, i64_ctz_test_04, *passed, *failed);
    RUN_TEST(vm, i64_ctz_test_05, *passed, *failed);
    RUN_TEST(vm, i64_ctz_test_06, *passed, *failed);

    //run popcnt tests
    RUN_TEST(vm, i64_popcnt_test_01, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_02, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_03, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_04, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_05, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_06, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_07, *passed, *failed);
    RUN_TEST(vm, i64_popcnt_test_08, *passed, *failed);

    //run eqz tests
    RUN_TEST(vm, i64_eqz_test_01, *passed, *failed);
    RUN_TEST(vm, i64_eqz_test_02, *passed, *failed);
    RUN_TEST(vm, i64_eqz_test_03, *passed, *failed);
    RUN_TEST(vm, i64_eqz_test_04, *passed, *failed);
    RUN_TEST(vm, i64_eqz_test_05, *passed, *failed);

    //run eq tests
    RUN_TEST(vm, i64_eq_test_01, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_02, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_03, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_04, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_05, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_06, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_07, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_08, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_09, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_10, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_11, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_12, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_13, *passed, *failed);
    RUN_TEST(vm, i64_eq_test_14, *passed, *failed);

    //run ne tests
    RUN_TEST(vm, i64_ne_test_01, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_02, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_03, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_04, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_05, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_06, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_07, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_08, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_09, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_10, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_11, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_12, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_13, *passed, *failed);
    RUN_TEST(vm, i64_ne_test_14, *passed, *failed);

    //run lt_s tests
    RUN_TEST(vm, i64_lt_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_lt_s_test_14, *passed, *failed);

    //run lt_u tests
    RUN_TEST(vm, i64_lt_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_lt_u_test_14, *passed, *failed);

    //run le_s tests
    RUN_TEST(vm, i64_le_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_le_s_test_14, *passed, *failed);

    //run le_u tests
    RUN_TEST(vm, i64_le_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_le_u_test_14, *passed, *failed);

    //run gt_s tests
    RUN_TEST(vm, i64_gt_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_gt_s_test_14, *passed, *failed);

    //run gt_u tests
    RUN_TEST(vm, i64_gt_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_gt_u_test_14, *passed, *failed);

    //run ge_s tests
    RUN_TEST(vm, i64_ge_s_test_01, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_02, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_03, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_04, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_05, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_06, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_07, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_08, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_09, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_10, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_11, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_12, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_13, *passed, *failed);
    RUN_TEST(vm, i64_ge_s_test_14, *passed, *failed);

    //run ge_u tests
    RUN_TEST(vm, i64_ge_u_test_01, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_02, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_03, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_04, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_05, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_06, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_07, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_08, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_09, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_10, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_11, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_12, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_13, *passed, *failed);
    RUN_TEST(vm, i64_ge_u_test_14, *passed, *failed);

    UNLOAD_MODULE(vm);
}
