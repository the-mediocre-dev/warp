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

#include <stdalign.h>

#include "warp-stack-ops.h"
#include "warp.h"
#include "warp-execution.h"
#include "warp-load.h"
#include "warp-validation.h"
#include "warp-wasm.h"

struct wrp_vm *wrp_open_vm(wrp_alloc_fn alloc_fn,
    wrp_free_fn free_fn,
    wrp_trap_fn trap_fn)
{
    if (!alloc_fn || !free_fn || !trap_fn) {
        return NULL;
    }

    struct wrp_vm *vm = alloc_fn(sizeof(struct wrp_vm), alignof(struct wrp_vm));

    if (!vm) {
        return NULL;
    }

    vm->alloc_fn = alloc_fn;
    vm->free_fn = free_fn;
    vm->trap_fn = trap_fn;
    vm->error = WRP_SUCCESS;
    vm->mdle = NULL;
    vm->operand_stk_head = -1;
    vm->block_stk_head = -1;
    vm->call_stk_head = -1;
    vm->program_counter = 0;
    return vm;
}

struct wrp_wasm_mdle *wrp_instantiate_mdle(struct wrp_vm *vm,
    uint8_t *buf,
    size_t buf_sz)
{
    struct wrp_wasm_meta meta = {};

    uint32_t error = wrp_validate_mdle(buf, buf_sz, &meta);

    if(error != WRP_SUCCESS){
        vm->error = error;
        return false;
    }

    size_t mdle_sz = wrp_mdle_sz(&meta);

    struct wrp_wasm_mdle *mdle = vm->alloc_fn(mdle_sz, 64);

    if (mdle == NULL) {
        return NULL;
    }

    wrp_mdle_init(mdle, &meta);

    error = wrp_load_mdle(mdle, buf, buf_sz, &meta);

    if(error != WRP_SUCCESS){
        vm->error = error;
        vm->free_fn(mdle);
        return false;
    }

    return mdle;
}

void wrp_destroy_mdle(struct wrp_vm *vm, struct wrp_wasm_mdle *mdle)
{
    vm->free_fn(mdle);
}

bool wrp_attach_mdle(struct wrp_vm *vm, struct wrp_wasm_mdle *mdle)
{
    if (vm->mdle) {
        return false;
    }

    // TODO validate imports

    vm->mdle = mdle;

    wrp_reset_vm(vm);

    // TODO run element init expressions
    // TODO run data init expressions

    return true;
}

bool wrp_detach_mdle(struct wrp_vm *vm)
{
    if (!vm->mdle) {
        return false;
    }

    vm->mdle = NULL;
    return true;
}

bool wrp_push_i32(struct wrp_vm *vm, int32_t value)
{
    return (wrp_push_operand(vm, value, I32) == WRP_SUCCESS);
}

bool wrp_push_i64(struct wrp_vm *vm, int64_t value)
{
    return (wrp_push_operand(vm, value, I64) == WRP_SUCCESS);
}

bool wrp_push_f32(struct wrp_vm *vm, float value)
{
    return (wrp_push_operand(vm, value, F32) == WRP_SUCCESS);
}

bool wrp_push_f64(struct wrp_vm *vm, double value)
{
    return (wrp_push_operand(vm, value, F64) == WRP_SUCCESS);
}

bool wrp_pop_i32(struct wrp_vm *vm, int32_t *value)
{
    uint64_t result = 0;

    if (wrp_pop_operand(vm, &result, I32) != WRP_SUCCESS) {
        return false;
    }

    *value = (int32_t)result;
    return true;
}

bool wrp_pop_i64(struct wrp_vm *vm, int64_t *value)
{
    uint64_t result = 0;

    if (wrp_pop_operand(vm, &result, I64) != WRP_SUCCESS) {
        return false;
    }

    *value = (int64_t)result;
    return true;
}

bool wrp_pop_f32(struct wrp_vm *vm, float *value)
{
    uint64_t result = 0;

    if (wrp_pop_operand(vm, &result, F32) != WRP_SUCCESS) {
        return false;
    }

    *value = (float)result;
    return true;
}

bool wrp_pop_f64(struct wrp_vm *vm, double *value)
{
    uint64_t result = 0;

    if (wrp_pop_operand(vm, &result, F64) != WRP_SUCCESS) {
        return false;
    }

    *value = (double)result;
    return true;
}

bool wrp_start(struct wrp_vm *vm)
{
    wrp_reset_vm(vm);

    return true;
}

bool wrp_call(struct wrp_vm *vm, uint32_t func_idx)
{
    if (vm->mdle == NULL) {
        //trap
        return false;
    }

    if (func_idx >= vm->mdle->num_funcs) {
        //trap
        return false;
    }

    uint32_t err = wrp_exec(vm, func_idx);

    if(err != WRP_SUCCESS){
        //trap
        return false;
    }

    return true;
}

void wrp_reset_vm(struct wrp_vm *vm)
{
    vm->error = WRP_SUCCESS;
    vm->operand_stk_head = -1;
    vm->block_stk_head = -1;
    vm->call_stk_head = -1;
}

void wrp_close_vm(struct wrp_vm *vm)
{
    vm->free_fn(vm);
}
