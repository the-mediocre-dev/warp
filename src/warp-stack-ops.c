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

static uint32_t frame_operand_count(struct wrp_vm *vm)
{
    uint32_t count = (uint32_t)(vm->operand_stk_head + 1);

    if (vm->call_stk_head >= 0) {
        count -= vm->call_stk_operand_ptrs[vm->call_stk_head] + 1;
    }

    return count;
}

static uint32_t frame_block_count(struct wrp_vm *vm)
{
    uint32_t count = (uint32_t)(vm->block_stk_head + 1);

    if (vm->call_stk_head >= 0) {
        count -= vm->call_stk_block_ptrs[vm->call_stk_head] + 1;
    }

    return count;
}

uint32_t wrp_push_operand(struct wrp_vm *vm, uint64_t value, int8_t type)
{
    if (vm->operand_stk_head >= WRP_OPERAND_STK_SZ - 1) {
        return WRP_ERR_OP_STK_OVERFLOW;
    }

    vm->operand_stk_head++;
    vm->operand_stk_values[vm->operand_stk_head] = value;
    vm->operand_stk_types[vm->operand_stk_head] = type;
    return WRP_SUCCESS;
}

uint32_t wrp_pop_operand(struct wrp_vm *vm, uint64_t *value, int8_t *type)
{
    if (vm->operand_stk_head == -1) {
        return WRP_ERR_OP_STK_UNDERFLOW;
    }

    if (frame_operand_count(vm) == 0) {
        return WRP_ERR_FRAME_OP_UNDERFLOW;
    }

    *value = vm->operand_stk_values[vm->operand_stk_head];
    *type = vm->operand_stk_types[vm->operand_stk_head];
    vm->operand_stk_head--;

    return WRP_SUCCESS;
}

uint32_t wrp_push_i32(struct wrp_vm *vm, int32_t value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(int32_t); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_push_operand(vm, operand, I32);
}

uint32_t wrp_push_i64(struct wrp_vm *vm, int64_t value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(int64_t); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_push_operand(vm, operand, I64);
}

uint32_t wrp_push_f32(struct wrp_vm *vm, float value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(float); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_push_operand(vm, operand, F32);
}

uint32_t wrp_push_f64(struct wrp_vm *vm, double value)
{
    uint64_t operand = 0;
    for (uint32_t i = 0; i < sizeof(double); i++) {
        ((uint8_t *)(&operand))[i] = ((uint8_t *)&value)[i];
    }

    return wrp_push_operand(vm, operand, F64);
}

uint32_t wrp_pop_i32(struct wrp_vm *vm, int32_t *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_pop_operand(vm, &operand, &type));

    if(type != I32){
        return WRP_ERR_OP_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(int32_t); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

uint32_t wrp_pop_i64(struct wrp_vm *vm, int64_t *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_pop_operand(vm, &operand, &type));

    if(type != I64){
        return WRP_ERR_OP_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(int64_t); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

uint32_t wrp_pop_f32(struct wrp_vm *vm, float *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_pop_operand(vm, &operand, &type));

    if(type != F32){
        return WRP_ERR_OP_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(float); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

uint32_t wrp_pop_f64(struct wrp_vm *vm, double *value)
{
    uint64_t operand = 0;
    int8_t type;
    WRP_CHECK(wrp_pop_operand(vm, &operand, &type));

    if(type != F64){
        return WRP_ERR_OP_MISMATCH;
    }

    for (uint32_t i = 0; i < sizeof(double); i++) {
        ((uint8_t *)value)[i] = ((uint8_t *)(&operand))[i];
    }

    return WRP_SUCCESS;
}

uint32_t wrp_push_block(struct wrp_vm *vm, size_t label, int8_t type)
{
    if (vm->block_stk_head >= WRP_BLOCK_STK_SZ) {
        return WRP_ERR_BLOCK_STK_OVERFLOW;
    }

    vm->block_stk_head++;
    vm->block_stk_labels[vm->block_stk_head] = label;
    vm->block_stk_types[vm->block_stk_head] = type;
    vm->block_stk_operand_ptrs[vm->block_stk_head] = vm->operand_stk_head;
    return WRP_SUCCESS;
}

uint32_t wrp_pop_block(struct wrp_vm *vm, uint32_t depth)
{
    if (vm->block_stk_head == -1) {
        return WRP_ERR_BLOCK_STK_UNDERFLOW;
    }

    if (frame_block_count(vm) < depth + 1) {
        return WRP_ERR_FRAME_BLOCK_UNDERFLOW;
    }

    //if at the outer most frame block (ie implicit func block), return call.
    //this has to happen before the stack fiddling below, as the func
    //block is only implicit and does not yield values
    if (vm->block_stk_head - (depth + 1) == vm->call_stk_block_ptrs[vm->call_stk_head]) {
        WRP_CHECK(wrp_pop_call(vm));
        return WRP_SUCCESS;
    }

    //TODO handle multiple block return types
    uint64_t value = 0;
    int8_t type = 0;
    if (vm->block_stk_types[vm->block_stk_head - depth] != VOID) {
        value = vm->operand_stk_values[vm->operand_stk_head];
        type = vm->operand_stk_types[vm->operand_stk_head];
    }

    //restore operand stack
    vm->operand_stk_head = vm->block_stk_operand_ptrs[vm->block_stk_head - depth];

    //TODO handle multiple block return types
    if (vm->block_stk_types[vm->block_stk_head - depth] != VOID) {
        WRP_CHECK(wrp_push_operand(vm, value, type));
    }

    vm->program_counter = vm->block_stk_labels[vm->block_stk_head - depth] + 1;
    vm->block_stk_head -= (depth + 1);
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

    if (frame_operand_count(vm) < param_count) {
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
    vm->program_counter = 0;

    //push implicit func block, with label as final OP_END
    WRP_CHECK(wrp_push_block(vm, vm->mdle->code_bodies_sz[func_idx], VOID));

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

    if (frame_operand_count(vm) < result_count) {
        return WRP_ERR_CALL_MISSING_RESULTS;
    }

    //TODO is this a thing? is it possible to return from a call leaving blocks on the stack?
    //if(frame_block_count(vm) != 0 ){
    //}

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
    vm->operand_stk_head -= vm->mdle->local_counts[func_idx];

    //pop args
    vm->operand_stk_head -=vm->mdle->param_counts[type_idx];

    //push results
    if (result_count > 0) {
        WRP_CHECK(wrp_push_operand(vm, result, result_type));
    }

    return WRP_SUCCESS;
}

uint32_t wrp_set_program_counter(struct wrp_vm *vm, size_t instruction)
{
    uint32_t func_idx = vm->call_stk_func_idx[vm->call_stk_head];

    if (instruction > vm->mdle->code_bodies_sz[func_idx]) {
        return WRP_ERR_INSTRUCTION_OVERFLOW;
    }

    vm->program_counter = instruction;
    return WRP_SUCCESS;
}
