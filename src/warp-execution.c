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

#include <stddef.h>

#include "warp-buf.h"
#include "warp-error.h"
#include "warp-execution.h"
#include "warp-macros.h"
#include "warp-stack-ops.h"
#include "warp-wasm.h"
#include "warp.h"

static uint32_t exec_invalid_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_unreachable_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNREACHABLE_CODE_EXECUTED;
}

static uint32_t exec_no_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));
    return WRP_SUCCESS;
}

static uint32_t exec_block_op(struct wrp_vm *vm)
{
    size_t block_address = vm->program_counter;
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    int8_t block_type = 0;
    WRP_CHECK(wrp_read_vari7(code_body, code_body_sz, &vm->program_counter, &block_type));

    uint32_t block_idx = 0;
    WRP_CHECK(wrp_get_block_idx(vm->mdle, func_idx, block_address, &block_idx));
    WRP_CHECK(wrp_push_block(vm, vm->mdle->block_labels[block_idx], block_type))

    return WRP_SUCCESS;
}

static uint32_t exec_loop_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_if_op(struct wrp_vm *vm)
{
    size_t if_address = vm->program_counter;
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    int8_t block_type = 0;
    WRP_CHECK(wrp_read_vari7(code_body, code_body_sz, &vm->program_counter, &block_type));

    if (!wrp_is_valid_block_type(block_type)) {
        return WRP_ERR_INVALID_BLOCK_TYPE;
    }

    uint32_t if_idx = 0;
    WRP_CHECK(wrp_get_if_idx(vm->mdle, func_idx, if_address, &if_idx));

    int32_t condition = 0;
    WRP_CHECK(wrp_pop_i32(vm, &condition));

    if (condition != 0 || vm->mdle->else_addresses[if_idx] != 0) {
        WRP_CHECK(wrp_push_block(vm, vm->mdle->if_labels[if_idx], block_type));
    }

    if (condition == 0 && vm->mdle->else_addresses[if_idx] == 0) {
        WRP_CHECK(wrp_set_program_counter(vm, vm->mdle->if_labels[if_idx] + 1));
    }

    if (condition == 0 && vm->mdle->else_addresses[if_idx] != 0) {
        WRP_CHECK(wrp_set_program_counter(vm, vm->mdle->else_addresses[if_idx] + 1));
    }

    return WRP_SUCCESS;
}

static uint32_t exec_else_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_pop_block(vm, 0));
    return WRP_SUCCESS;
}

static uint32_t exec_end_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_pop_block(vm, 0));
    return WRP_SUCCESS;
}

static uint32_t exec_br_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t depth = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &depth));
    WRP_CHECK(wrp_pop_block(vm, depth))

    return WRP_SUCCESS;
}

static uint32_t exec_br_if_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t depth = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &depth));

    int32_t condition = 0;
    WRP_CHECK(wrp_pop_i32(vm, &condition));

    if (condition != 0) {
        WRP_CHECK(wrp_pop_block(vm, depth));
    }

    return WRP_SUCCESS;
}

static uint32_t exec_br_table_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t target_count = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &target_count));

    //TODO handle any table size
    if (target_count > MAX_BRANCH_TABLE_SIZE) {
        return WRP_ERR_MDLE_BRANCH_TABLE_OVERFLOW;
    }

    uint32_t branch_table[MAX_BRANCH_TABLE_SIZE] = {0};

    for (uint32_t i = 0; i < target_count; i++) {
        WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &branch_table[i]));
    }

    uint32_t default_target = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &default_target));

    int32_t target_idx = 0;
    WRP_CHECK(wrp_pop_i32(vm, &target_idx));

    int32_t depth = default_target;

    if (target_idx >= 0 && (uint32_t)target_idx < target_count) {
        depth = target_idx;
    }

    WRP_CHECK(wrp_pop_block(vm, depth));
    return WRP_SUCCESS;
}

static uint32_t exec_return_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_call_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));
    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t target_idx = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &target_idx));
    WRP_CHECK(wrp_push_call(vm, target_idx));
    return WRP_SUCCESS;
}

