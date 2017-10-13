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

#include <string.h>

#include "warp-buf.h"
#include "warp-error.h"
#include "warp-macros.h"
#include "warp-wasm.h"

#define ALIGN_64(x) (((x + 63) / 64) * 64)

size_t wrp_mdle_sz(wrp_wasm_meta_t *meta)
{
    size_t mdle_sz = sizeof(wrp_wasm_mdle_t);
    mdle_sz += ALIGN_64(meta->num_type_params * sizeof(int8_t *));
    mdle_sz += ALIGN_64(meta->num_type_returns * sizeof(int8_t *));
    mdle_sz += ALIGN_64(meta->num_code_locals * sizeof(int8_t));
    mdle_sz += ALIGN_64(meta->code_buf_sz * sizeof(uint8_t));
    mdle_sz += ALIGN_64(meta->num_block_ops * sizeof(size_t));
    mdle_sz += ALIGN_64(meta->num_block_ops * sizeof(size_t));
    mdle_sz += ALIGN_64(meta->num_if_ops * sizeof(size_t));
    mdle_sz += ALIGN_64(meta->num_if_ops * sizeof(size_t));
    mdle_sz += ALIGN_64(meta->num_if_ops * sizeof(size_t));
    mdle_sz += ALIGN_64(meta->num_globals * sizeof(uint64_t));
    mdle_sz += ALIGN_64(meta->num_globals * sizeof(int8_t));
    mdle_sz += ALIGN_64(meta->export_name_buf_sz * sizeof(char));
    mdle_sz += ALIGN_64(meta->data_buf_sz * sizeof(uint8_t));
    mdle_sz += ALIGN_64(meta->num_types * sizeof(wrp_type_t));
    mdle_sz += ALIGN_64(meta->num_funcs * sizeof(wrp_func_t));
    mdle_sz += ALIGN_64(meta->num_globals * sizeof(wrp_global_t));
    mdle_sz += ALIGN_64(meta->num_exports * sizeof(wrp_export_t));
    mdle_sz += ALIGN_64(meta->num_memories * sizeof(wrp_memory_t));
    mdle_sz += ALIGN_64(meta->num_data_segments * sizeof(wrp_data_segment_t));
    return mdle_sz;
}

void wrp_mdle_init(wrp_wasm_meta_t *meta, wrp_wasm_mdle_t *out_mdle)
{
    uint8_t *ptr = (uint8_t *)out_mdle;
    size_t offset = sizeof(wrp_wasm_mdle_t);

    out_mdle->param_type_buf = (int8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_type_params * sizeof(uint8_t));

    out_mdle->result_type_buf = (int8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_type_returns * sizeof(uint8_t));

    out_mdle->local_type_buf = (int8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_code_locals * sizeof(uint8_t));

    out_mdle->code_buf = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->code_buf_sz * sizeof(uint8_t));

    out_mdle->block_addrs_buf = (size_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_block_ops * sizeof(size_t));

    out_mdle->block_label_buf = (size_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_block_ops * sizeof(size_t));

    out_mdle->if_addrs_buf = (size_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_if_ops * sizeof(size_t));

    out_mdle->else_addrs_buf = (size_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_if_ops * sizeof(size_t));

    out_mdle->if_label_buf = (size_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_if_ops * sizeof(size_t));

    out_mdle->global_value_buf = (uint64_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_globals * sizeof(uint64_t));

    out_mdle->global_type_buf = (int8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_globals * sizeof(uint8_t));

    out_mdle->export_name_buf = (char *)(ptr + offset);
    offset += ALIGN_64(meta->export_name_buf_sz * sizeof(char));

    out_mdle->data_buf = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->data_buf_sz * sizeof(uint8_t));

    out_mdle->types = (wrp_type_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_types * sizeof(wrp_type_t));

    out_mdle->funcs = (wrp_func_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_funcs * sizeof(wrp_func_t));

    out_mdle->globals = (wrp_global_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_globals * sizeof(wrp_global_t));

    out_mdle->exports = (wrp_export_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_exports * sizeof(wrp_export_t));

    out_mdle->memories = (wrp_memory_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_memories * sizeof(wrp_memory_t));

    out_mdle->data_segments = (wrp_data_segment_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_data_segments * sizeof(wrp_data_segment_t));

    //set memory
    out_mdle->num_memories = 1;
    out_mdle->memories[0].bytes = NULL;
    out_mdle->memories[0].num_pages = 0;
    out_mdle->memories[0].num_pages = MAX_MEMORY_PAGES;
}

