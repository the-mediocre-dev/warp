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
#include "warp-encode.h"
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
    return wrp_stk_exec_push_op(vm, wrp_encode_i32(value), I32);
}

wrp_err_t wrp_stk_exec_push_i64(wrp_vm_t *vm, int64_t value)
{
    return wrp_stk_exec_push_op(vm, wrp_encode_i64(value), I64);
}

wrp_err_t wrp_stk_exec_push_f32(wrp_vm_t *vm, float value)
{
    return wrp_stk_exec_push_op(vm, wrp_encode_f32(value), F32);
}

wrp_err_t wrp_stk_exec_push_f64(wrp_vm_t *vm, double value)
{
    return wrp_stk_exec_push_op(vm, wrp_encode_f64(value), F64);
}

wrp_err_t wrp_stk_exec_pop_i32(wrp_vm_t *vm, int32_t *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_stk_exec_pop_op(vm, &operand, &type));

    if (type != I32) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    *value = wrp_decode_i32(operand);
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

    *value = wrp_decode_i64(operand);
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

    *value = wrp_decode_f32(operand);
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

    *value = wrp_decode_f64(operand);
    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_push_block(wrp_vm_t *vm,
    size_t label,
    uint8_t block_type,
    int8_t signature)
{
    if (vm->ctrl_stk_head >= WRP_BLOCK_STK_SZ) {
        return WRP_ERR_BLOCK_STK_OVERFLOW;
    }

    vm->ctrl_stk_head++;
    vm->ctrl_stk[vm->ctrl_stk_head].label = label;
    vm->ctrl_stk[vm->ctrl_stk_head].type = block_type;
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

    wrp_func_t *func = &vm->mdle->funcs[func_idx];
    wrp_type_t *type = &vm->mdle->types[func->type_idx];

    if (call_frame_operand_count(vm) < type->num_params) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    //validate operand stack
    for (uint32_t i = 0; i < type->num_params; i++) {
        int32_t operand_idx = vm->oprd_stk_head - (type->num_params - 1) + i;
        uint8_t operand_type = vm->oprd_stk[operand_idx].type;

        if (operand_type != type->param_types[i]) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    //push locals
    for (uint32_t i = 0; i < func->num_locals; i++) {
        WRP_CHECK(wrp_stk_exec_push_op(vm, 0, func->local_types[i]));
    }

    //push frame
    vm->call_stk_head++;
    vm->call_stk[vm->call_stk_head].func_idx = func_idx;
    vm->call_stk[vm->call_stk_head].oprd_stk_ptr = vm->oprd_stk_head;
    vm->call_stk[vm->call_stk_head].ctrl_stk_ptr = vm->ctrl_stk_head;
    vm->call_stk[vm->call_stk_head].return_ptr = vm->opcode_stream.pos;

    //set opcode stream
    vm->opcode_stream.bytes = func->code;
    vm->opcode_stream.sz = func->code_sz;
    vm->opcode_stream.pos = 0;

    //push implicit func block, with label as final OP_END
    WRP_CHECK(wrp_stk_exec_push_block(vm, func->code_sz, BLOCK_FUNC, VOID));

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_pop_call(wrp_vm_t *vm)
{
    if (vm->call_stk_head < 0) {
        return WRP_ERR_CALL_STK_UNDERFLOW;
    }

    uint32_t func_idx = vm->call_stk[vm->call_stk_head].func_idx;
    wrp_func_t *func = &vm->mdle->funcs[func_idx];
    wrp_type_t *type = &vm->mdle->types[func->type_idx];

    if (call_frame_operand_count(vm) < type->num_results) {
        return WRP_ERR_TYPE_MISMATCH;
    }

    uint64_t result = 0;
    uint8_t result_type = 0;

    //TODO handle multiple results
    if (type->num_results > 0) {
        result = vm->oprd_stk[vm->oprd_stk_head].value;
        result_type = vm->oprd_stk[vm->oprd_stk_head].type;

        if (result_type != type->result_types[0]) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    //set opcode stream
    if (vm->call_stk_head > 0) {
        uint32_t return_func_idx = vm->call_stk[vm->call_stk_head - 1].func_idx;
        vm->opcode_stream.bytes = vm->mdle->funcs[return_func_idx].code;
        vm->opcode_stream.sz = vm->mdle->funcs[return_func_idx].code_sz;
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
    vm->oprd_stk_head -= func->num_locals;

    //pop args
    vm->oprd_stk_head -= type->num_params;

    //push results
    if (type->num_results > 0) {
        WRP_CHECK(wrp_stk_exec_push_op(vm, result, result_type));
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_stk_exec_call_frame_tail(wrp_vm_t *vm, int32_t *out_tail)
{
    uint32_t func_idx = vm->call_stk[vm->call_stk_head].func_idx;
    uint32_t type_idx = vm->mdle->funcs[func_idx].type_idx;
    uint32_t num_params = vm->mdle->types[type_idx].num_params;
    uint32_t num_locals = vm->mdle->funcs[func_idx].num_locals;
    uint32_t operand_stk_ptr = vm->call_stk[vm->call_stk_head].oprd_stk_ptr;
    *out_tail = (operand_stk_ptr + 1) - num_locals - num_params;
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
    wrp_func_t *func = &vm->mdle->funcs[func_idx];
    wrp_type_t *type = &vm->mdle->types[func->type_idx];
    uint32_t oprd_count = (vm->oprd_stk_head + 1) - (vm->ctrl_stk[vm->ctrl_stk_head].oprd_stk_ptr + 1);

    if (!vm->ctrl_stk[vm->ctrl_stk_head].unreachable) {

        if (type->num_results == 0 && oprd_count != 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }

        if (type->num_results != 0 && oprd_count == 0) {
            return WRP_ERR_TYPE_MISMATCH;
        }
    }

    int8_t result_type = 0;

    //pop the results
    if (type->num_results != 0) {
        WRP_CHECK(wrp_stk_check_pop_op(vm, type->result_types[0], &result_type));
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
    uint8_t block_type,
    int8_t signature)
{
    if (vm->ctrl_stk_head >= WRP_BLOCK_STK_SZ) {
        return WRP_ERR_BLOCK_STK_OVERFLOW;
    }

    if (!wrp_is_valid_block_signature(signature)) {
        return WRP_ERR_INVALID_BLOCK_SIGNATURE;
    }

    vm->ctrl_stk_head++;
    vm->ctrl_stk[vm->ctrl_stk_head].type = block_type;
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
    if (branch && vm->ctrl_stk[block_idx].type == BLOCK_LOOP) {
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
