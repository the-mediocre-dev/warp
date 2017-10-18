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

#pragma once

#define TEST_MODULE(vm, dir, path_buf, path_buf_sz, mdle_name, result, passed, failed)        \
    {                                                                                         \
        wrp_err_t err = validate_mdle(vm, dir, path_buf, path_buf_sz, mdle_name);             \
                                                                                              \
        if (err == result) {                                                                  \
            passed++;                                                                         \
            printf("module test %s passed\n", mdle_name);                                     \
        } else {                                                                              \
            failed++;                                                                         \
            const char *format = "module test %s failed. Expected: \"%s\", Actual: \"%s\"\n"; \
            const char *result_name = wrp_debug_err(result);                                  \
            const char *err_name = wrp_debug_err(err);                                        \
            printf(format, mdle_name, result_name, err_name);                                 \
        }                                                                                     \
    }

#define TEST_LINK(vm, dir, path_buf, path_buf_sz, mdle_name, result, passed, failed)          \
    {                                                                                         \
        wrp_err_t err = link_mdle(vm, dir, path_buf, path_buf_sz, mdle_name);                 \
                                                                                              \
        if (err == result) {                                                                  \
            passed++;                                                                         \
            printf("module test %s passed\n", mdle_name);                                     \
        } else {                                                                              \
            failed++;                                                                         \
            const char *format = "module test %s failed. Expected: \"%s\", Actual: \"%s\"\n"; \
            const char *result_name = wrp_debug_err(result);                                  \
            const char *err_name = wrp_debug_err(err);                                        \
            printf(format, mdle_name, result_name, err_name);                                 \
        }                                                                                     \
    }