static uint32_t exec_call_indirect_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_drop_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_select_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_get_local_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t param_count = vm->mdle->param_counts[type_idx];
    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t local_idx = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &local_idx));

    if (param_count + local_count == 0 || local_idx > param_count + local_count) {
        return WRP_ERR_INVALID_LOCAL_IDX;
    }

    uint32_t operand_stk_ptr = vm->call_stk_operand_ptrs[vm->call_stk_head];
    int32_t frame_tail = (operand_stk_ptr + 1) - local_count - param_count;
    int32_t local_stk_ptr = frame_tail + local_idx;

    //TODO validate local_stk_ptr is in current frame?
    if (local_stk_ptr < 0) {
        return WRP_ERR_INVALID_STK_OPERATION;
    }

    uint64_t local_value = vm->operand_stk_values[local_stk_ptr];
    uint8_t local_type = vm->operand_stk_types[local_stk_ptr];
    WRP_CHECK(wrp_push_operand(vm, local_value, local_type));

    return WRP_SUCCESS;
}

static uint32_t exec_set_local_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t param_count = vm->mdle->param_counts[type_idx];
    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t local_idx = 0;
    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &local_idx));

    if (param_count + local_count == 0 || local_idx > param_count + local_count) {
        return WRP_ERR_INVALID_LOCAL_IDX;
    }

    uint32_t operand_stk_ptr = vm->call_stk_operand_ptrs[vm->call_stk_head];
    int32_t frame_tail = (operand_stk_ptr + 1) - local_count - param_count;
    int32_t local_stk_ptr = frame_tail + local_idx;

    //TODO validate local_stk_ptr is in current frame?
    if (local_stk_ptr < 0) {
        return WRP_ERR_INVALID_STK_OPERATION;
    }

    uint64_t local_value = 0;
    int8_t local_type = 0;
    WRP_CHECK(wrp_pop_operand(vm, &local_value, &local_type));

    vm->operand_stk_values[local_stk_ptr] = local_value;
    vm->operand_stk_types[local_stk_ptr] = local_type;
    return WRP_SUCCESS;
}

static uint32_t exec_tee_local_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_get_global_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_set_global_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_load_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_load_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_load_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_load_8_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_load_8_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_load_16_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_load_16_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_8_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_8_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_16_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_16_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_32_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_load_32_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_store_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_store_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_store_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_store_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_store_8_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_store_16_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_store_8_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_store_16_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_store_32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_current_memory_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_grow_memory_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_const_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));
    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    int32_t i32_const = 0;
    WRP_CHECK(wrp_read_vari32(code_body, code_body_sz, &vm->program_counter, &i32_const));
    WRP_CHECK(wrp_push_i32(vm, i32_const));
    return WRP_SUCCESS;
}

static uint32_t exec_i64_const_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_const_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));
    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    float f32_const = 0;
    WRP_CHECK(wrp_read_f32(code_body, code_body_sz, &vm->program_counter, &f32_const));
    WRP_CHECK(wrp_push_f32(vm, f32_const));
    return WRP_SUCCESS;
}

