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
#include "warp-wasm.h"
#include "warp.h"

static wrp_err_t call_frame_operand_count(wrp_vm_t *vm)
{
    uint32_t count = (uint32_t)(vm->oprd_stk_head + 1);

    if (vm->call_stk_head >= 0) {
        count -= vm->call_stk[vm->call_stk_head].oprd_stk_ptr + 1;
    }

    return count;
}

static wrp_err_t call_frame_block_count(wrp_vm_t *vm)
{
    uint32_t count = (uint32_t)(vm->ctrl_stk_head + 1);

    if (vm->call_stk_head >= 0) {
        count -= vm->call_stk[vm->call_stk_head].ctrl_stk_ptr + 1;
    }

    return count;
}

wrp_err_t wrp_stk_exec_push_op(wrp_vm_t *vm, uint64_t value, int8_t type)
{
    if (vm->oprd_stk_head >= WRP_OPERAND_STK_SZ - 1) {
        return WRP_ERR_OP_STK_OVERFLOW;
    }

    vm->oprd_stk_head++;
    vm->oprd_stk[vm->oprd_stk_head].value = value;
    vm->oprd_stk[vm->oprd_stk_head].type = type;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_op(wrp_vm_t *vm, uint64_t *value, int8_t *type)
{
    if (vm->oprd_stk_head == -1) {
        return WRP_ERR_OP_STK_UNDERFLOW;
    }

    if (call_frame_operand_count(vm) == 0) {
        return WRP_ERR_CALL_FRAME_OP_UNDERFLOW;
    }

    *value = vm->oprd_stk[vm->oprd_stk_head].value;
    *type = vm->oprd_stk[vm->oprd_stk_head].type;
    vm->oprd_stk_head--;

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_push_i32(wrp_vm_t *vm, int32_t value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(int32_t); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_stk_exec_push_op(vm, operand, I32);
}

wrp_err_t wrp_stk_exec_push_i64(wrp_vm_t *vm, int64_t value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(int64_t); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_stk_exec_push_op(vm, operand, I64);
}

wrp_err_t wrp_stk_exec_push_f32(wrp_vm_t *vm, float value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(float); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_stk_exec_push_op(vm, operand, F32);
}

wrp_err_t wrp_stk_exec_push_f64(wrp_vm_t *vm, double value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(double); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_stk_exec_push_op(vm, operand, F64);
}

wrp_err_t wrp_stk_exec_pop_i32(wrp_vm_t *vm, int32_t *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_stk_exec_pop_op(vm, &operand, &type));

    if (type != I32) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(int32_t); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_i64(wrp_vm_t *vm, int64_t *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_stk_exec_pop_op(vm, &operand, &type));

    if (type != I64) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(int64_t); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_f32(wrp_vm_t *vm, float *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_stk_exec_pop_op(vm, &operand, &type));

    if (type != F32) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(float); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_f64(wrp_vm_t *vm, double *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_stk_exec_pop_op(vm, &operand, &type));

    if (type != F64) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(double); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_push_block(wrp_vm_t *vm,
    size_t label,
    wrp_block_t type,
    int8_t signature)
{
    if (vm->ctrl_stk_head >= WRP_BLOCK_STK_SZ) {
        return WRP_ERR_BLOCK_STK_OVERFLOW;
    }

    vm->ctrl_stk_head++;
    vm->ctrl_stk[vm->ctrl_stk_head].label = label;
    vm->ctrl_stk[vm->ctrl_stk_head].type = type;
    vm->ctrl_stk[vm->ctrl_stk_head].signature = signature;
    vm->ctrl_stk[vm->ctrl_stk_head].oprd_stk_ptr = vm->oprd_stk_head;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_block(wrp_vm_t *vm, uint32_t depth)
{
    if (vm->ctrl_stk_head == -1) {
        return WRP_ERR_BLOCK_STK_UNDERFLOW;
    }

    if (call_frame_block_count(vm) < depth + 1) {
        return WRP_ERR_CALL_FRAME_BLOCK_UNDERFLOW;
    }

    //if at the outer most frame block (ie implicit func block), return call.
    //this has to happen before the stack fiddling below, as the func
    //block is only implicit and does not yield values
    if (vm->ctrl_stk_head - ((int32_t)depth + 1) == vm->call_stk[vm->call_stk_head].ctrl_stk_ptr) {
        WRP_CHECK(wrp_stk_exec_pop_call(vm));
        return WRP_SUCCESS;
    }

    //TODO handle multiple block return types
    uint64_t value = 0;
    int8_t type = 0;
    if (vm->ctrl_stk[vm->ctrl_stk_head - depth].signature != VOID) {
        value = vm->oprd_stk[vm->oprd_stk_head].value;
        type = vm->oprd_stk[vm->oprd_stk_head].type;
    }

    //restore operand stack
    vm->oprd_stk_head = vm->ctrl_stk[vm->ctrl_stk_head - depth].oprd_stk_ptr;

    //TODO handle multiple block return types
    if (vm->ctrl_stk[vm->ctrl_stk_head - depth].signature != VOID) {
        WRP_CHECK(wrp_stk_exec_push_op(vm, value, type));
    }

    vm->opcode_stream.pos = vm->ctrl_stk[vm->ctrl_stk_head - depth].label + 1;
    vm->ctrl_stk_head -= (depth + 1);
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_push_call(wrp_vm_t *vm, uint32_t func_idx)
{
    if (vm->call_stk_head >= WRP_CALL_STK_SZ) {
        return WRP_ERR_CALL_STK_OVERFLOW;
    }

    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t param_count = vm->mdle->param_counts[type_idx];
    uint32_t param_type_offset = vm->mdle->param_type_offsets[type_idx];

    if (call_frame_operand_count(vm) < param_count) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    //validate operand stack
    for (uint32_t i = 0; i < param_count; i++) {
        int32_t operand_idx = vm->oprd_stk_head - (param_count - 1) + i;
        uint8_t operand_type = vm->oprd_stk[operand_idx].type;

        if (operand_type != vm->mdle->param_types[param_type_offset + i]) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint32_t local_type_offset = vm->mdle->local_type_offsets[func_idx];

    //push locals
    for (uint32_t i = 0; i < local_count; i++) {
        uint32_t local_idx = local_type_offset + i;
        WRP_CHECK(wrp_stk_exec_push_op(vm, 0, vm->mdle->local_types[local_idx]));
    }

    //push frame
    vm->call_stk_head++;
    vm->call_stk[vm->call_stk_head].func_idx = func_idx;
    vm->call_stk[vm->call_stk_head].oprd_stk_ptr = vm->oprd_stk_head;
    vm->call_stk[vm->call_stk_head].ctrl_stk_ptr = vm->ctrl_stk_head;
    vm->call_stk[vm->call_stk_head].return_ptr = vm->opcode_stream.pos;

    //set opcode stream
    vm->opcode_stream.bytes = vm->mdle->code_bodies[func_idx];
    vm->opcode_stream.sz = vm->mdle->code_bodies_sz[func_idx];
    vm->opcode_stream.pos = 0;

    //push implicit func block, with label as final OP_END
    WRP_CHECK(wrp_stk_exec_push_block(vm, vm->mdle->code_bodies_sz[func_idx], BLOCK_FUNC, VOID));

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_call(wrp_vm_t *vm)
{
    if (vm->call_stk_head < 0) {
        return WRP_ERR_CALL_STK_UNDERFLOW;
    }

    uint32_t func_idx = vm->call_stk[vm->call_stk_head].func_idx;
    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t result_count = vm->mdle->result_counts[type_idx];
    uint32_t result_type_offset = vm->mdle->result_type_offsets[type_idx];

    if (call_frame_operand_count(vm) < result_count) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    uint64_t result = 0;
    uint8_t result_type = 0;

    //TODO handle multiple results
    if (result_count > 0) {
        result = vm->oprd_stk[vm->oprd_stk_head].value;
        result_type = vm->oprd_stk[vm->oprd_stk_head].type;

        if (result_type != vm->mdle->result_types[result_type_offset]) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    //set opcode stream
    if (vm->call_stk_head > 0) {
        uint32_t return_func_idx = vm->call_stk[vm->call_stk_head - 1].func_idx;
        vm->opcode_stream.bytes = vm->mdle->code_bodies[return_func_idx];
        vm->opcode_stream.sz = vm->mdle->code_bodies_sz[return_func_idx];
        vm->opcode_stream.pos = vm->call_stk[vm->call_stk_head].return_ptr;
    } else {
        vm->opcode_stream.bytes = NULL;
        vm->opcode_stream.sz = 0;
        vm->opcode_stream.pos = 0;
    }

    //pop frame
    vm->oprd_stk_head = vm->call_stk[vm->call_stk_head].oprd_stk_ptr;
    vm->ctrl_stk_head = vm->call_stk[vm->call_stk_head].ctrl_stk_ptr;
    vm->call_stk_head--;

    //pop locals
    vm->oprd_stk_head -= vm->mdle->local_counts[func_idx];

    //pop args
    vm->oprd_stk_head -= vm->mdle->param_counts[type_idx];

    //push results
    if (result_count > 0) {
        WRP_CHECK(wrp_stk_exec_push_op(vm, result, result_type));
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_call_frame_tail(wrp_vm_t *vm, int32_t *out_tail)
{
    uint32_t func_idx = vm->call_stk[vm->call_stk_head].func_idx;
    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t param_count = vm->mdle->param_counts[type_idx];
    uint32_t local_count = vm->mdle->local_counts[func_idx];
    uint32_t operand_stk_ptr = vm->call_stk[vm->call_stk_head].oprd_stk_ptr;
    *out_tail = (operand_stk_ptr + 1) - local_count - param_count;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_push_call(wrp_vm_t *vm, uint32_t func_idx)
{
    if (vm->call_stk_head >= WRP_CALL_STK_SZ - 1) {
        return WRP_ERR_CALL_STK_OVERFLOW;
    }

    vm->call_stk_head++;
    vm->call_stk[vm->call_stk_head].func_idx = func_idx;
    vm->call_stk[vm->call_stk_head].return_ptr = 0;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_func_sig(wrp_vm_t *vm)
{
    if (vm->call_stk_head == -1) {
        return WRP_ERR_CALL_STK_UNDERFLOW;
    }

    uint32_t func_idx = vm->call_stk[vm->call_stk_head].func_idx;
    uint32_t type_idx = vm->mdle->func_type_idxs[func_idx];
    uint32_t oprd_count = (vm->oprd_stk_head + 1) - (vm->ctrl_stk[vm->ctrl_stk_head].oprd_stk_ptr + 1);

    if (!vm->ctrl_stk[vm->ctrl_stk_head].unreachable) {

        if (vm->mdle->result_counts[type_idx] == 0 && oprd_count != 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }

        if (vm->mdle->result_counts[type_idx] != 0 && oprd_count == 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    int8_t result_type = 0;

    //pop the results
    if (vm->mdle->result_counts[type_idx] != 0) {
        uint32_t result_type_offset = vm->mdle->result_type_offsets[type_idx];
        WRP_CHECK(wrp_stk_check_pop_op(vm, vm->mdle->result_types[result_type_offset], &result_type));
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_push_op(wrp_vm_t *vm, int8_t type)
{
    if (vm->oprd_stk_head >= WRP_OPERAND_STK_SZ - 1) {
        return WRP_ERR_OP_STK_OVERFLOW;
    }

    vm->oprd_stk_head++;
    vm->oprd_stk[vm->oprd_stk_head].value = 0;
    vm->oprd_stk[vm->oprd_stk_head].type = type;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_pop_op(wrp_vm_t *vm, int8_t expected_type, int8_t *type)
{
    int32_t block_idx = vm->ctrl_stk_head;

    if (block_idx == -1) {
        return WRP_ERR_INVALID_BLOCK_IDX;
    }

    if (!vm->ctrl_stk[block_idx].unreachable && vm->oprd_stk_head == -1) {
        return WRP_ERR_OP_STK_UNDERFLOW;
    }

    if (!vm->ctrl_stk[block_idx].unreachable && vm->oprd_stk_head <= vm->ctrl_stk[block_idx].oprd_stk_ptr) {
        return WRP_ERR_CONTROL_FRAME_OP_UNDERFLOW;
    }

    uint8_t actual_type = 0;
    if (vm->ctrl_stk[block_idx].unreachable && vm->oprd_stk_head == vm->ctrl_stk[block_idx].oprd_stk_ptr) {
        actual_type = UNKNOWN;
    } else {
        actual_type = vm->oprd_stk[vm->oprd_stk_head].type;
        vm->oprd_stk_head--;
    }

    if (expected_type != actual_type && expected_type != UNKNOWN && actual_type != UNKNOWN) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    *type = actual_type;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_push_block(wrp_vm_t *vm,
    size_t address,
    wrp_block_t type,
    int8_t signature)
{
    if (vm->ctrl_stk_head >= WRP_BLOCK_STK_SZ) {
        return WRP_ERR_BLOCK_STK_OVERFLOW;
    }

    if (!wrp_is_valid_block_signature(signature)) {
        return WRP_ERR_INVALID_BLOCK_SIGNATURE;
    }

    vm->ctrl_stk_head++;
    vm->ctrl_stk[vm->ctrl_stk_head].type = type;
    vm->ctrl_stk[vm->ctrl_stk_head].address = address;
    vm->ctrl_stk[vm->ctrl_stk_head].label = 0;
    vm->ctrl_stk[vm->ctrl_stk_head].signature = signature;
    vm->ctrl_stk[vm->ctrl_stk_head].oprd_stk_ptr = vm->oprd_stk_head;
    vm->ctrl_stk[vm->ctrl_stk_head].unreachable = false;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_pop_block(wrp_vm_t *vm)
{
    int32_t block_idx = vm->ctrl_stk_head;

    if (block_idx == -1) {
        return WRP_ERR_BLOCK_STK_UNDERFLOW;
    }

    uint32_t oprd_count = (vm->oprd_stk_head + 1) - (vm->ctrl_stk[block_idx].oprd_stk_ptr + 1);

    if (!vm->ctrl_stk[block_idx].unreachable) {

        if (vm->ctrl_stk[block_idx].signature == VOID && oprd_count != 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }

        if (vm->ctrl_stk[block_idx].signature != VOID && oprd_count == 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    int8_t result_type = 0;

    //pop the results
    if (vm->ctrl_stk[vm->ctrl_stk_head].signature != VOID) {
        WRP_CHECK(wrp_stk_check_pop_op(vm, vm->ctrl_stk[vm->ctrl_stk_head].signature, &result_type));
    }

    //restore operand stack
    vm->oprd_stk_head = vm->ctrl_stk[vm->ctrl_stk_head].oprd_stk_ptr;

    //push the results
    if (vm->ctrl_stk[vm->ctrl_stk_head].signature != VOID) {
        WRP_CHECK(wrp_stk_check_push_op(vm, vm->ctrl_stk[vm->ctrl_stk_head].signature));
    }

    vm->ctrl_stk_head--;
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_block_sig(wrp_vm_t *vm, uint32_t depth, bool branch)
{
    int32_t block_idx = vm->ctrl_stk_head - depth;

    if (block_idx == -1) {
        return WRP_ERR_INVALID_BLOCK_IDX;
    }

    //loop labels have an arity of 0
    if (branch && vm->ctrl_stk[block_idx].type == LOOP_TYPE) {
        return WRP_SUCCESS;
    }

    uint32_t oprd_count = (vm->oprd_stk_head + 1) - (vm->ctrl_stk[block_idx].oprd_stk_ptr + 1);

    if (!vm->ctrl_stk[vm->ctrl_stk_head].unreachable) {

        if (vm->ctrl_stk[block_idx].signature == VOID && oprd_count != 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }

        if (vm->ctrl_stk[block_idx].signature != VOID && oprd_count == 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    int8_t result_type = 0;

    //pop the results
    if (vm->ctrl_stk[vm->ctrl_stk_head].signature != VOID) {
        WRP_CHECK(wrp_stk_check_pop_op(vm, vm->ctrl_stk[vm->ctrl_stk_head].signature, &result_type));
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_check_unreachable(wrp_vm_t *vm)
{
    int32_t block_idx = vm->ctrl_stk_head;

    if (block_idx == -1) {
        return WRP_ERR_INVALID_BLOCK_IDX;
    }

    //set top block as unreachable
    vm->ctrl_stk[block_idx].unreachable = true;

    //resize operand stack to allow for polymorphic type checking
    vm->oprd_stk_head = vm->ctrl_stk[block_idx].oprd_stk_ptr;

    return WRP_SUCCESS;
}
