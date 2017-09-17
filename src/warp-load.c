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

#include "warp-load.h"
#include "warp-buf.h"
#include "warp-macros.h"
#include "warp-wasm.h"
#include "warp.h"

static uint32_t load_types(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    mdle->num_types = meta->num_types;
    uint32_t current_param = 0;
    uint32_t current_result = 0;

    for (uint32_t i = 0; i < meta->num_types; i++) {
        size_t pos = 0;
        WRP_CHECK(wrp_seek(buf, buf_sz, &pos, meta->types[i]));
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, &pos, &mdle->forms[i]));
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &mdle->param_counts[i]));

        mdle->param_type_offsets[i] = current_param;

        for (uint32_t j = 0; j < mdle->param_counts[i]; j++) {
            WRP_CHECK(wrp_read_varui7(buf, buf_sz, &pos, &mdle->param_types[current_param]));
            current_param++;
        }

        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &mdle->result_counts[i]));

        mdle->result_type_offsets[i] = current_result;

        for (uint32_t j = 0; j < mdle->result_counts[i]; j++) {
            WRP_CHECK(wrp_read_varui7(buf, buf_sz, &pos, &mdle->result_types[current_result]));
            current_result++;
        }
    }

    return WRP_SUCCESS;
}

static uint32_t load_imports(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    return WRP_SUCCESS;
}

static uint32_t load_funcs(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    mdle->num_funcs = meta->num_funcs;

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        size_t pos = 0;
        WRP_CHECK(wrp_seek(buf, buf_sz, &pos, meta->funcs[i]));
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &mdle->func_type_idxs[i]));
    }

    return WRP_SUCCESS;
}

static uint32_t load_tables(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    return WRP_SUCCESS;
}

static uint32_t load_memories(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    return WRP_SUCCESS;
}

static uint32_t load_globals(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    mdle->num_globals = meta->num_globals;

    for (uint32_t i = 0; i < meta->num_globals; i++) {
        size_t pos = 0;
        WRP_CHECK(wrp_seek(buf, buf_sz, &pos, meta->globals[i]));

        mdle->global_values[i] = 0;
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, &pos, &mdle->global_types[i]));

        //skip mutibility
        uint8_t mutibility = 0;
        WRP_CHECK(wrp_read_varui1(buf, buf_sz, &pos, &mutibility));
    }

    return WRP_SUCCESS;
}

static uint32_t load_exports(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    mdle->num_exports = meta->num_exports;
    uint32_t current_char = 0;

    for (uint32_t i = 0; i < meta->num_exports; i++) {
        size_t pos = 0;
        WRP_CHECK(wrp_seek(buf, buf_sz, &pos, meta->exports[i]));

        mdle->export_name_offsets[i] = current_char;
        char *name = &mdle->export_names[current_char];
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, buf_sz, &pos, name, MAX_GLOBAL_NAME_SIZE, &name_len));

        current_char += name_len + 1;

        //skip kind
        uint8_t kind = 0;
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, &pos, &kind));
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &mdle->export_func_idxs[i]));
    }

    return WRP_SUCCESS;
}

static uint32_t load_start(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    if (!meta->start_func_present) {
        return WRP_SUCCESS;
    }

    size_t pos = 0;
    WRP_CHECK(wrp_seek(buf, buf_sz, &pos, meta->start_func));
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &mdle->start_func_idx));

    mdle->start_func_present = true;

    return WRP_SUCCESS;
}

static uint32_t load_elements(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    return WRP_SUCCESS;
}

static uint32_t load_code(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    uint32_t current_local = 0;
    uint8_t code_pos = 0;

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        size_t pos = 0;
        WRP_CHECK(wrp_seek(buf, buf_sz, &pos, meta->codes[i]));

        mdle->local_type_offsets[i] = current_local;

        uint32_t body_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &body_sz));

        uint32_t num_local_entries = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &num_local_entries));

        for (uint32_t j = 0; j < num_local_entries; j++) {
            uint32_t num_locals = 0;
            WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &num_locals));

            int8_t value_type;
            WRP_CHECK(wrp_read_vari7(buf, buf_sz, &pos, &value_type));

            for (uint32_t k = 0; k < num_locals; k++) {
                mdle->local_types[current_local++] = value_type;
            }

            mdle->local_counts[i] += num_locals;
        }

        mdle->code_bodies[i] = &mdle->code[code_pos];
        mdle->code_bodies_sz[i] = body_sz - (pos - meta->codes[i]);

        size_t end_pos = pos + mdle->code_bodies_sz[i];

        //TODO load blocks and copy code in one pass
        while (pos <= end_pos) {
            WRP_CHECK(wrp_read_uint8(buf, buf_sz, &pos, &mdle->code[code_pos]));
            code_pos++;
        }
    }

    return WRP_SUCCESS;
}

static uint32_t load_data(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    struct wrp_wasm_mdle *mdle)
{
    return WRP_SUCCESS;
}

static uint32_t map_blocks(struct wrp_wasm_meta *meta, struct wrp_wasm_mdle *mdle)
{
    mdle->num_blocks = meta->num_blocks;

    if (mdle->num_blocks == 0) {
        return WRP_SUCCESS;
    }

    uint32_t idx_stk[MAX_BLOCK_DEPTH] = {};
    int32_t idx_stk_head = -1;
    uint32_t current_block = 0;

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        size_t pos = 0;
        size_t end_pos = pos + mdle->code_bodies_sz[i];

        while (pos < end_pos) {
            uint8_t opcode = 0;
            WRP_CHECK(wrp_read_uint8(mdle->code_bodies[i], mdle->code_bodies_sz[i], &pos, &opcode));

            if (opcode == OP_BLOCK || opcode == OP_IF || opcode == OP_ELSE || opcode == OP_LOOP) {
                if (idx_stk_head >= MAX_BLOCK_DEPTH) {
                    return false;
                }

                idx_stk[++idx_stk_head] = current_block;
                mdle->blocks[current_block] = pos - 1;
                current_block++;
            }

            if (opcode == OP_END) {
                if (idx_stk_head < 0) {
                    return false;
                }

                uint32_t block_idx = idx_stk[idx_stk_head--];
                mdle->block_labels[block_idx] = pos - 1;
            }

            WRP_CHECK(check_immediates(opcode, mdle->code_bodies[i], mdle->code_bodies_sz[i], &pos, meta));
        }
    }

    return WRP_SUCCESS;
}

uint32_t wrp_load_mdle(struct wrp_wasm_mdle *mdle,
    uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(load_types(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_imports(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_funcs(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_tables(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_memories(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_globals(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_exports(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_start(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_elements(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_code(buf, buf_sz, meta, mdle));
    WRP_CHECK(load_data(buf, buf_sz, meta, mdle));
    WRP_CHECK(map_blocks(meta, mdle));
    return WRP_SUCCESS;
}
