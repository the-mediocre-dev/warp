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

#include "warp-execution.h"
#include "warp-buf.h"
#include "warp-error.h"
#include "warp-macros.h"
#include "warp-stack-ops.h"
#include "warp.h"
#include "warp-wasm.h"

static uint32_t exec_invalid_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_unreachable_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_no_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_block_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_loop_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_if_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_else_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_end_op(struct wrp_vm *vm)
{
    if (vm->block_stk_head == vm->call_stk_block_ptrs[vm->call_stk_head]) {
        WRP_CHECK(wrp_pop_call(vm));
    } else {
        //pop block
    }

    return WRP_SUCCESS;
}

static uint32_t exec_br_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_br_if_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_br_table_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_return_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_call_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
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
    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint8_t *code_body = vm->mdle->code_bodies[func_idx];
    size_t code_body_sz = vm->mdle->code_bodies_sz[func_idx];
    uint32_t local_idx = 0;

    WRP_CHECK(wrp_read_varui32(code_body, code_body_sz, &vm->program_counter, &local_idx));

    uint32_t param_count = vm->mdle->param_counts[func_idx];
    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint32_t operand_stk_ptr = vm->call_stk_operand_ptrs[vm->call_stk_head];
    int32_t frame_tail = operand_stk_ptr - local_count - param_count;
    int32_t local_stk_ptr = frame_tail + param_count + local_idx;

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
    return WRP_ERR_UNKNOWN;
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
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_const_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f32_const_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_f64_const_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_eqz_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_eq_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_ne_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_lt_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_lt_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_gt_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_gt_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_le_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_le_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_ge_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_ge_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
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
    return WRP_ERR_UNKNOWN;
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
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_ctz_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_popcnt_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_add_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_sub_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_mul_op(struct wrp_vm *vm)
{
    uint64_t y = 0;
    WRP_CHECK(wrp_pop_operand(vm, &y, I32));

    uint64_t x = 0;
    WRP_CHECK(wrp_pop_operand(vm, &x, I32));

    uint64_t result = (uint64_t)((int32_t)x * (int32_t)y);
    WRP_CHECK(wrp_push_operand(vm, result, I32));

    return WRP_SUCCESS;
}

static uint32_t exec_i32_div_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_div_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_rem_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_rem_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_and_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_or_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_xor_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_shl_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_shr_s_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_shr_u_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_rotl_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i32_rotr_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_clz_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
}

static uint32_t exec_i64_ctz_op(struct wrp_vm *vm)
{
    return WRP_ERR_UNKNOWN;
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
};

uint32_t wrp_exec(struct wrp_vm *vm, uint32_t func_idx)
{
    WRP_CHECK(wrp_push_call(vm, func_idx));

    vm->program_counter = 0;

    while (vm->call_stk_head >= 0) {
        uint32_t current_func_idx = vm->call_stk_func_idx[vm->call_stk_head];
        uint8_t *code_body = vm->mdle->code_bodies[current_func_idx];
        uint8_t opcode = code_body[vm->program_counter];

        if (opcode >= NUM_OPCODES) {
            return WRP_ERR_INVALID_OPCODE;
        }

        vm->program_counter++;

        uint32_t err = exec_jump_table[opcode](vm);

        if (err != WRP_SUCCESS) {
            //restore program counter
            return err;
        }
    }

    return WRP_SUCCESS;
}
