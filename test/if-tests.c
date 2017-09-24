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
#include "test-common.h"
#include "test-builder.h"

/* empty test 01 */
START_TEST(vm, if_empty_test_01)
PUSH_I32(vm, 0)
CALL(vm, "empty")
CHECK_STK_EMPTY(vm)
END_TEST()

/* empty test 02 */
START_TEST(vm, if_empty_test_02)
PUSH_I32(vm, 1)
CALL(vm, "empty")
CHECK_STK_EMPTY(vm)
END_TEST()

/* empty test 03 */
START_TEST(vm, if_empty_test_03)
PUSH_I32(vm, 100)
CALL(vm, "empty")
CHECK_STK_EMPTY(vm)
END_TEST()

/* empty test 04 */
START_TEST(vm, if_empty_test_04)
PUSH_I32(vm, -2)
CALL(vm, "empty")
CHECK_STK_EMPTY(vm)
END_TEST()

/* singular test 01 */
START_TEST(vm, if_singular_test_01)
PUSH_I32(vm, 0)
CALL(vm, "singular")
CHECK_I32(vm, 8)
END_TEST()

/* singular test 02 */
START_TEST(vm, if_singular_test_02)
PUSH_I32(vm, 1)
CALL(vm, "singular")
CHECK_I32(vm, 7)
END_TEST()

/* singular test 03 */
START_TEST(vm, if_singular_test_03)
PUSH_I32(vm, 10)
CALL(vm, "singular")
CHECK_I32(vm, 7)
END_TEST()

/* singular test 04 */
START_TEST(vm, if_singular_test_04)
PUSH_I32(vm, -10)
CALL(vm, "singular")
CHECK_I32(vm, 7)
END_TEST()

/* multi test 01 */
START_TEST(vm, if_multi_test_01)
PUSH_I32(vm, 0)
CALL(vm, "multi")
CHECK_I32(vm, 9)
END_TEST()

/* multi test 02 */
START_TEST(vm, if_multi_test_02)
PUSH_I32(vm, 1)
CALL(vm, "multi")
CHECK_I32(vm, 8)
END_TEST()

/* multi test 03 */
START_TEST(vm, if_multi_test_03)
PUSH_I32(vm, 13)
CALL(vm, "multi")
CHECK_I32(vm, 8)
END_TEST()

/* multi test 04 */
START_TEST(vm, if_multi_test_04)
PUSH_I32(vm, -5)
CALL(vm, "multi")
CHECK_I32(vm, 8)
END_TEST()

/* nested test 01 */
START_TEST(vm, if_nested_test_01)
PUSH_I32(vm, 0)
PUSH_I32(vm, 0)
CALL(vm, "nested")
CHECK_I32(vm, 11)
END_TEST()

/* nested test 02 */
START_TEST(vm, if_nested_test_02)
PUSH_I32(vm, 1)
PUSH_I32(vm, 0)
CALL(vm, "nested")
CHECK_I32(vm, 10)
END_TEST()

/* nested test 03 */
START_TEST(vm, if_nested_test_03)
PUSH_I32(vm, 0)
PUSH_I32(vm, 1)
CALL(vm, "nested")
CHECK_I32(vm, 10)
END_TEST()

/* nested test 04 */
START_TEST(vm, if_nested_test_04)
PUSH_I32(vm, 3)
PUSH_I32(vm, 2)
CALL(vm, "nested")
CHECK_I32(vm, 9)
END_TEST()

/* nested test 05 */
START_TEST(vm, if_nested_test_05)
PUSH_I32(vm, 0)
PUSH_I32(vm, -100)
CALL(vm, "nested")
CHECK_I32(vm, 10)
END_TEST()

/* nested test 06 */
START_TEST(vm, if_nested_test_06)
PUSH_I32(vm, 10)
PUSH_I32(vm, 10)
CALL(vm, "nested")
CHECK_I32(vm, 9)
END_TEST()

