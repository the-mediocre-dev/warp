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
#include <string.h>

#include "warp-encode.h"
#include "warp-execution.h"
#include "warp-load.h"
#include "warp-scan.h"
#include "warp-stack-ops.h"
#include "warp-type-check.h"
#include "warp-wasm.h"
#include "warp.h"

wrp_vm_t *wrp_open_vm(wrp_alloc_fn_t alloc_fn, wrp_free_fn_t free_fn)
{
    if (!alloc_fn || !free_fn) {
        return NULL;
    }

    wrp_vm_t *vm = alloc_fn(sizeof(wrp_vm_t), alignof(wrp_vm_t));

    if (!vm) {
        return NULL;
    }

    vm->alloc_fn = alloc_fn;
    vm->free_fn = free_fn;
    vm->mdle = NULL;
    vm->oprd_stk_head = -1;
    vm->ctrl_stk_head = -1;
    vm->call_stk_head = -1;
    vm->opcode_stream.bytes = NULL;
    vm->opcode_stream.sz = 0;
    vm->opcode_stream.pos = 0;
    vm->err = WRP_SUCCESS;
    return vm;
}

wrp_wasm_mdle_t *wrp_instantiate_mdle(wrp_vm_t *vm, wrp_buf_t *buf)
{
    if (vm->mdle != NULL) {
        return NULL;
    }

    wrp_wasm_meta_t meta = {0};

    vm->err = WRP_ERR_UNKNOWN;

    if ((vm->err = wrp_scan_mdle(buf, &meta)) != WRP_SUCCESS) {
        return NULL;
    }

    if ((vm->err = wrp_check_meta(&meta)) != WRP_SUCCESS) {
        return NULL;
    }

    size_t mdle_sz = wrp_mdle_sz(&meta);

    wrp_wasm_mdle_t *mdle = vm->alloc_fn(mdle_sz, 64);

    if (mdle == NULL) {
        vm->err = WRP_ERR_MEMORY_ALLOCATION_FAILED;
        return mdle;
    }

    wrp_mdle_init(&meta, mdle);

    if ((vm->err = wrp_load_mdle(vm, buf, mdle)) != WRP_SUCCESS) {
        wrp_destroy_mdle(vm, mdle);
        vm->mdle = NULL;
        return NULL;
    }

    if ((vm->err = wrp_type_check_mdle(vm, mdle)) != WRP_SUCCESS) {
        wrp_destroy_mdle(vm, mdle);
        vm->mdle = NULL;
        return NULL;
    }

    vm->err = WRP_SUCCESS;
    return mdle;
}

void wrp_destroy_mdle(wrp_vm_t *vm, wrp_wasm_mdle_t *mdle)
{
    for (uint32_t i = 0; i < mdle->num_memories; i++) {
        if (mdle->memories[i].bytes != NULL) {
            vm->free_fn(mdle->memories[i].bytes);
        }
    }

    vm->free_fn(mdle);
}

wrp_err_t wrp_link_mdle(wrp_vm_t *vm, wrp_wasm_mdle_t *mdle)
{
    if (vm->mdle) {
        return WRP_ERR_UNKNOWN;
    }

    vm->mdle = mdle;

    for (uint32_t i = 0; i < mdle->num_imports; i++) {
        if (mdle->imports[i].kind == EXTERNAL_GLOBAL && mdle->globals[mdle->imports[i].idx].value == NULL) {
            vm->mdle = NULL;
            vm->err = WRP_ERR_MISSING_GLOBAL_IMPORT;
            return vm->err;
        }
    }

    // TODO run element init expressions

    for (uint32_t i = 0; i < mdle->num_memories; i++) {
        if (mdle->memories[i].num_pages != 0) {
            memset(mdle->memories[i].bytes, 0, mdle->memories[i].num_pages * PAGE_SIZE);
        }
    }

    for (uint32_t i = 0; i < mdle->num_data_segments; i++) {
        wrp_data_segment_t *segment = &mdle->data_segments[i];
        uint32_t mem_idx = segment->mem_idx;
        uint32_t mem_sz = mdle->memories[segment->mem_idx].num_pages * PAGE_SIZE;
        uint32_t data_sz = segment->sz;
        uint64_t offset_value = 0;

        if ((vm->err = wrp_exec_init_expr(vm, &segment->offset_expr, &offset_value)) != WRP_SUCCESS) {
            vm->mdle = NULL;
            return vm->err;
        }

        int32_t offset = wrp_decode_i32(offset_value);

        if ((offset < 0 || offset + data_sz > mem_sz)) {
            vm->mdle = NULL;
            vm->err = WRP_ERR_INVALID_MEMORY_ACCESS;
            return vm->err;
        }

        memcpy(mdle->memories[mem_idx].bytes + offset, segment->data, data_sz);
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_unlink_mdle(wrp_vm_t *vm)
{
    if (vm->mdle == NULL) {
        vm->err = WRP_ERR_UNKNOWN;
        return vm->err;
    }

    for (uint32_t i = 0; i < vm->mdle->num_imports; i++) {
        if (vm->mdle->imports[i].kind == EXTERNAL_GLOBAL){
            vm->mdle->globals[vm->mdle->imports[i].idx].value = NULL;
        }
    }

    vm->mdle = NULL;
    return WRP_SUCCESS;
}

wrp_err_t wrp_start(wrp_vm_t *vm)
{
    return WRP_SUCCESS;
}

wrp_err_t wrp_call(wrp_vm_t *vm, uint32_t func_idx)
{
    if (vm->mdle == NULL) {
        return WRP_ERR_UNKNOWN;
    }

    if (func_idx >= vm->mdle->num_funcs) {
        return WRP_ERR_INVALID_FUNC_IDX;
    }

    return (vm->err = wrp_exec_func(vm, func_idx));
}

void wrp_reset_vm(wrp_vm_t *vm)
{
    vm->oprd_stk_head = -1;
    vm->ctrl_stk_head = -1;
    vm->call_stk_head = -1;
    vm->opcode_stream.bytes = NULL;
    vm->opcode_stream.sz = 0;
    vm->opcode_stream.pos = 0;
    vm->err = WRP_SUCCESS;
}

void wrp_close_vm(wrp_vm_t *vm)
{
    vm->free_fn(vm);
}
