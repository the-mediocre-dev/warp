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

//first include to check compiler
#include "warp-compiler-check.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "warp-buf.h"
#include "warp-config.h"
#include "warp-error.h"
#include "warp-stack-ops.h"
#include "warp-types.h"
#include "warp-wasm.h"

typedef void *(*wrp_alloc_fn_t)(size_t size, size_t align);

typedef void (*wrp_free_fn_t)(void *ptr);

typedef struct wrp_oprd {
    uint64_t value;
    uint8_t type;
} wrp_oprd_t;

typedef struct wrp_ctrl_frame {
    uint8_t type;
    size_t address;
    size_t label;
    int8_t signature;
    int32_t oprd_stk_ptr;
    bool unreachable;
} wrp_ctrl_frame_t;

typedef struct wrp_call_frame {
    uint32_t func_idx;
    int32_t oprd_stk_ptr;
    int32_t ctrl_stk_ptr;
    size_t return_ptr;
} wrp_call_frame_t;

typedef struct wrp_vm {
    wrp_wasm_mdle_t *mdle;
    wrp_alloc_fn_t alloc_fn;
    wrp_free_fn_t free_fn;
    wrp_oprd_t oprd_stk[WRP_OPERAND_STK_SZ];
    int32_t oprd_stk_head;
    wrp_ctrl_frame_t ctrl_stk[WRP_BLOCK_STK_SZ];
    int32_t ctrl_stk_head;
    wrp_call_frame_t call_stk[WRP_CALL_STK_SZ];
    int32_t call_stk_head;
    wrp_buf_t opcode_stream;
    wrp_err_t err;
} wrp_vm_t;

wrp_vm_t *wrp_open_vm(wrp_alloc_fn_t alloc_fn, wrp_free_fn_t free_fn);

wrp_wasm_mdle_t *wrp_instantiate_mdle(wrp_vm_t *vm, wrp_buf_t *buf);

void wrp_destroy_mdle(wrp_vm_t *vm, wrp_wasm_mdle_t *mdle);

wrp_err_t wrp_link_mdle(wrp_vm_t *vm, wrp_wasm_mdle_t *mdle);

wrp_err_t wrp_unlink_mdle(wrp_vm_t *vm);

wrp_err_t wrp_start(wrp_vm_t *vm);

wrp_err_t wrp_call(wrp_vm_t *vm, uint32_t func_idx);

void wrp_reset_vm(wrp_vm_t *vm);

void wrp_close_vm(wrp_vm_t *vm);
