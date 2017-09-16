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

#include "warp-stack-ops.h"
#include "warp-error.h"
#include "warp-macros.h"
#include "warp.h"
#include "warp-wasm.h"

static uint32_t operand_count(struct wrp_vm *vm)
{
    uint32_t count = (uint32_t)vm->operand_stk_head + 1;

    if (vm->call_stk_head >= 0) {
        count -= vm->call_stk_operand_ptrs[vm->call_stk_head] + 1;
    }

    return count;
}

uint32_t wrp_push_operand(struct wrp_vm *vm, uint64_t value, uint8_t type)
{
    if (vm->operand_stk_head >= WRP_OPERAND_STK_SZ - 1) {
        return WRP_ERR_OP_STK_OVERFLOW;
    }

    vm->operand_stk_head++;
    vm->operand_stk_values[vm->operand_stk_head] = value;
    vm->operand_stk_types[vm->operand_stk_head] = type;
    return WRP_SUCCESS;
}

uint32_t wrp_pop_operand(struct wrp_vm *vm, uint64_t *value, uint8_t type)
{
    if (vm->operand_stk_head == -1 || vm->operand_stk_types[vm->operand_stk_head] != type) {
        return WRP_ERR_OP_STK_UNDERFLOW;
    }

    *value = vm->operand_stk_values[vm->operand_stk_head];
    vm->operand_stk_head--;
    return WRP_SUCCESS;
}

uint32_t wrp_push_block()
{
    return WRP_SUCCESS;
}

uint32_t wrp_pop_block()
{
    return WRP_SUCCESS;
}

uint32_t wrp_push_call(struct wrp_vm *vm, uint32_t func_idx)
{
    if (vm->call_stk_head >= WRP_CALL_STK_SZ) {
        return WRP_ERR_CALL_STK_OVERFLOW;
    }

    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t param_count = vm->mdle->param_counts[type_idx];
    uint32_t param_type_offset = vm->mdle->param_type_offsets[type_idx];

    if (operand_count(vm) < param_count) {
        //missing arguments
        //trap
        return WRP_ERR_CALL_MISSING_ARGS;
    }

    //validate value stack
    for (uint32_t i = 0; i < param_count; i++) {
        int32_t operand_idx = vm->operand_stk_head - (param_count - 1) + i;
        uint8_t operand_type = vm->operand_stk_types[operand_idx];

        if (operand_type != vm->mdle->param_types[param_type_offset + i]) {
            return WRP_ERR_CALL_INVALID_ARGS;
        }
    }

    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint32_t local_type_offset = vm->mdle->local_type_offsets[func_idx];

    //push locals
    for (uint32_t i = 0; i < local_count; i++) {
        uint32_t local_idx = local_type_offset + i;
        WRP_CHECK(wrp_push_operand(vm, 0, vm->mdle->local_types[local_idx]));
    }

    //push frame
    vm->call_stk_head++;
    vm->call_stk_func_idx[vm->call_stk_head] = func_idx;
    vm->call_stk_operand_ptrs[vm->call_stk_head] = vm->operand_stk_head;
    vm->call_stk_block_ptrs[vm->call_stk_head] = vm->block_stk_head;
    vm->call_stk_return_ptrs[vm->call_stk_head] = vm->program_counter;
    return WRP_SUCCESS;
}

uint32_t wrp_pop_call(struct wrp_vm *vm)
{
    if (vm->call_stk_head < 0) {
        return WRP_ERR_CALL_STK_UNDERFLOW;
    }

    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];
    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t result_count = vm->mdle->result_counts[type_idx];
    uint32_t result_type_offset = vm->mdle->result_type_offsets[type_idx];

    if (operand_count(vm) < result_count) {
        return WRP_ERR_CALL_MISSING_RESULTS;
    }

    uint64_t result = 0;
    uint8_t result_type = 0;

    //TODO handle multiple results
    if (result_count > 0) {
        result = vm->operand_stk_values[vm->operand_stk_head];
        result_type = vm->operand_stk_types[vm->operand_stk_head];

        if (result_type != vm->mdle->result_types[result_type_offset]) {
            return WRP_ERR_CALL_INVALID_RESULTS;
        }
    }

    //pop frame
    vm->operand_stk_head = vm->call_stk_operand_ptrs[vm->call_stk_head];
    vm->block_stk_head = vm->call_stk_block_ptrs[vm->call_stk_head];
    vm->program_counter = vm->call_stk_return_ptrs[vm->call_stk_head];
    vm->call_stk_head--;

    //pop locals
    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint32_t local_type_offset = vm->mdle->local_type_offsets[func_idx];

    for (uint32_t i = 0; i < local_count; i++) {
        uint64_t local = 0;
        uint32_t local_idx = local_type_offset + (local_count - 1 - i);
        WRP_CHECK(wrp_pop_operand(vm, &local, vm->mdle->local_types[local_idx]));
    }

    //pop args
    uint32_t param_count = vm->mdle->param_counts[type_idx];
    uint32_t param_type_offset = vm->mdle->param_type_offsets[type_idx];

    for (uint32_t i = 0; i < param_count; i++) {
        uint64_t param = 0;
        uint32_t param_idx = param_type_offset + (param_count - 1 - i);
        WRP_CHECK(wrp_pop_operand(vm, &param, vm->mdle->param_types[param_idx]));
    }

    //push results
    if (result_count > 0) {
        WRP_CHECK(wrp_push_operand(vm, result, result_type));
    }

    return WRP_SUCCESS;
}