static uint32_t exec_f64_const_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_eqz_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x == 0);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_eq_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x == y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_ne_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x != y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_lt_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x < y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_lt_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = ((uint32_t)x < (uint32_t)y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_gt_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x > y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_gt_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = ((uint32_t)x > (uint32_t)y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_le_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x <= y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_le_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = ((uint32_t)x <= (uint32_t)y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_ge_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (x >= y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_ge_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = ((uint32_t)x >= (uint32_t)y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i64_eqz_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_eq_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_ne_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_lt_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_lt_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_gt_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_gt_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_le_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_le_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_ge_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_ge_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_eq_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_ne_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_lt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_gt_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    float y = 0;
    WRP_CHECK(wrp_pop_f32(vm, &y));

    float x = 0;
    WRP_CHECK(wrp_pop_f32(vm, &x));

    int32_t result = (x > y);
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_f32_le_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_ge_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_eq_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_ne_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_lt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_gt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_le_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_ge_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_clz_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t operand = 0;
    WRP_CHECK(wrp_pop_i32(vm, &operand));

    uint32_t x = (uint32_t)operand;
    int32_t num_zeros = 32;

    //TODO optimize
    if (x != 0) {
        num_zeros = 0;
        while (x != 0 && (x & (1u << 31u)) == 0) {
            num_zeros++;
            x <<= 1;
        }
    }

    WRP_CHECK(wrp_push_i32(vm, num_zeros));
    return WRP_SUCCESS;
}

static uint32_t exec_i32_ctz_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t operand = 0;
    WRP_CHECK(wrp_pop_i32(vm, &operand));

    uint32_t x = (uint32_t)operand;
    uint32_t num_zeros = 32;

    //TODO optimize
    if (x != 0) {
        num_zeros = 0;
        while (x != 0 && (x & 1u) == 0) {
            num_zeros++;
            x >>= 1;
        }
    }

    WRP_CHECK(wrp_push_i32(vm, num_zeros));
    return WRP_SUCCESS;
}

static uint32_t exec_i32_popcnt_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t operand = 0;
    WRP_CHECK(wrp_pop_i32(vm, &operand));

    uint32_t x = (uint32_t)operand;
    uint32_t num_ones = 0;

    //TODO optimize
    while (x != 0) {
        if (x & 1u) {
            num_ones++;
        }
        x >>= 1;
    }

    WRP_CHECK(wrp_push_i32(vm, num_ones));
    return WRP_SUCCESS;
}

static uint32_t exec_i32_add_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x + y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_sub_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x - y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_mul_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x * y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_div_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    if (y == 0) {
        return WRP_ERR_I32_DIVIDE_BY_ZERO;
    }

    if (x == INT32_MIN && y == -1) {
        return WRP_ERR_I32_OVERFLOW;
    }

    int32_t result = x / y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_div_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    if (y == 0) {
        return WRP_ERR_I32_DIVIDE_BY_ZERO;
    }

    int32_t result = (uint32_t)x / (uint32_t)y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_rem_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    if (y == 0) {
        return WRP_ERR_I32_DIVIDE_BY_ZERO;
    }

    int32_t result = 0;

    //TODO confirm this shouldnt trap
    if (x != INT32_MIN && y != -1) {
        result = x % y;
    }

    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_rem_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    if (y == 0) {
        return WRP_ERR_I32_DIVIDE_BY_ZERO;
    }

    int32_t result = (uint32_t)x % (uint32_t)y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_and_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x & y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_or_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x | y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_xor_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x ^ y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_shl_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x << y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_shr_s_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = x >> y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_shr_u_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    int32_t result = (uint32_t)x >> (uint32_t)y;
    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_rotl_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    uint32_t count = ((uint32_t)y) % 32;

    //https://blog.regehr.org/archives/1063
    int32_t result = (((uint32_t)x) << count) | (((uint32_t)x) >> (-count & 31));

    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_rotr_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int32_t y = 0;
    WRP_CHECK(wrp_pop_i32(vm, &y));

    int32_t x = 0;
    WRP_CHECK(wrp_pop_i32(vm, &x));

    uint32_t count = ((uint32_t)y) % 32;

    //https://blog.regehr.org/archives/1063
    int32_t result = (((uint32_t)x) >> count) | (((uint32_t)x) << (-count & 31));

    WRP_CHECK(wrp_push_i32(vm, result));

    return WRP_SUCCESS;
}

static uint32_t exec_i64_clz_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int64_t x = 0;
    WRP_CHECK(wrp_pop_i64(vm, &x));

    uint64_t num_zeros = 64;

    //TODO optimize
    if (x != 0) {
        num_zeros = 0;
        while (x > 0) {
            x <<= 1;
            num_zeros++;
        }
    }

    WRP_CHECK(wrp_push_i32(vm, num_zeros));
    return WRP_SUCCESS;
}

