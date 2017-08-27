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

#include "wasm-load.h"
#include "buf-io.h"
#include "wasm.h"

#define ALIGN_64(x) (((x + 63) / 64) * 64)

static size_t calculate_module_size(struct wasm_meta *meta)
{
    size_t module_sz = sizeof(struct wasm_module);
    module_sz += ALIGN_64(meta->num_types * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_type_params * sizeof(uint8_t *));
    module_sz += ALIGN_64(meta->num_types * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_types * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_type_returns * sizeof(uint8_t *));
    module_sz += ALIGN_64(meta->num_types * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_types * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_funcs * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_globals * sizeof(uint64_t));
    module_sz += ALIGN_64(meta->num_globals * sizeof(uint8_t));
    module_sz += ALIGN_64(meta->export_name_len * sizeof(char));
    module_sz += ALIGN_64(meta->num_exports * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_exports * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_code_locals * sizeof(uint8_t));
    module_sz += ALIGN_64(meta->num_code_segments * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->num_code_segments * sizeof(uint32_t));
    module_sz += ALIGN_64(meta->code_body_sz * sizeof(uint8_t));
    module_sz += ALIGN_64(meta->num_code_segments * sizeof(size_t));
    return module_sz;
}

static void initialise_module(struct wasm_meta *meta, struct wasm_module *mdle)
{
    uint8_t *ptr = (uint8_t *)mdle;
    size_t offset = sizeof(struct wasm_module);

    mdle->forms = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->param_types = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_type_params * sizeof(uint8_t));

    mdle->param_type_offsets = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->param_counts = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->result_types = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_type_returns * sizeof(uint8_t));

    mdle->result_type_offsets = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->result_counts = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_types * sizeof(uint32_t));

    mdle->func_type_idxs = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_funcs * sizeof(uint32_t));

    mdle->global_values = (uint64_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_globals * sizeof(uint64_t));

    mdle->global_types = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_globals * sizeof(uint8_t));

    mdle->export_names = (char *)(ptr + offset);
    offset += ALIGN_64(meta->export_name_len * sizeof(char));

    mdle->export_name_offsets = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_exports * sizeof(uint32_t));

    mdle->export_func_idxs = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_exports * sizeof(uint32_t));

    mdle->local_types = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_code_locals * sizeof(uint8_t));

    mdle->local_type_offsets = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_code_segments * sizeof(uint32_t));

    mdle->local_counts = (uint32_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_code_segments * sizeof(uint32_t));

    mdle->code = (uint8_t *)(ptr + offset);
    offset += ALIGN_64(meta->code_body_sz * sizeof(uint8_t));

    mdle->start_instructions = (size_t *)(ptr + offset);
    offset += ALIGN_64(meta->num_code_segments * sizeof(size_t));
}

static void load_types(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
    mdle->num_types = meta->num_types;
    uint32_t current_param = 0;
    uint32_t current_result = 0;

    for (uint32_t i = 0; i < meta->num_types; i++) {
        size_t pos = 0;
        wrp_seek(buf, buf_sz, &pos, meta->types[i]);

        mdle->forms[i] = wrp_get_varui7(buf, &pos);
        mdle->param_counts[i] = wrp_get_varui32(buf, &pos);
        mdle->param_type_offsets[i] = current_param;

        for (uint32_t j = 0; j < mdle->param_counts[i]; j++) {
            mdle->param_types[current_param++] = wrp_get_varui7(buf, &pos);
        }

        mdle->result_counts[i] = wrp_get_varui32(buf, &pos);
        mdle->result_type_offsets[i] = current_result;

        for (uint32_t j = 0; j < mdle->result_counts[i]; j++) {
            mdle->result_types[current_result++] = wrp_get_varui7(buf, &pos);
        }
    }
}

static void load_imports(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
}

static void load_funcs(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
    mdle->num_funcs = meta->num_funcs;

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        size_t pos = 0;
        wrp_seek(buf, buf_sz, &pos, meta->funcs[i]);
        mdle->func_type_idxs[i] = wrp_get_varui32(buf, &pos);
    }
}

