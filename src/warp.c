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

#include "warp.h"
#include "wasm.h"

static bool push_value(struct wrp_vm *vm, uint64_t value, uint8_t type)
{
    if (vm->stk_head >= WRP_VALUE_STACK_SIZE - 1) {
        return false;
    }

    vm->stk_head++;
    vm->stk_values[vm->stk_head] = value;
    vm->stk_types[vm->stk_head] = type;
    return true;
}

static bool pop_value(struct wrp_vm *vm, uint64_t *value, uint8_t type)
{
    if (vm->stk_head == -1 || vm->stk_types[vm->stk_head] != type) {
        return false;
    }

    *value = vm->stk_values[vm->stk_head--];
    return true;
}

struct wrp_vm *wrp_open_vm(wrp_alloc_fn alloc_fn,
    wrp_free_fn free_fn,
    wrp_trap_fn trap_fn)
{
    if (!alloc_fn || !free_fn || !trap_fn) {
        return NULL;
    }

    struct wrp_vm *vm = alloc_fn(sizeof(struct wrp_vm), 0);

    if (!vm) {
        return NULL;
    }

    //TODO zero mem?
    vm->alloc_fn = alloc_fn;
    vm->free_fn = free_fn;
    vm->trap_fn = trap_fn;
    vm->mdle = NULL;

    //-1 indicates empty stack
    vm->stk_head = -1;

    return vm;
}

bool wrp_attach_mdle(struct wrp_vm *vm, struct wrp_wasm_mdle *mdle)
{
    if (vm->mdle) {
        return false;
    }

    vm->mdle = mdle;

    // TODO clear value stack
    // TODO clear frame stack
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
    return push_value(vm, (uint64_t)value, I32);
}

bool wrp_push_i64(struct wrp_vm *vm, int64_t value)
{
    return push_value(vm, (uint64_t)value, I64);
}

bool wrp_push_f32(struct wrp_vm *vm, float value)
{
    return push_value(vm, (uint64_t)value, F32);
}

bool wrp_push_f64(struct wrp_vm *vm, double value)
{
    return push_value(vm, (uint64_t)value, F64);
}

bool wrp_pop_i32(struct wrp_vm *vm, int32_t *value)
{
    uint64_t result = 0;

    if (!pop_value(vm, &result, I32)) {
        return false;
    }

    *value = (int32_t)result;
    return true;
}

bool wrp_pop_i64(struct wrp_vm *vm, int64_t *value)
{
    uint64_t result = 0;

    if (!pop_value(vm, &result, I64)) {
        return false;
    }

    *value = (int64_t)result;
    return true;
}

bool wrp_pop_f32(struct wrp_vm *vm, float *value)
{
    uint64_t result = 0;

    if (!pop_value(vm, &result, F32)) {
        return false;
    }

    *value = (float)result;
    return true;
}

bool wrp_pop_f64(struct wrp_vm *vm, double *value)
{
    uint64_t result = 0;

    if (!pop_value(vm, &result, F64)) {
        return false;
    }

    *value = (double)result;
    return true;
}

bool wrp_start(struct wrp_vm *vm)
{
    return true;
}

bool wrp_call(struct wrp_vm *vm, uint32_t func_idx)
{
    return true;
}

void wrp_close_vm(struct wrp_vm *vm)
{
    vm->free_fn(vm);
}
