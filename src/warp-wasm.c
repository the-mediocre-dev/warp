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

size_t wrp_mdle_sz(struct wrp_wasm_meta *meta)
{
    size_t mdle_sz = sizeof(struct wrp_wasm_mdle);
    mdle_sz += WRP_ALIGN_64(meta->num_types * sizeof(uint8_t));
    mdle_sz += WRP_ALIGN_64(meta->num_type_params * sizeof(uint8_t *));
    mdle_sz += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_type_returns * sizeof(uint8_t *));
    mdle_sz += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_globals * sizeof(uint64_t));
    mdle_sz += WRP_ALIGN_64(meta->num_globals * sizeof(uint8_t));
    mdle_sz += WRP_ALIGN_64(meta->export_name_len * sizeof(char));
    mdle_sz += WRP_ALIGN_64(meta->num_exports * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_exports * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_code_locals * sizeof(uint8_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->code_body_sz * sizeof(uint8_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint8_t *));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(size_t *));
    mdle_sz += WRP_ALIGN_64(meta->num_block_ops * sizeof(size_t));
    mdle_sz += WRP_ALIGN_64(meta->num_block_ops * sizeof(size_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_if_ops * sizeof(size_t));
    mdle_sz += WRP_ALIGN_64(meta->num_if_ops * sizeof(size_t));
    mdle_sz += WRP_ALIGN_64(meta->num_if_ops * sizeof(size_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    mdle_sz += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    return mdle_sz;
}

void wrp_mdle_init(struct wrp_wasm_mdle *mdle, struct wrp_wasm_meta *meta)
{
    uint8_t *ptr = (uint8_t *)mdle;
    size_t offset = sizeof(struct wrp_wasm_mdle);

    mdle->forms = (uint8_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_types * sizeof(uint8_t));

    mdle->param_types = (uint8_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_type_params * sizeof(uint8_t));

    mdle->param_type_offsets = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->param_counts = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->result_types = (uint8_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_type_returns * sizeof(uint8_t));

    mdle->result_type_offsets = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->result_counts = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->func_type_idxs = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->global_values = (uint64_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_globals * sizeof(uint64_t));

    mdle->global_types = (uint8_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_globals * sizeof(uint8_t));

    mdle->export_names = (char *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->export_name_len * sizeof(char));

    mdle->export_name_offsets = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_exports * sizeof(uint32_t));

    mdle->export_func_idxs = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_exports * sizeof(uint32_t));

    mdle->local_types = (uint8_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_code_locals * sizeof(uint8_t));

    mdle->local_type_offsets = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->local_counts = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->code = (uint8_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->code_body_sz * sizeof(uint8_t));

    mdle->code_bodies = (uint8_t **)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint8_t *));

    mdle->code_bodies_sz = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(size_t));

    mdle->block_addresses = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_block_ops * sizeof(size_t));

    mdle->block_labels = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_block_ops * sizeof(size_t));

    mdle->block_offsets = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->block_counts = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->if_addresses = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_if_ops * sizeof(size_t));

    mdle->else_addresses = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_if_ops * sizeof(size_t));

    mdle->if_labels = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_if_ops * sizeof(size_t));

    mdle->if_offsets = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->if_counts = (uint32_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_funcs * sizeof(uint32_t));
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