/* nested test 07 */
START_TEST(vm, if_nested_test_07)
PUSH_I32(vm, 0)
PUSH_I32(vm, -1)
CALL(vm, "nested")
CHECK_I32(vm, 10)
END_TEST()

/* nested test 08 */
START_TEST(vm, if_nested_test_08)
PUSH_I32(vm, -111)
PUSH_I32(vm, -2)
CALL(vm, "nested")
CHECK_I32(vm, 9)
END_TEST()

/* as-unary-operand test 01 */
START_TEST(vm, if_as_unary_operand_test_01)
PUSH_I32(vm, 0)
CALL(vm, "as-unary-operand")
CHECK_I32(vm, 0)
END_TEST()

/* as-unary-operand test 02 */
START_TEST(vm, if_as_unary_operand_test_02)
PUSH_I32(vm, 1)
CALL(vm, "as-unary-operand")
CHECK_I32(vm, 0)
END_TEST()

/* as-unary-operand test 03 */
START_TEST(vm, if_as_unary_operand_test_03)
PUSH_I32(vm, -1)
CALL(vm, "as-unary-operand")
CHECK_I32(vm, 0)
END_TEST()

/* as-binary-operand test 01 */
START_TEST(vm, if_as_binary_operand_test_01)
PUSH_I32(vm, 0)
PUSH_I32(vm, 0)
CALL(vm, "as-binary-operand")
CHECK_I32(vm, 15)
END_TEST()

/* as-binary-operand test 02 */
START_TEST(vm, if_as_binary_operand_test_02)
PUSH_I32(vm, 0)
PUSH_I32(vm, 1)
CALL(vm, "as-binary-operand")
CHECK_I32(vm, -12)
END_TEST()

/* as-binary-operand test 03 */
START_TEST(vm, if_as_binary_operand_test_03)
PUSH_I32(vm, 1)
PUSH_I32(vm, 0)
CALL(vm, "as-binary-operand")
CHECK_I32(vm, -15)
END_TEST()

/* as-binary-operand test 04 */
START_TEST(vm, if_as_binary_operand_test_04)
PUSH_I32(vm, 1)
PUSH_I32(vm, 1)
CALL(vm, "as-binary-operand")
CHECK_I32(vm, 12)
END_TEST()

/* as-test-operand test 01 */
START_TEST(vm, if_as_test_operand_test_01)
PUSH_I32(vm, 0)
CALL(vm, "as-test-operand")
CHECK_I32(vm, 1)
END_TEST()

/* as-test-operand test 02 */
START_TEST(vm, if_as_test_operand_test_02)
PUSH_I32(vm, 1)
CALL(vm, "as-test-operand")
CHECK_I32(vm, 0)
END_TEST()

/* as-compare-operand test 01 */
START_TEST(vm, if_as_compare_operand_test_01)
PUSH_I32(vm, 0)
PUSH_I32(vm, 0)
CALL(vm, "as-compare-operand")
CHECK_I32(vm, 1)
END_TEST()

/* as-compare-operand test 02 */
START_TEST(vm, if_as_compare_operand_test_02)
PUSH_I32(vm, 0)
PUSH_I32(vm, 1)
CALL(vm, "as-compare-operand")
CHECK_I32(vm, 0)
END_TEST()

/* as-compare-operand test 03 */
START_TEST(vm, if_as_compare_operand_test_03)
PUSH_I32(vm, 1)
PUSH_I32(vm, 0)
CALL(vm, "as-compare-operand")
CHECK_I32(vm, 1)
END_TEST()

/* as-compare-operand test 04 */
START_TEST(vm, if_as_compare_operand_test_04)
PUSH_I32(vm, 1)
PUSH_I32(vm, 1)
CALL(vm, "as-compare-operand")
CHECK_I32(vm, 0)
END_TEST()

/* break-bare test 01 */
START_TEST(vm, if_break_bare_test_01)
CALL(vm, "break-bare")
CHECK_I32(vm, 19)
END_TEST()

/* break-value test 01 */
START_TEST(vm, if_break_value_test_01)
PUSH_I32(vm, 1)
CALL(vm, "break-value")
CHECK_I32(vm, 18)
END_TEST()

