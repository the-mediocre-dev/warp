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

#define START_TEST(vm, test)    \
                                \
bool test(struct wrp_vm *vm)    \
{                               \
    wrp_reset_vm(vm);

#define PUSH_I32(vm, value)                       \
                                                  \
    if (wrp_push_i32(vm, value) != WRP_SUCCESS) { \
        return false;                             \
    }

#define PUSH_I64(vm, value)                       \
                                                  \
    if (wrp_push_i64(vm, value) != WRP_SUCCESS) { \
        return false;                             \
    }

#define PUSH_F32(vm, value)                       \
                                                  \
    if (wrp_push_f32(vm, value) != WRP_SUCCESS) { \
        return false;                             \
    }

#define PUSH_F64(vm, value)                       \
                                                  \
    if (wrp_push_f64(vm, value) != WRP_SUCCESS) { \
        return false;                             \
    }

#define CALL(vm, func)                                                    \
    {                                                                     \
        uint32_t func_idx = 0;                                            \
        if (wrp_get_func_idx(vm->mdle, func, &func_idx) != WRP_SUCCESS) { \
            return false;                                                 \
        }                                                                 \
                                                                          \
        if (!wrp_call(vm, func_idx)) {                                    \
            return false;                                                 \
        }                                                                 \
    }

#define CHECK_STK_EMPTY(vm)           \
                                      \
    if (vm->call_stk_head != -1) {    \
        return false;                 \
    }                                 \
                                      \
    if (vm->operand_stk_head != -1) { \
        return false;                 \
    }                                 \
                                      \
    if (vm->block_stk_head != -1) {   \
        return false;                 \
    }

#define CHECK_I32(vm, value)                           \
    {                                                  \
        int32_t result = 0;                            \
        if (wrp_pop_i32(vm, &result) != WRP_SUCCESS) { \
            return false;                              \
        }                                              \
                                                       \
        if (result != value) {                         \
            return false;                              \
        }                                              \
    }

#define CHECK_I64(vm, value)                           \
    {                                                  \
        int64_t result = 0;                            \
        if (wrp_pop_i64(vm, &result) != WRP_SUCCESS) { \
            return false;                              \
        }                                              \
                                                       \
        if (result != value) {                         \
            return false;                              \
        }                                              \
    }

#define CHECK_F32(vm, value)                           \
    {                                                  \
        float result = 0;                              \
        if (wrp_pop_f32(vm, &result) != WRP_SUCCESS) { \
            return false;                              \
        }                                              \
                                                       \
        if (result != value) {                         \
            return false;                              \
        }                                              \
    }

#define CHECK_F64(vm, value)                           \
    {                                                  \
        double result = 0;                             \
        if (wrp_pop_f64(vm, &result) != WRP_SUCCESS) { \
            return false;                              \
        }                                              \
                                                       \
        if (result != value) {                         \
            return false;                              \
        }                                              \
    }

#define END_TEST() \
                   \
    return true;   \
}