bool wrp_is_valid_block_type(int8_t type)
{
    switch (type) {
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

uint32_t wrp_get_block_idx(struct wrp_wasm_mdle *mdle,
    uint32_t func_idx,
    size_t block_address,
    uint32_t *block_idx)
{
    for (uint32_t i = 0; i < mdle->block_counts[func_idx]; i++) {
        if (mdle->block_addresses[i + mdle->block_offsets[func_idx]] == block_address) {
            *block_idx = i;
            return WRP_SUCCESS;
        }
    }

    return WRP_ERR_UNKNOWN_BLOCK;
}

uint32_t wrp_get_if_idx(struct wrp_wasm_mdle *mdle,
    uint32_t func_idx,
    size_t if_address,
    uint32_t *if_idx)
{
    for (uint32_t i = 0; i < mdle->if_counts[func_idx]; i++) {
        if (mdle->if_addresses[i + mdle->if_offsets[func_idx]] == if_address) {
            *if_idx = i;
            return WRP_SUCCESS;
        }
    }

    return WRP_ERR_UNKNOWN_IF;
}

uint32_t wrp_get_func_idx(struct wrp_wasm_mdle *mdle,
    const char *func_name,
    uint32_t *func_idx)
{
    for (uint32_t i = 0; i < mdle->num_exports; i++) {
        char *name = &mdle->export_names[mdle->export_name_offsets[i]];

        if (strcmp(name, func_name) == 0) {
            *func_idx = mdle->export_func_idxs[i];
            return WRP_SUCCESS;
        }
    }

    return WRP_ERR_UNKNOWN_FUNC;
}

uint32_t wrp_check_immediates(uint8_t opcode,
    uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (opcode >= OP_BLOCK && opcode <= OP_IF) {
        int8_t block_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, buf_sz, pos, &block_type));

        //only void block supported in WASM v1
        if (!wrp_is_valid_block_type(block_type)) {
            return WRP_ERR_INVALID_BLOCK_TYPE;
        }
    } else if (opcode >= OP_BR && opcode <= OP_BR_IF) {
        uint32_t relative_depth = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &relative_depth));
    } else if (opcode == OP_BR_TABLE) {
        uint32_t target_count = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &target_count));

        for (uint32_t i = 0; i < target_count; i++) {
            uint32_t target = 0;
            WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &target));
        }

        uint32_t default_target = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &default_target));
    } else if (opcode == OP_CALL) {
        uint32_t func_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &func_idx));

        if (func_idx >= meta->num_funcs) {
            return WRP_ERR_INVALID_FUNC_IDX;
        }
    } else if (opcode == OP_CALL_INDIRECT) {
        uint32_t type_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &type_idx));

        if (type_idx >= meta->num_types) {
            return WRP_ERR_INVALID_TYPE_IDX;
        }

        int8_t indirect_reserved = 0;
        WRP_CHECK(wrp_read_vari7(buf, buf_sz, pos, &indirect_reserved));

        if (indirect_reserved != 0) {
            return WRP_ERR_INVALID_RESERVED;
        }
    } else if (opcode >= OP_GET_LOCAL && opcode <= OP_SET_LOCAL) {
        uint32_t local_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &local_idx));

        //TODO validate local idx
    } else if (opcode >= OP_GET_GLOBAL && opcode <= OP_SET_GLOBAL) {
        uint32_t global_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &global_idx));

        if (global_idx >= meta->num_globals) {
            return WRP_ERR_INVALID_GLOBAL_IDX;
        }
    } else if (opcode >= OP_I32_LOAD && opcode <= OP_I64_STORE_32) {
        uint32_t memory_immediate_flags = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &memory_immediate_flags));

        uint32_t memory_immediate_offset = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &memory_immediate_offset));

        //TODO validate memory immediate
    } else if (opcode >= OP_CURRENT_MEMORY && opcode <= OP_GROW_MEMORY) {
        int8_t memory_reserved = 0;
        WRP_CHECK(wrp_read_vari7(buf, buf_sz, pos, &memory_reserved));

        if (memory_reserved != 0) {
            return WRP_ERR_INVALID_RESERVED;
        }
    } else if (opcode == OP_I32_CONST) {
        int32_t i32_const = 0;
        WRP_CHECK(wrp_read_vari32(buf, buf_sz, pos, &i32_const));
    } else if (opcode == OP_I64_CONST) {
        int64_t i64_const = 0;
        WRP_CHECK(wrp_read_vari64(buf, buf_sz, pos, &i64_const));
    } else if (opcode == OP_F32_CONST) {
        float f32_const = 0;
        WRP_CHECK(wrp_read_f32(buf, buf_sz, pos, &f32_const));
    } else if (opcode == OP_F64_CONST) {
        double f64_const = 0;
        WRP_CHECK(wrp_read_f64(buf, buf_sz, pos, &f64_const));
    }

    return WRP_SUCCESS;
}