/* break-value test 02 */
START_TEST(vm, if_break_value_test_02)
PUSH_I32(vm, 0)
CALL(vm, "break-value")
CHECK_I32(vm, 21)
END_TEST()

/* effects test 01 */
START_TEST(vm, if_effects_test_01)
PUSH_I32(vm, 1)
CALL(vm, "effects")
CHECK_I32(vm, -14)
END_TEST()

/* effects test 02 */
START_TEST(vm, if_effects_test_02)
PUSH_I32(vm, 0)
CALL(vm, "effects")
CHECK_I32(vm, -6)
END_TEST()

void run_if_tests(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    LOAD_MODULE(vm, dir, path_buf, path_buf_sz, "if.wasm");

    //run empty tests
    RUN_TEST(vm, if_empty_test_01, *passed, *failed);
    RUN_TEST(vm, if_empty_test_02, *passed, *failed);
    RUN_TEST(vm, if_empty_test_03, *passed, *failed);
    RUN_TEST(vm, if_empty_test_04, *passed, *failed);

    //run singular tests
    RUN_TEST(vm, if_singular_test_01, *passed, *failed);
    RUN_TEST(vm, if_singular_test_02, *passed, *failed);
    RUN_TEST(vm, if_singular_test_03, *passed, *failed);
    RUN_TEST(vm, if_singular_test_04, *passed, *failed);

    //run multi tests
    RUN_TEST(vm, if_multi_test_01, *passed, *failed);
    RUN_TEST(vm, if_multi_test_02, *passed, *failed);
    RUN_TEST(vm, if_multi_test_03, *passed, *failed);
    RUN_TEST(vm, if_multi_test_04, *passed, *failed);

    //run nested tests
    RUN_TEST(vm, if_nested_test_01, *passed, *failed);
    RUN_TEST(vm, if_nested_test_02, *passed, *failed);
    RUN_TEST(vm, if_nested_test_03, *passed, *failed);
    RUN_TEST(vm, if_nested_test_04, *passed, *failed);
    RUN_TEST(vm, if_nested_test_05, *passed, *failed);
    RUN_TEST(vm, if_nested_test_06, *passed, *failed);
    RUN_TEST(vm, if_nested_test_07, *passed, *failed);
    RUN_TEST(vm, if_nested_test_08, *passed, *failed);

    //run as-unary-operand tests
    RUN_TEST(vm, if_as_unary_operand_test_01, *passed, *failed);
    RUN_TEST(vm, if_as_unary_operand_test_02, *passed, *failed);
    RUN_TEST(vm, if_as_unary_operand_test_03, *passed, *failed);

    //run as-binary-operand tests
    RUN_TEST(vm, if_as_binary_operand_test_01, *passed, *failed);
    RUN_TEST(vm, if_as_binary_operand_test_02, *passed, *failed);
    RUN_TEST(vm, if_as_binary_operand_test_03, *passed, *failed);
    RUN_TEST(vm, if_as_binary_operand_test_04, *passed, *failed);

    //run as-test-operand tests
    RUN_TEST(vm, if_as_test_operand_test_01, *passed, *failed);
    RUN_TEST(vm, if_as_test_operand_test_02, *passed, *failed);

    //run as-compare-operand tests
    RUN_TEST(vm, if_as_compare_operand_test_01, *passed, *failed);
    RUN_TEST(vm, if_as_compare_operand_test_02, *passed, *failed);

    //run break-bare tests
    RUN_TEST(vm, if_break_bare_test_01, *passed, *failed);

    //run break-value tests
    RUN_TEST(vm, if_break_value_test_01, *passed, *failed);
    RUN_TEST(vm, if_break_value_test_02, *passed, *failed);

    //run effects tests
    RUN_TEST(vm, if_effects_test_01, *passed, *failed);
    RUN_TEST(vm, if_effects_test_02, *passed, *failed);

    UNLOAD_MODULE(vm);
}