static uint32_t exec_i64_ctz_op(struct wrp_vm *vm)
{
    WRP_CHECK(wrp_set_program_counter(vm, vm->program_counter + 1));

    int64_t x = 0;
    WRP_CHECK(wrp_pop_i64(vm, &x));

    uint32_t num_zeros = 64;

    //TODO optimize
    //http://graphics.stanford.edu/~seander/bithacks.html
    if (x != 0) {

        // set x's trailing 0s to 1s and zero rest
        x = (x ^ (x - 1)) >> 1;

        num_zeros = 0;
        while (x > 0) {
            x >>= 1;
            num_zeros++;
        }
    }

    WRP_CHECK(wrp_push_i32(vm, num_zeros));
    return WRP_SUCCESS;
}

static uint32_t exec_i64_popcnt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_add_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_sub_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_mul_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_div_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_div_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_rem_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_rem_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_and_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_or_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_xor_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_shl_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_shr_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_shr_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_rotl_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_rotr_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_abs_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_neg_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_ceil_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_floor_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_trunc_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_nearest_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_sqrt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_add_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_sub_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_mul_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_div_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_min_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_max_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_copy_sign_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_abs_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_neg_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_ceil_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_floor_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_trunc_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_nearest_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_sqrt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_add_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_sub_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_mul_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_div_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_min_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_max_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_copy_sign_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_wrap_i64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_trunc_s_f32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_trunc_u_f32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_trunc_s_f64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_trunc_u_f64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_extend_s_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_extend_u_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_trunc_s_f32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_trunc_u_f32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_trunc_s_f64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_trunc_u_f64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_convert_s_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_convert_u_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_convert_s_i64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_convert_u_i64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_demote_f64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_convert_s_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_convert_u_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_convert_s_i64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_convert_u_i64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_promote_f32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_reinterpret_f32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_reinterpret_f64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_reinterpret_i32_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_reinterpret_i64_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t (*const exec_jump_table[])(struct wrp_vm *vm) = {
        [OP_UNREACHABLE] = exec_unreachable_op,
    [OP_NOOP] = exec_no_op,
    [OP_BLOCK] = exec_block_op,
    [OP_LOOP] = exec_loop_op,
    [OP_IF] = exec_if_op,
    [OP_ELSE] = exec_else_op,
    [OP_RES_01] = exec_invalid_op,
    [OP_RES_02] = exec_invalid_op,
    [OP_RES_03] = exec_invalid_op,
    [OP_RES_04] = exec_invalid_op,
    [OP_RES_05] = exec_invalid_op,
    [OP_END] = exec_end_op,
    [OP_BR] = exec_br_op,
    [OP_BR_IF] = exec_br_if_op,
    [OP_BR_TABLE] = exec_br_table_op,
    [OP_RETURN] = exec_return_op,
    [OP_CALL] = exec_call_op,
    [OP_CALL_INDIRECT] = exec_call_indirect_op,
    [OP_RES_06] = exec_invalid_op,
    [OP_RES_07] = exec_invalid_op,
    [OP_RES_08] = exec_invalid_op,
    [OP_RES_09] = exec_invalid_op,
    [OP_RES_0A] = exec_invalid_op,
    [OP_RES_0B] = exec_invalid_op,
    [OP_RES_0C] = exec_invalid_op,
    [OP_RES_0D] = exec_invalid_op,
    [OP_DROP] = exec_drop_op,
    [OP_SELECT] = exec_select_op,
    [OP_RES_0E] = exec_invalid_op,
    [OP_RES_0F] = exec_invalid_op,
    [OP_RES_10] = exec_invalid_op,
    [OP_RES_11] = exec_invalid_op,
    [OP_GET_LOCAL] = exec_get_local_op,
    [OP_SET_LOCAL] = exec_set_local_op,
    [OP_TEE_LOCAL] = exec_tee_local_op,
    [OP_GET_GLOBAL] = exec_get_global_op,
    [OP_SET_GLOBAL] = exec_set_global_op,
    [OP_RES_12] = exec_invalid_op,
    [OP_RES_13] = exec_invalid_op,
    [OP_RES_14] = exec_invalid_op,
    [OP_I32_LOAD] = exec_i32_load_op,
    [OP_I64_LOAD] = exec_i64_load_op,
    [OP_F32_LOAD] = exec_f32_load_op,
    [OP_F64_LOAD] = exec_f64_load_op,
    [OP_I32_LOAD_8_S] = exec_i32_load_8_s_op,
    [OP_I32_LOAD_8_U] = exec_i32_load_8_u_op,
    [OP_I32_LOAD_16_S] = exec_i32_load_16_s_op,
    [OP_I32_LOAD_16_U] = exec_i32_load_16_u_op,
    [OP_I64_LOAD_8_S] = exec_i64_load_8_s_op,
    [OP_I64_LOAD_8_U] = exec_i64_load_8_u_op,
    [OP_I64_LOAD_16_S] = exec_i64_load_16_s_op,
    [OP_I64_LOAD_16_U] = exec_i64_load_16_u_op,
    [OP_I64_LOAD_32_S] = exec_i64_load_32_s_op,
    [OP_I64_LOAD_32_U] = exec_i64_load_32_u_op,
    [OP_I32_STORE] = exec_i32_store_op,
    [OP_I64_STORE] = exec_i64_store_op,
    [OP_F32_STORE] = exec_f32_store_op,
    [OP_F64_STORE] = exec_f64_store_op,
    [OP_I32_STORE_8] = exec_i32_store_8_op,
    [OP_I32_STORE_16] = exec_i32_store_16_op,
    [OP_I64_STORE_8] = exec_i64_store_8_op,
    [OP_I64_STORE_16] = exec_i64_store_16_op,
    [OP_I64_STORE_32] = exec_i64_store_32_op,
    [OP_CURRENT_MEMORY] = exec_current_memory_op,
    [OP_GROW_MEMORY] = exec_grow_memory_op,
    [OP_I32_CONST] = exec_i32_const_op,
    [OP_I64_CONST] = exec_i64_const_op,
    [OP_F32_CONST] = exec_f32_const_op,
    [OP_F64_CONST] = exec_f64_const_op,
    [OP_I32_EQZ] = exec_i32_eqz_op,
    [OP_I32_EQ] = exec_i32_eq_op,
    [OP_I32_NE] = exec_i32_ne_op,
    [OP_I32_LT_S] = exec_i32_lt_s_op,
    [OP_I32_LT_U] = exec_i32_lt_u_op,
    [OP_I32_GT_S] = exec_i32_gt_s_op,
    [OP_I32_GT_U] = exec_i32_gt_u_op,
    [OP_I32_LE_S] = exec_i32_le_s_op,
    [OP_I32_LE_U] = exec_i32_le_u_op,
    [OP_I32_GE_S] = exec_i32_ge_s_op,
    [OP_I32_GE_U] = exec_i32_ge_u_op,
    [OP_I64_EQZ] = exec_i64_eqz_op,
    [OP_I64_EQ] = exec_i64_eq_op,
    [OP_I64_NE] = exec_i64_ne_op,
    [OP_I64_LT_S] = exec_i64_lt_s_op,
    [OP_I64_LT_U] = exec_i64_lt_u_op,
    [OP_I64_GT_S] = exec_i64_gt_s_op,
    [OP_I64_GT_U] = exec_i64_gt_u_op,
    [OP_I64_LE_S] = exec_i64_le_s_op,
    [OP_I64_LE_U] = exec_i64_le_u_op,
    [OP_I64_GE_S] = exec_i64_ge_s_op,
    [OP_I64_GE_U] = exec_i64_ge_u_op,
    [OP_F32_EQ] = exec_f32_eq_op,
    [OP_F32_NE] = exec_f32_ne_op,
    [OP_F32_LT] = exec_f32_lt_op,
    [OP_F32_GT] = exec_f32_gt_op,
    [OP_F32_LE] = exec_f32_le_op,
    [OP_F32_GE] = exec_f32_ge_op,
    [OP_F64_EQ] = exec_f64_eq_op,
    [OP_F64_NE] = exec_f64_ne_op,
    [OP_F64_LT] = exec_f64_lt_op,
    [OP_F64_GT] = exec_f64_gt_op,
    [OP_F64_LE] = exec_f64_le_op,
    [OP_F64_GE] = exec_f64_ge_op,
    [OP_I32_CLZ] = exec_i32_clz_op,
    [OP_I32_CTZ] = exec_i32_ctz_op,
    [OP_I32_POPCNT] = exec_i32_popcnt_op,
    [OP_I32_ADD] = exec_i32_add_op,
    [OP_I32_SUB] = exec_i32_sub_op,
    [OP_I32_MUL] = exec_i32_mul_op,
    [OP_I32_DIV_S] = exec_i32_div_s_op,
    [OP_I32_DIV_U] = exec_i32_div_u_op,
    [OP_I32_REM_S] = exec_i32_rem_s_op,
    [OP_I32_REM_U] = exec_i32_rem_u_op,
    [OP_I32_AND] = exec_i32_and_op,
    [OP_I32_OR] = exec_i32_or_op,
    [OP_I32_XOR] = exec_i32_xor_op,
    [OP_I32_SHL] = exec_i32_shl_op,
    [OP_I32_SHR_S] = exec_i32_shr_s_op,
    [OP_I32_SHR_U] = exec_i32_shr_u_op,
    [OP_I32_ROTL] = exec_i32_rotl_op,
    [OP_I32_ROTR] = exec_i32_rotr_op,
    [OP_I64_CLZ] = exec_i64_clz_op,
    [OP_I64_CTZ] = exec_i64_ctz_op,
    [OP_I64_POPCNT] = exec_i64_popcnt_op,
    [OP_I64_ADD] = exec_i64_add_op,
    [OP_I64_SUB] = exec_i64_sub_op,
    [OP_I64_MUL] = exec_i64_mul_op,
    [OP_I64_DIV_S] = exec_i64_div_s_op,
    [OP_I64_DIV_U] = exec_i64_div_u_op,
    [OP_I64_REM_S] = exec_i64_rem_s_op,
    [OP_I64_REM_U] = exec_i64_rem_u_op,
    [OP_I64_AND] = exec_i64_and_op,
    [OP_I64_OR] = exec_i64_or_op,
    [OP_I64_XOR] = exec_i64_xor_op,
    [OP_I64_SHL] = exec_i64_shl_op,
    [OP_I64_SHR_S] = exec_i64_shr_s_op,
    [OP_I64_SHR_U] = exec_i64_shr_u_op,
    [OP_I64_ROTL] = exec_i64_rotl_op,
    [OP_I64_ROTR] = exec_i64_rotr_op,
    [OP_F32_ABS] = exec_f32_abs_op,
    [OP_F32_NEG] = exec_f32_neg_op,
    [OP_F32_CEIL] = exec_f32_ceil_op,
    [OP_F32_FLOOR] = exec_f32_floor_op,
    [OP_F32_TRUNC] = exec_f32_trunc_op,
    [OP_F32_NEAREST] = exec_f32_nearest_op,
    [OP_F32_SQRT] = exec_f32_sqrt_op,
    [OP_F32_ADD] = exec_f32_add_op,
    [OP_F32_SUB] = exec_f32_sub_op,
    [OP_F32_MUL] = exec_f32_mul_op,
    [OP_F32_DIV] = exec_f32_div_op,
    [OP_F32_MIN] = exec_f32_min_op,
    [OP_F32_MAX] = exec_f32_max_op,
    [OP_F32_COPY_SIGN] = exec_f32_copy_sign_op,
    [OP_F64_ABS] = exec_f64_abs_op,
    [OP_F64_NEG] = exec_f64_neg_op,
    [OP_F64_CEIL] = exec_f64_ceil_op,
    [OP_F64_FLOOR] = exec_f64_floor_op,
    [OP_F64_TRUNC] = exec_f64_trunc_op,
    [OP_F64_NEAREST] = exec_f64_nearest_op,
    [OP_F64_SQRT] = exec_f64_sqrt_op,
    [OP_F64_ADD] = exec_f64_add_op,
    [OP_F64_SUB] = exec_f64_sub_op,
    [OP_F64_MUL] = exec_f64_mul_op,
    [OP_F64_DIV] = exec_f64_div_op,
    [OP_F64_MIN] = exec_f64_min_op,
    [OP_F64_MAX] = exec_f64_max_op,
    [OP_F64_COPY_SIGN] = exec_f64_copy_sign_op,
    [OP_I32_WRAP_I64] = exec_i32_wrap_i64_op,
    [OP_I32_TRUNC_S_F32] = exec_i32_trunc_s_f32_op,
    [OP_I32_TRUNC_U_F32] = exec_i32_trunc_u_f32_op,
    [OP_I32_TRUNC_S_F64] = exec_i32_trunc_s_f64_op,
    [OP_I32_TRUNC_U_F64] = exec_i32_trunc_u_f64_op,
    [OP_I64_EXTEND_S_I32] = exec_i64_extend_s_i32_op,
    [OP_I64_EXTEND_U_I32] = exec_i64_extend_u_i32_op,
    [OP_I64_TRUNC_S_F32] = exec_i64_trunc_s_f32_op,
    [OP_I64_TRUNC_U_F32] = exec_i64_trunc_u_f32_op,
    [OP_I64_TRUNC_S_F64] = exec_i64_trunc_s_f64_op,
    [OP_I64_TRUNC_U_F64] = exec_i64_trunc_u_f64_op,
    [OP_F32_CONVERT_S_I32] = exec_f32_convert_s_i32_op,
    [OP_F32_CONVERT_U_I32] = exec_f32_convert_u_i32_op,
    [OP_F32_CONVERT_S_I64] = exec_f32_convert_s_i64_op,
    [OP_F32_CONVERT_U_I64] = exec_f32_convert_u_i64_op,
    [OP_F32_DEMOTE_F64] = exec_f32_demote_f64_op,
    [OP_F64_CONVERT_S_I32] = exec_f64_convert_s_i32_op,
    [OP_F64_CONVERT_U_I32] = exec_f64_convert_u_i32_op,
    [OP_F64_CONVERT_S_I64] = exec_f64_convert_s_i64_op,
    [OP_F64_CONVERT_U_I64] = exec_f64_convert_u_i64_op,
    [OP_F64_PROMOTE_F32] = exec_f64_promote_f32_op,
    [OP_I32_REINTERPRET_F32] = exec_i32_reinterpret_f32_op,
    [OP_I64_REINTERPRET_F64] = exec_i64_reinterpret_f64_op,
    [OP_F32_REINTERPRET_I32] = exec_f32_reinterpret_i32_op,
    [OP_F64_REINTERPRET_I64] = exec_f64_reinterpret_i64_op
    //clang-format brace hack
};

uint32_t wrp_exec(struct wrp_vm *vm, uint32_t func_idx)
{
    WRP_CHECK(wrp_push_call(vm, func_idx));

    vm->program_counter = 0;

    while (vm->call_stk_head >= 0) {
        uint32_t current_func_idx = vm->call_stk_func_idx[vm->call_stk_head];
        uint8_t *code_body = vm->mdle->code_bodies[current_func_idx];
        size_t code_body_sz = vm->mdle->code_bodies_sz[current_func_idx];

        if (vm->program_counter >= code_body_sz) {
            return WRP_ERR_INSTRUCTION_OVERFLOW;
        }

        uint8_t opcode = code_body[vm->program_counter];

        if (opcode >= NUM_OPCODES) {
            return WRP_ERR_INVALID_OPCODE;
        }

        uint32_t error = exec_jump_table[opcode](vm);

        if (error != WRP_SUCCESS) {
            //restore program counter
            return error;
        }
    }

    return WRP_SUCCESS;
}