#define START_FUNC_TESTS(vm, func)                                             \
    {                                                                          \
        const char *func_name = func;                                          \
        printf("running tests for function \"%s\"\n", func_name);              \
                                                                               \
        uint32_t func_idx = 0;                                                 \
        if (wrp_export_func(vm->mdle, func_name, &func_idx) != WRP_SUCCESS) {  \
            ASSERT(false, "function %s does not exist in module!", func_name); \
        }                                                                      \
                                                                               \
        uint32_t num_func_tests = 0;                                           \
        uint32_t passed_func_tests = 0;                                        \
        uint32_t failed_func_tests = 0;

#define START_TEST(vm)    \
    {                     \
        num_func_tests++; \
        wrp_reset_vm(vm); \
        bool success = true;

#define PUSH_I32(vm, value)                                           \
                                                                      \
    if (success && wrp_stk_exec_push_i32(vm, value) != WRP_SUCCESS) { \
        success = false;                                              \
    }

#define PUSH_I64(vm, value)                                           \
                                                                      \
    if (success && wrp_stk_exec_push_i64(vm, value) != WRP_SUCCESS) { \
        success = false;                                              \
    }

#define PUSH_F32(vm, value)                                           \
                                                                      \
    if (success && wrp_stk_exec_push_f32(vm, value) != WRP_SUCCESS) { \
        success = false;                                              \
    }

#define PUSH_F64(vm, value)                                           \
                                                                      \
    if (success && wrp_stk_exec_push_f64(vm, value) != WRP_SUCCESS) { \
        success = false;                                              \
    }

#define CALL(vm)                                            \
                                                            \
    if (success && wrp_call(vm, func_idx) != WRP_SUCCESS) { \
        success = false;                                    \
    }

#define CALL_AND_TRAP(vm, expected_err)                     \
                                                            \
    if (success && wrp_call(vm, func_idx) == WRP_SUCCESS) { \
        success = false;                                    \
    }                                                       \
                                                            \
    if (success && vm->err != expected_err) {               \
        success = false;                                    \
    }

#define POP_I32(vm, result)                                           \
                                                                      \
    int32_t value = 0;                                                \
    if (success && wrp_stk_exec_pop_i32(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && value != (int32_t)result) {                        \
        success = false;                                              \
    }

#define POP_I64(vm, result)                                           \
                                                                      \
    int64_t value = 0;                                                \
    if (success && wrp_stk_exec_pop_i64(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && value != (int64_t)result) {                        \
        success = false;                                              \
    }

#define POP_F32(vm, result)                                           \
                                                                      \
    float value = 0;                                                  \
    if (success && wrp_stk_exec_pop_f32(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && value != result) {                                 \
        success = false;                                              \
    }

#define POP_F32_BITWISE(vm, result)                                   \
                                                                      \
    float value = 0;                                                  \
    if (success && wrp_stk_exec_pop_f32(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success) {                                                    \
        float result_value = result;                                  \
        uint32_t int_value = *((uint32_t *)&value);                   \
        uint32_t int_result = *((uint32_t *)&result_value);           \
                                                                      \
        if (int_value != int_result) {                                \
            success = false;                                          \
        }                                                             \
    }

#define POP_F32_ARITHMETIC_NAN(vm)                                    \
                                                                      \
    float value = 0;                                                  \
    if (success && wrp_stk_exec_pop_f32(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && !isnan(value)) {                                   \
        success = false;                                              \
    }

#define POP_F32_CANONICAL_NAN(vm)                                     \
                                                                      \
    float value = 0;                                                  \
    if (success && wrp_stk_exec_pop_f32(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && !isnan(value)) {                                   \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success) {                                                    \
        uint32_t int_value = *((uint32_t *)&value);                   \
                                                                      \
        if ((int_value & 0x400000) == 0) {                            \
            success = false;                                          \
        }                                                             \
                                                                      \
        if ((int_value | 0x3FFFFF) == 1) {                            \
            success = false;                                          \
        }                                                             \
    }

#define POP_F64(vm, result)                                           \
                                                                      \
    double value = 0;                                                 \
    if (success && wrp_stk_exec_pop_f64(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && value != result) {                                 \
        success = false;                                              \
    }

#define POP_F64_BITWISE(vm, result)                                   \
                                                                      \
    double value = 0;                                                 \
    if (success && wrp_stk_exec_pop_f64(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success) {                                                    \
        double result_value = result;                                 \
        uint64_t int_value = *((uint64_t *)&value);                   \
        uint64_t int_result = *((uint64_t *)&result_value);           \
                                                                      \
        if (int_value != int_result) {                                \
            success = false;                                          \
        }                                                             \
    }

#define POP_F64_ARITHMETIC_NAN(vm)                                    \
                                                                      \
    double value = 0;                                                 \
    if (success && wrp_stk_exec_pop_f64(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && !isnan(value)) {                                   \
        success = false;                                              \
    }

#define POP_F64_CANONICAL_NAN(vm)                                     \
                                                                      \
    double value = 0;                                                 \
    if (success && wrp_stk_exec_pop_f64(vm, &value) != WRP_SUCCESS) { \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success && !isnan(value)) {                                   \
        success = false;                                              \
    }                                                                 \
                                                                      \
    if (success) {                                                    \
        uint64_t int_value = *((uint64_t *)&value);                   \
                                                                      \
        if ((int_value & 0x8000000000000) == 0) {                     \
            success = false;                                          \
        }                                                             \
                                                                      \
        if ((int_value | 0x7FFFFFFFFFFFF) == 1) {                     \
            success = false;                                          \
        }                                                             \
    }

#define END_TEST()                                                \
    if (success) {                                                \
        passed_func_tests++;                                      \
    } else {                                                      \
        failed_func_tests++;                                      \
        printf("%s test %d failed\n", func_name, num_func_tests); \
    }                                                             \
    }

#define END_FUNC_TESTS(passed, failed) \
                                       \
    passed += passed_func_tests;       \
    failed += failed_func_tests;       \
    printf("done\n");                  \
    }

#define TEST_EMPTY(vm) \
    START_TEST(vm)     \
    CALL(vm)           \
    END_TEST()

#define TEST_IN_I32(vm, param_1) \
    START_TEST(vm)               \
    PUSH_I32(vm, param_1)        \
    CALL(vm)                     \
    END_TEST()

#define TEST_IN_I32_TRAP(vm, param_1, err) \
    START_TEST(vm)                         \
    PUSH_I32(vm, param_1)                  \
    CALL_AND_TRAP(vm, err)                 \
    END_TEST()

#define TEST_IN_I32_I32(vm, param_1, param_2) \
    START_TEST(vm)                            \
    PUSH_I32(vm, param_1)                     \
    PUSH_I32(vm, param_2)                     \
    CALL(vm)                                  \
    END_TEST()

#define TEST_OUT_I32(vm, result) \
    START_TEST(vm)               \
    CALL(vm)                     \
    POP_I32(vm, result)          \
    END_TEST()

#define TEST_IN_I32_OUT_I32(vm, param_1, result) \
    START_TEST(vm)                               \
    PUSH_I32(vm, param_1)                        \
    CALL(vm)                                     \
    POP_I32(vm, result)                          \
    END_TEST()

#define TEST_IN_I32_I32_OUT_I32(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_I32(vm, param_1)                                     \
    PUSH_I32(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_I32(vm, result)                                       \
    END_TEST()

#define TEST_IN_I32_I32_I32_OUT_I32(vm, param_1, param_2, param_3, result) \
    START_TEST(vm)                                                         \
    PUSH_I32(vm, param_1)                                                  \
    PUSH_I32(vm, param_2)                                                  \
    PUSH_I32(vm, param_3)                                                  \
    CALL(vm)                                                               \
    POP_I32(vm, result)                                                    \
    END_TEST()

#define TEST_IN_I32_I32_TRAP(vm, param_1, param_2, err) \
    START_TEST(vm)                                      \
    PUSH_I32(vm, param_1)                               \
    PUSH_I32(vm, param_2)                               \
    CALL_AND_TRAP(vm, err)                              \
    END_TEST()

#define TEST_IN_I32_OUT_I64(vm, param_1, result) \
    START_TEST(vm)                               \
    PUSH_I32(vm, param_1)                        \
    CALL(vm)                                     \
    POP_I64(vm, result)                          \
    END_TEST()

#define TEST_IN_I64_OUT_I64(vm, param_1, result) \
    START_TEST(vm)                               \
    PUSH_I64(vm, param_1)                        \
    CALL(vm)                                     \
    POP_I64(vm, result)                          \
    END_TEST()

#define TEST_IN_I64_OUT_I32(vm, param_1, result) \
    START_TEST(vm)                               \
    PUSH_I64(vm, param_1)                        \
    CALL(vm)                                     \
    POP_I32(vm, result)                          \
    END_TEST()

#define TEST_IN_I64_I64_OUT_I64(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_I64(vm, param_1)                                     \
    PUSH_I64(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_I64(vm, result)                                       \
    END_TEST()

#define TEST_IN_I32_I64_TRAP(vm, param_1, param_2, err) \
    START_TEST(vm)                                      \
    PUSH_I32(vm, param_1)                               \
    PUSH_I64(vm, param_2)                               \
    CALL_AND_TRAP(vm, err)                              \
    END_TEST()

#define TEST_IN_I64_I64_TRAP(vm, param_1, param_2, err) \
    START_TEST(vm)                                      \
    PUSH_I64(vm, param_1)                               \
    PUSH_I64(vm, param_2)                               \
    CALL_AND_TRAP(vm, err)                              \
    END_TEST()

#define TEST_IN_I64_I64_OUT_I32(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_I64(vm, param_1)                                     \
    PUSH_I64(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_I32(vm, result)                                       \
    END_TEST()

#define TEST_OUT_F32(vm, result) \
    START_TEST(vm)               \
    CALL(vm)                     \
    POP_F32(vm, result)          \
    END_TEST()

#define TEST_IN_F32_OUT_F32(vm, param_1, result) \
    START_TEST(vm)                               \
    PUSH_F32(vm, param_1)                        \
    CALL(vm)                                     \
    POP_F32(vm, result)                          \
    END_TEST()

#define TEST_IN_F32_OUT_F32_BITWISE(vm, param_1, result) \
    START_TEST(vm)                                       \
    PUSH_F32(vm, param_1)                                \
    CALL(vm)                                             \
    POP_F32_BITWISE(vm, result)                          \
    END_TEST()

#define TEST_IN_F32_OUT_ARITHMETIC_NAN(vm, param_1) \
    START_TEST(vm)                                  \
    PUSH_F32(vm, param_1)                           \
    CALL(vm)                                        \
    POP_F32_ARITHMETIC_NAN(vm)                      \
    END_TEST()

#define TEST_IN_F32_OUT_CANONICAL_NAN(vm, param_1) \
    START_TEST(vm)                                 \
    PUSH_F32(vm, param_1)                          \
    CALL(vm)                                       \
    POP_F32_CANONICAL_NAN(vm)                      \
    END_TEST()

#define TEST_IN_F32_F32_OUT_F32(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_F32(vm, param_1)                                     \
    PUSH_F32(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_F32(vm, result)                                       \
    END_TEST()

#define TEST_IN_F32_F32_OUT_F32_BITWISE(vm, param_1, param_2, result) \
    START_TEST(vm)                                                    \
    PUSH_F32(vm, param_1)                                             \
    PUSH_F32(vm, param_2)                                             \
    CALL(vm)                                                          \
    POP_F32_BITWISE(vm, result)                                       \
    END_TEST()

#define TEST_IN_F32_F32_OUT_I32(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_F32(vm, param_1)                                     \
    PUSH_F32(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_I32(vm, result)                                       \
    END_TEST()

#define TEST_IN_F32_F32_OUT_ARITHMETIC_NAN(vm, param_1, param_2) \
    START_TEST(vm)                                               \
    PUSH_F32(vm, param_1)                                        \
    PUSH_F32(vm, param_2)                                        \
    CALL(vm)                                                     \
    POP_F32_ARITHMETIC_NAN(vm)                                   \
    END_TEST()

#define TEST_IN_F32_F32_OUT_CANONICAL_NAN(vm, param_1, param_2) \
    START_TEST(vm)                                              \
    PUSH_F32(vm, param_1)                                       \
    PUSH_F32(vm, param_2)                                       \
    CALL(vm)                                                    \
    POP_F32_CANONICAL_NAN(vm)                                   \
    END_TEST()

#define TEST_IN_I32_F32_TRAP(vm, param_1, param_2, err) \
    START_TEST(vm)                                      \
    PUSH_I32(vm, param_1)                               \
    PUSH_F32(vm, param_2)                               \
    CALL_AND_TRAP(vm, err)                              \
    END_TEST()

#define TEST_OUT_F64(vm, result) \
    START_TEST(vm)               \
    CALL(vm)                     \
    POP_F64(vm, result)          \
    END_TEST()

#define TEST_IN_F64_OUT_F64(vm, param_1, result) \
    START_TEST(vm)                               \
    PUSH_F64(vm, param_1)                        \
    CALL(vm)                                     \
    POP_F64(vm, result)                          \
    END_TEST()

#define TEST_IN_F64_OUT_F64_BITWISE(vm, param_1, result) \
    START_TEST(vm)                                       \
    PUSH_F64(vm, param_1)                                \
    CALL(vm)                                             \
    POP_F64_BITWISE(vm, result)                          \
    END_TEST()

#define TEST_IN_F64_OUT_ARITHMETIC_NAN(vm, param_1) \
    START_TEST(vm)                                  \
    PUSH_F64(vm, param_1)                           \
    CALL(vm)                                        \
    POP_F64_ARITHMETIC_NAN(vm)                      \
    END_TEST()

#define TEST_IN_F64_OUT_CANONICAL_NAN(vm, param_1) \
    START_TEST(vm)                                 \
    PUSH_F64(vm, param_1)                          \
    CALL(vm)                                       \
    POP_F64_CANONICAL_NAN(vm)                      \
    END_TEST()

#define TEST_IN_F64_F64_OUT_F64(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_F64(vm, param_1)                                     \
    PUSH_F64(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_F64(vm, result)                                       \
    END_TEST()

#define TEST_IN_F64_F64_OUT_F64_BITWISE(vm, param_1, param_2, result) \
    START_TEST(vm)                                                    \
    PUSH_F64(vm, param_1)                                             \
    PUSH_F64(vm, param_2)                                             \
    CALL(vm)                                                          \
    POP_F64_BITWISE(vm, result)                                       \
    END_TEST()

#define TEST_IN_F64_F64_OUT_I32(vm, param_1, param_2, result) \
    START_TEST(vm)                                            \
    PUSH_F64(vm, param_1)                                     \
    PUSH_F64(vm, param_2)                                     \
    CALL(vm)                                                  \
    POP_I32(vm, result)                                       \
    END_TEST()

#define TEST_IN_F64_F64_OUT_ARITHMETIC_NAN(vm, param_1, param_2) \
    START_TEST(vm)                                               \
    PUSH_F64(vm, param_1)                                        \
    PUSH_F64(vm, param_2)                                        \
    CALL(vm)                                                     \
    POP_F64_ARITHMETIC_NAN(vm)                                   \
    END_TEST()

#define TEST_IN_F64_F64_OUT_CANONICAL_NAN(vm, param_1, param_2) \
    START_TEST(vm)                                              \
    PUSH_F64(vm, param_1)                                       \
    PUSH_F64(vm, param_2)                                       \
    CALL(vm)                                                    \
    POP_F64_CANONICAL_NAN(vm)                                   \
    END_TEST()

#define TEST_IN_I32_F64_TRAP(vm, param_1, param_2, err) \
    START_TEST(vm)                                      \
    PUSH_I32(vm, param_1)                               \
    PUSH_F64(vm, param_2)                               \
    CALL_AND_TRAP(vm, err)                              \
    END_TEST()
