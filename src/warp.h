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
#include <stdint.h>

#include "warp-common.h"
#include "warp-config.h"

struct wrp_wasm_mdle;

struct wrp_vm {
    struct wrp_wasm_mdle *mdle;
    uint64_t stk_values[WRP_VALUE_STACK_SIZE];
    uint8_t stk_types[WRP_VALUE_STACK_SIZE];
    int32_t stk_head;
    wrp_alloc_fn alloc_fn;
    wrp_free_fn free_fn;
    wrp_trap_fn trap_fn;
} wrp_vm;

struct wrp_vm *wrp_open_vm(wrp_alloc_fn alloc_fn,
    wrp_free_fn free_fn,
    wrp_trap_fn trap_fn);

bool wrp_attach_mdle(struct wrp_vm *vm, struct wrp_wasm_mdle *mdle);

bool wrp_detach_mdle(struct wrp_vm *vm);

bool wrp_push_i32(struct wrp_vm *vm, int32_t value);

bool wrp_push_i64(struct wrp_vm *vm, int64_t value);

bool wrp_push_f32(struct wrp_vm *vm, float value);

bool wrp_push_f64(struct wrp_vm *vm, double value);

bool wrp_pop_i32(struct wrp_vm *vm, int32_t *value);

bool wrp_pop_i64(struct wrp_vm *vm, int64_t *value);

bool wrp_pop_f32(struct wrp_vm *vm, float *value);

bool wrp_pop_f64(struct wrp_vm *vm, double *value);

bool wrp_start(struct wrp_vm *vm);

bool wrp_call(struct wrp_vm *vm, uint32_t func_idx);

void wrp_close_vm(struct wrp_vm *vm);
