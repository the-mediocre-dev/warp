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
    mdle_sz += WRP_ALIGN_64(meta->num_blocks * sizeof(size_t));
    mdle_sz += WRP_ALIGN_64(meta->num_blocks * sizeof(size_t));
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

    mdle->blocks = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_blocks * sizeof(size_t));

    mdle->block_labels = (size_t *)(ptr + offset);
    offset += WRP_ALIGN_64(meta->num_blocks * sizeof(size_t));
}

bool wrp_is_valid_wasm_type(uint8_t type)
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

bool wrp_is_valid_block_type(uint8_t type)
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

bool wrp_is_valid_value_type(uint8_t type)
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

bool wrp_get_func_idx(struct wrp_wasm_mdle *mdle,
    const char *func_name,
    uint32_t *func_idx)
{
    for (uint32_t i = 0; i < mdle->num_exports; i++) {
        char *name = &mdle->export_names[mdle->export_name_offsets[i]];

        if (strcmp(name, func_name) == 0) {
            *func_idx = mdle->export_func_idxs[i];
            return true;
        }
    }

    return false;
}