static void load_tables(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
}

static void load_memories(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
}

static void load_globals(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
    mdle->num_globals = meta->num_globals;

    for (uint32_t i = 0; i < meta->num_globals; i++) {
        size_t pos = 0;
        wrp_seek(buf, buf_sz, &pos, meta->globals[i]);

        mdle->global_values[i] = 0;
        mdle->global_types[i] = wrp_get_varui7(buf, &pos);

        //skip mutibility
        wrp_get_varui1(buf, &pos);
    }
}

static void load_exports(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
    mdle->num_exports = meta->num_exports;
    uint32_t current_char = 0;

    for (uint32_t i = 0; i < meta->num_exports; i++) {
        size_t pos = 0;
        wrp_seek(buf, buf_sz, &pos, meta->exports[i]);

        mdle->export_name_offsets[i] = current_char;
        char *name = &mdle->export_names[current_char];
        uint32_t name_len = 0;
        wrp_read_string(buf, buf_sz, &pos, name, MAX_GLOBAL_NAME_SIZE, &name_len);

        current_char += name_len + 1;

        //skip kind
        wrp_get_varui7(buf, &pos);

        mdle->export_func_idxs[i] = wrp_get_varui32(buf, &pos);
    }
}

static void load_start(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
    if (!meta->start_func_present) {
        return;
    }

    size_t pos = 0;
    wrp_seek(buf, buf_sz, &pos, meta->start_func);

    mdle->start_func_idx = wrp_get_varui32(buf, &pos);
    mdle->start_func_present = true;
}

static void load_elements(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
}

static void load_code(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
    mdle->num_code_segments = meta->num_code_segments;

    uint32_t current_local = 0;
    uint8_t current_instruction = 0;

    for (uint32_t i = 0; i < meta->num_code_segments; i++) {
        size_t pos = 0;
        wrp_seek(buf, buf_sz, &pos, meta->code[i]);

        mdle->local_type_offsets[i] = current_local;

        uint32_t body_sz = wrp_get_varui32(buf, &pos);
        uint32_t num_local_entries = wrp_get_varui32(buf, &pos);

        for (uint32_t j = 0; j < num_local_entries; j++) {
            uint32_t num_locals = wrp_get_varui32(buf, &pos);
            uint8_t value_type = wrp_get_vari7(buf, &pos);

            for (uint32_t k = 0; k < num_locals; k++) {
                mdle->local_types[current_local++] = value_type;
            }

            mdle->local_counts[i] += num_locals;
        }

        mdle->start_instructions[i] = pos;
        size_t count = body_sz - (pos - meta->code[i]);

        while (count--) {
            mdle->code[current_instruction++] = wrp_get_uint8(buf, &pos);
        }
    }
}

static void load_data(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    struct wasm_module *mdle)
{
}

struct wasm_module *wrp_load_module(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    warp_alloc_fn alloc_fn)
{
    size_t module_sz = calculate_module_size(meta);

    struct wasm_module *mdle = alloc_fn(module_sz, 64);

    if (mdle == NULL) {
        return NULL;
    }

    initialise_module(meta, mdle);
    load_types(buf, buf_sz, meta, mdle);
    load_imports(buf, buf_sz, meta, mdle);
    load_funcs(buf, buf_sz, meta, mdle);
    load_tables(buf, buf_sz, meta, mdle);
    load_memories(buf, buf_sz, meta, mdle);
    load_globals(buf, buf_sz, meta, mdle);
    load_exports(buf, buf_sz, meta, mdle);
    load_start(buf, buf_sz, meta, mdle);
    load_elements(buf, buf_sz, meta, mdle);
    load_code(buf, buf_sz, meta, mdle);
    load_data(buf, buf_sz, meta, mdle);

    return mdle;
}

void wrp_unload_module(struct wasm_module *mdle, warp_free_fn free_fn)
{
    free_fn(mdle);
}
