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

#include "warp-config.h"
#include "warp-compiler-check.h"
#include "warp-error.h"
#include "warp-stack-ops.h"
#include "warp-wasm.h"

struct wrp_vm;
struct wrp_wasm_mdle;

typedef void *(*wrp_alloc_fn)(size_t size, size_t align);

typedef void (*wrp_free_fn)(void *ptr);

typedef void (*wrp_thunk_fn)(uint64_t *args,
    uint8_t arg_types,
    uint32_t num_args,
    uint64_t returns,
    uint8_t return_types,
    uint32_t num_returns);

struct wrp_vm {
    struct wrp_wasm_mdle *mdle;
    uint32_t error;
    size_t program_counter;
    wrp_alloc_fn alloc_fn;
    wrp_free_fn free_fn;
    uint64_t operand_stk_values[WRP_OPERAND_STK_SZ];
    int8_t operand_stk_types[WRP_OPERAND_STK_SZ];
    int32_t operand_stk_head;
    size_t block_stk_labels[WRP_BLOCK_STK_SZ];
    int32_t block_stk_operand_ptrs[WRP_CALL_STK_SZ];
    int8_t block_stk_types[WRP_BLOCK_STK_SZ];
    int32_t block_stk_head;
    uint32_t call_stk_func_idx[WRP_CALL_STK_SZ];
    int32_t call_stk_operand_ptrs[WRP_CALL_STK_SZ];
    int32_t call_stk_block_ptrs[WRP_CALL_STK_SZ];
    size_t call_stk_return_ptrs[WRP_CALL_STK_SZ];
    int32_t call_stk_head;
};

struct wrp_vm *wrp_open_vm(wrp_alloc_fn alloc_fn, wrp_free_fn free_fn);

struct wrp_wasm_mdle *wrp_instantiate_mdle(struct wrp_vm *vm,
    uint8_t *buf,
    size_t buf_sz);

void wrp_destroy_mdle(struct wrp_vm *vm, struct wrp_wasm_mdle *mdle);

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

void wrp_reset_vm(struct wrp_vm *vm);

void wrp_close_vm(struct wrp_vm *vm);
