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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "warp-types.h"

wrp_err_t wrp_stk_exec_push_op(wrp_vm_t *vm, uint64_t value, int8_t type);

wrp_err_t wrp_stk_exec_pop_op(wrp_vm_t *vm, uint64_t *value, int8_t *type);

wrp_err_t wrp_stk_exec_push_i32(wrp_vm_t *vm, int32_t value);

wrp_err_t wrp_stk_exec_push_i64(wrp_vm_t *vm, int64_t value);

wrp_err_t wrp_stk_exec_push_f32(wrp_vm_t *vm, float value);

wrp_err_t wrp_stk_exec_push_f64(wrp_vm_t *vm, double value);

wrp_err_t wrp_stk_exec_pop_i32(wrp_vm_t *vm, int32_t *value);

wrp_err_t wrp_stk_exec_pop_i64(wrp_vm_t *vm, int64_t *value);

wrp_err_t wrp_stk_exec_pop_f32(wrp_vm_t *vm, float *value);

wrp_err_t wrp_stk_exec_pop_f64(wrp_vm_t *vm, double *value);

wrp_err_t wrp_stk_exec_push_block(wrp_vm_t *vm,
    size_t label,
    uint8_t block_type,
    int8_t signature);

wrp_err_t wrp_stk_exec_pop_block(wrp_vm_t *vm, uint32_t depth, bool branch);

wrp_err_t wrp_stk_exec_push_call(wrp_vm_t *vm, uint32_t func_idx);

wrp_err_t wrp_stk_exec_pop_call(wrp_vm_t *vm);

wrp_err_t wrp_stk_exec_call_frame_tail(wrp_vm_t *vm, int32_t *out_tail);

wrp_err_t wrp_stk_check_push_call(wrp_vm_t *vm, uint32_t func_idx);

wrp_err_t wrp_stk_check_func_sig(wrp_vm_t *vm);

wrp_err_t wrp_stk_check_push_op(wrp_vm_t *vm, int8_t type);

wrp_err_t wrp_stk_check_pop_op(wrp_vm_t *vm, int8_t expected_type, int8_t *type);

wrp_err_t wrp_stk_check_push_block(wrp_vm_t *vm,
    size_t address,
    uint8_t block_type,
    int8_t signature);

wrp_err_t wrp_stk_check_pop_block(wrp_vm_t *vm);

wrp_err_t wrp_stk_check_block_sig(wrp_vm_t *vm,
    uint32_t depth,
    bool branch,
    bool push_results);

wrp_err_t wrp_stk_check_unreachable(wrp_vm_t *vm);