bool wrp_is_valid_wasm_type(int8_t type)
{
    switch (type) {
    case I32:
    case I64:
    case F32:
    case F64:
    case ANY_FUNC:
    case FUNC:
    case VOID:
        return true;

    default:
        return false;
    }
}

bool wrp_is_valid_block_signature(int8_t signature)
{
    switch (signature) {
    case I32:
    case I64:
    case F32:
    case F64:
    case VOID:
        return true;

    default:
        return false;
    }
}

bool wrp_is_valid_value_type(int8_t type)
{
    switch (type) {
    case I32:
    case I64:
    case F32:
    case F64:
        return true;

    default:
        return false;
    }
}

wrp_err_t wrp_check_meta(wrp_wasm_meta_t *meta)
{
    if (meta->num_types > MAX_TYPES) {
        return WRP_ERR_MDLE_TYPE_OVERFLOW;
    }

    if (meta->num_funcs > MAX_FUNCS) {
        return WRP_ERR_MDLE_FUNC_OVERFLOW;
    }

    if (meta->num_tables > MAX_TABLES) {
        return WRP_ERR_MDLE_TABLE_OVERFLOW;
    }

    if (meta->num_memories > MAX_MEMORIES) {
        return WRP_ERR_MDLE_MEMORY_OVERFLOW;
    }

    if (meta->num_globals > MAX_GLOBALS) {
        return WRP_ERR_MDLE_GLOBAL_OVERFLOW;
    }

    if (meta->num_exports > MAX_EXPORTS) {
        return WRP_ERR_MDLE_EXPORT_OVERFLOW;
    }

    if (meta->num_elements > MAX_ELEMENT_SEGMENTS) {
        return WRP_ERR_MDLE_TYPE_OVERFLOW;
    }

    if (meta->num_code_locals > MAX_LOCALS) {
        return WRP_ERR_MDLE_LOCAL_OVERFLOW;
    }

    if (meta->code_buf_sz > MAX_CODE_SIZE) {
        return WRP_ERR_MDLE_CODE_OVERFLOW;
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_get_block_idx(wrp_wasm_mdle_t *mdle,
    uint32_t func_idx,
    size_t block_address,
    uint32_t *block_idx)
{
    wrp_func_t *func = &mdle->funcs[func_idx];

    for (uint32_t i = 0; i < func->num_blocks; i++) {
        if (func->block_addrs[i] == block_address) {
            *block_idx = i;
            return WRP_SUCCESS;
        }
    }

    return WRP_ERR_UNKNOWN_BLOCK;
}

wrp_err_t wrp_get_if_idx(wrp_wasm_mdle_t *mdle,
    uint32_t func_idx,
    size_t if_address,
    uint32_t *if_idx)
{
    wrp_func_t *func = &mdle->funcs[func_idx];

    for (uint32_t i = 0; i < func->num_ifs; i++) {
        if (func->if_addrs[i] == if_address) {
            *if_idx = i;
            return WRP_SUCCESS;
        }
    }

    return WRP_ERR_UNKNOWN_IF;
}

wrp_err_t wrp_get_func_idx(wrp_wasm_mdle_t *mdle,
    const char *func_name,
    uint32_t *func_idx)
{
    for (uint32_t i = 0; i < mdle->num_exports; i++) {
        if (mdle->exports[i].kind == EXTERNAL_FUNC) {
            char *name = mdle->exports[i].name;

            if (strcmp(name, func_name) == 0) {
                *func_idx = mdle->exports[i].idx;
                return WRP_SUCCESS;
            }
        }
    }

    return WRP_ERR_UNKNOWN_FUNC;
}
