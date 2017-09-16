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

#include "warp-validation.h"
#include "warp-buf.h"
#include "warp-macros.h"
#include "warp-wasm.h"
#include "warp.h"

static uint32_t validate_preamble(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (buf_sz < 8) {
        return WRP_ERR_MDLE_MISSING_PREAMBLE;
    }

    uint32_t magic_number = 0;
    WRP_CHECK(wrp_read_uint32(buf, buf_sz, pos, &magic_number));

    if (magic_number != WASM_MAGIC_NUMBER) {
        return WRP_ERR_MDLE_INVALID_MAGIC_NUMBER;
    }

    uint32_t wasm_version = 0;
    WRP_CHECK(wrp_read_uint32(buf, buf_sz, pos, &wasm_version));

    if (wasm_version != WASM_VERSION) {
        return WRP_ERR_MDLE_UNSUPPORTED_VERSION;
    }

    return WRP_SUCCESS;
}

static uint32_t validate_type_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &meta->num_types));

    if (meta->num_types > MAX_TYPES) {
        return WRP_ERR_MDLE_TYPE_OVERFLOW;
    }

    for (uint32_t i = 0; i < meta->num_types; i++) {
        meta->types[i] = *pos;

        uint8_t form = 0;
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, pos, &form));

        if (form != TYPE_FUNCTION) {
            return WRP_ERR_MDLE_INVALID_FORM;
        }

        uint32_t num_params = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &num_params));

        if (num_params > MAX_FUNC_PARAMETERS) {
            return WRP_ERR_MDLE_FUNC_PARAMETER_OVERFLOW;
        }

        meta->num_type_params += num_params;

        for (uint32_t j = 0; j < num_params; j++) {
            uint8_t value_type = 0;
            WRP_CHECK(wrp_read_varui7(buf, buf_sz, pos, &value_type));

            if (!wrp_is_valid_value_type(value_type)) {
                return WRP_ERR_INVALID_TYPE;
            }
        }

        uint32_t num_results = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &num_results));

        if (num_results > MAX_FUNC_RESULTS) {
            return WRP_ERR_MDLE_FUNC_RETURN_OVERFLOW;
        }

        meta->num_type_returns += num_results;

        for (uint8_t j = 0; j < num_results; j++) {
            uint8_t value_type = 0;
            WRP_CHECK(wrp_read_varui7(buf, buf_sz, pos, &value_type));

            if (!wrp_is_valid_value_type(value_type)) {
                return WRP_ERR_INVALID_TYPE;
            }
        }
    }

    return WRP_SUCCESS;
}

static uint32_t validate_import_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    return WRP_SUCCESS;
}

static uint32_t validate_func_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &meta->num_funcs));

    if (meta->num_funcs > MAX_FUNCS) {
        return WRP_ERR_MDLE_FUNC_OVERFLOW;
    }

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        meta->funcs[i] = *pos;

        uint32_t type_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &type_idx));

        if (type_idx >= meta->num_types) {
            return WRP_ERR_INVALID_TYPE_IDX;
        }
    }

    return WRP_SUCCESS;
}

static uint32_t validate_table_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &meta->num_tables));

    if (meta->num_tables > MAX_TABLES) {
        return WRP_ERR_MDLE_TABLE_OVERFLOW;
    }

    for (uint32_t i = 0; i < meta->num_tables; i++) {
        meta->tables[i] = *pos;

        uint8_t elem_type = 0;
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, pos, &elem_type));

        if (elem_type != ANY_FUNC) {
            return WRP_ERR_MDLE_INVALID_TABLE_ELEMENT;
        }

        uint32_t min_table_sz = 0;
        uint32_t max_table_sz = 0;
        WRP_CHECK(wrp_read_limits(buf, buf_sz, pos, &min_table_sz, &max_table_sz, MAX_TABLE_SIZE));
    }

    return WRP_SUCCESS;
}

static uint32_t validate_memory_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &meta->num_memories));

    if (meta->num_memories > MAX_MEMORIES) {
        return WRP_ERR_MDLE_MEMORY_OVERFLOW;
    }

    for (uint32_t i = 0; i < meta->num_memories; i++) {
        meta->memories[i] = *pos;

        uint32_t min_pages = 0;
        uint32_t max_pages = 0;
        WRP_CHECK(wrp_read_limits(buf, buf_sz, pos, &min_pages, &max_pages, MAX_MEMORY_SIZE));
    }

    return WRP_SUCCESS;
}

static uint32_t validate_global_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &meta->num_globals));

    if (meta->num_globals > MAX_GLOBALS) {
        return WRP_ERR_MDLE_GLOBAL_OVERFLOW;
    }

    for (uint32_t i = 0; i < meta->num_globals; i++) {
        meta->globals[i] = *pos;

        uint8_t value_type = 0;
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, pos, &value_type));

        if (!wrp_is_valid_value_type(value_type)) {
            return WRP_ERR_INVALID_TYPE;
        }

        uint8_t mutability = 0;
        WRP_CHECK(wrp_read_varui1(buf, buf_sz, pos, &mutability));

        //WASM v1 forbids mutable globals
        if (mutability != GLOBAL_IMMUTABLE) {
            return WRP_ERR_INVALID_MUTIBILITY;
        }
    }

    return WRP_SUCCESS;
}

static uint32_t validate_export_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &meta->num_exports));

    if (meta->num_exports > MAX_EXPORTS) {
        return WRP_ERR_MDLE_EXPORT_OVERFLOW;
    }

    char name[MAX_GLOBAL_NAME_SIZE + 1] = {};

    for (uint32_t i = 0; i < meta->num_exports; i++) {
        meta->exports[i] = *pos;

        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, buf_sz, pos, name, MAX_GLOBAL_NAME_SIZE, &name_len));

        meta->export_name_len += name_len + 1;

        uint8_t kind = 0;
        WRP_CHECK(wrp_read_uint8(buf, buf_sz, pos, &kind));

        //WASM v1 forbids any but func exports
        if (kind != FUNC_EXPORT) {
            return WRP_ERR_INVALID_EXPORT;
        }

        uint32_t func_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &func_idx));

        if (func_idx >= meta->num_funcs) {
            return WRP_ERR_INVALID_FUNC_IDX;
        }
    }

    return WRP_SUCCESS;
}

static uint32_t validate_start_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    meta->start_func_present = true;
    meta->start_func = *pos;

    uint32_t start_func_idx;
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &start_func_idx));

    return WRP_SUCCESS;
}

static uint32_t validate_element_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    return WRP_SUCCESS;
}

static uint32_t validate_immediates(uint8_t opcode,
    uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    switch (opcode) {
    case OP_BLOCK:
    case OP_LOOP:
    case OP_IF:
        int8_t block_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, buf_sz, pos, &block_type));

        if (!wrp_is_valid_block_type(block_type)) {
            return WRP_ERR_INVALID_BLOCK_TYPE;
        }

        break;

    case OP_BR:
    case OP_BR_IF:
        uint32_t relative_depth = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &relative_depth));

        break;

    case OP_BR_TABLE:
        uint32_t target_count = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &target_count));

        for (uint32_t i = 0; i < target_count; i++) {
            uint32_t target = 0;
            WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &target));
        }

        uint32_t default_target = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &default_target));

        break;
    case OP_CALL:
        uint32_t func_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &func_idx));

        if (func_idx >= meta->num_funcs) {
            return WRP_ERR_INVALID_FUNC_IDX;
        }

        break;

    case OP_CALL_INDIRECT:
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
        break;

    case OP_GET_LOCAL:
    case OP_SET_LOCAL:
        uint32_t local_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &local_idx));

        //TODO validate local idx
        break;

    case OP_GET_GLOBAL:
    case OP_SET_GLOBAL:
        uint32_t global_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &global_idx));

        if (global_idx >= meta->num_globals) {
            return WRP_ERR_INVALID_GLOBAL_IDX;
        }
        break;

    case OP_I32_LOAD:
    case OP_I64_LOAD:
    case OP_F32_LOAD:
    case OP_F64_LOAD:
    case OP_I32_LOAD_8_S:
    case OP_I32_LOAD_8_U:
    case OP_I32_LOAD_16_S:
    case OP_I32_LOAD_16_U:
    case OP_I64_LOAD_8_S:
    case OP_I64_LOAD_8_U:
    case OP_I64_LOAD_16_S:
    case OP_I64_LOAD_16_U:
    case OP_I64_LOAD_32_S:
    case OP_I64_LOAD_32_U:
    case OP_I32_STORE:
    case OP_I64_STORE:
    case OP_F32_STORE:
    case OP_F64_STORE:
    case OP_I32_STORE_8:
    case OP_I32_STORE_16:
    case OP_I64_STORE_8:
    case OP_I64_STORE_16:
    case OP_I64_STORE_32:
        uint32_t memory_immediate_flags = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &memory_immediate_flags));

        uint32_t memory_immediate_offset = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &memory_immediate_offset));

        //TODO validate memory immediate

        break;
    case OP_CURRENT_MEMORY:
    case OP_GROW_MEMORY:
        int8_t memory_reserved = 0;
        WRP_CHECK(wrp_read_vari7(buf, buf_sz, pos, &memory_reserved));

        if (memory_reserved != 0) {
            return WRP_ERR_INVALID_RESERVED;
        }

    case OP_I32_CONST:
        int32_t i32_const = 0;
        WRP_CHECK(wrp_read_vari32(buf, buf_sz, pos, &i32_const));

    case OP_I64_CONST:
        int64_t i64_const = 0;
        WRP_CHECK(wrp_read_vari64(buf, buf_sz, pos, &i64_const));

    case OP_F32_CONST:
        float f32_const = 0;
        WRP_CHECK(wrp_read_f32(buf, buf_sz, pos, &f32_const));

    case OP_F64_CONST:
        double f64_const = 0;
        WRP_CHECK(wrp_read_f64(buf, buf_sz, pos, &f64_const));

    default:
        break;
    }

    return WRP_SUCCESS;
}

static uint32_t validate_code_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    uint32_t num_code_segments = 0;
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &num_code_segments));

    if (num_code_segments != meta->num_funcs) {
        return WRP_ERR_MDLE_CODE_MISMATCH;
    }

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        meta->codes[i] = *pos;

        uint32_t body_sz;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &body_sz));

        uint32_t num_local_entry = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &num_local_entry));

        uint32_t total_locals = 0;
        for (uint32_t j = 0; j < num_local_entry; j++) {
            uint32_t num_locals = 0;
            WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &num_locals));

            uint8_t value_type = 0;
            WRP_CHECK(wrp_read_varui7(buf, buf_sz, pos, &value_type));

            if (!wrp_is_valid_value_type(value_type)) {
                return WRP_ERR_INVALID_TYPE;
            }

            total_locals += num_locals;
        }

        if (total_locals > MAX_LOCALS) {
            return WRP_ERR_MDLE_LOCALS_OVERFLOW;
        }

        size_t code_sz = body_sz - (*pos - meta->codes[i]);
        uint32_t num_blocks = 0;
        uint32_t num_ends = 0;
        size_t end_pos = *pos + code_sz;

        meta->num_code_locals += total_locals;
        meta->code_body_sz += code_sz;

        while (*pos <= end_pos) {
            uint8_t opcode = 0;
            WRP_CHECK(wrp_read_uint8(buf, buf_sz, pos, &opcode));
            WRP_CHECK(validate_immediates(opcode, buf, buf_sz, pos, meta));

            if (opcode == OP_BLOCK || opcode == OP_IF || opcode == OP_ELSE || opcode == OP_LOOP) {
                num_blocks++;
            }

            if (opcode == OP_END) {
                num_ends++;
            }

            if (*pos > end_pos && opcode != OP_END) {
                return WRP_ERR_MDLE_INVALID_END_OPCODE;
            }
        }

        if (num_ends != num_blocks + 1) {
            return WPR_ERR_MDLE_BLOCK_END_MISMATCH;
        }

        meta->num_blocks += num_blocks;
    }

    return WRP_SUCCESS;
}

static uint32_t validate_data_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    return WRP_SUCCESS;
}

uint32_t wrp_validate_mdle(uint8_t *buf, size_t buf_sz, struct wrp_wasm_meta *meta)
{
    size_t pos = 0;

    WRP_CHECK(validate_preamble(buf, buf_sz, &pos, meta))

    uint32_t prev_section_id = 0;

    while (pos < buf_sz) {
        uint8_t section_id = 0;
        WRP_CHECK(wrp_read_varui7(buf, buf_sz, &pos, &section_id));

        uint32_t section_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, &pos, &section_sz));

        if (section_id > SECTION_DATA) {
            return WRP_ERR_MDLE_INVALID_SECTION_ID;
        }

        if (section_id > 0 && section_id <= prev_section_id) {
            return WRP_ERR_MDLE_SECTION_ORDER;
        }

        switch (section_id) {
        case SECTION_CUSTOM:
            WRP_CHECK(wrp_skip(buf, buf_sz, &pos, section_sz))
            break;

        case SECTION_TYPE:
            WRP_CHECK(validate_type_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_IMPORT:

            //TODO
            // if (!validate_data_section(buf, buf_sz, &pos, meta)) {
            //     return false;
            // }

            WRP_CHECK(wrp_skip(buf, buf_sz, &pos, section_sz));
            break;

        case SECTION_FUNC:
            WRP_CHECK(validate_func_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_TABLE:
            WRP_CHECK(validate_table_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_MEMORY:
            WRP_CHECK(validate_memory_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_GLOBAL:
            WRP_CHECK(validate_global_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_EXPORT:
            WRP_CHECK(validate_export_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_START:
            WRP_CHECK(validate_start_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_ELEMENT:

            //TODO
            // if (!validate_element_section(buf, buf_sz, &pos, meta)) {
            //     return false;
            // }

            WRP_CHECK(wrp_skip(buf, buf_sz, &pos, section_sz));
            break;

        case SECTION_CODE:
            WRP_CHECK(validate_code_section(buf, buf_sz, &pos, meta));
            break;

        case SECTION_DATA:

            //TODO
            // if (!validate_data_section(buf, buf_sz, &pos, meta)) {
            //     return false;
            // }

            WRP_CHECK(wrp_skip(buf, buf_sz, &pos, section_sz));
            break;

        default:
            break;
        }

        prev_section_id = section_id;
    }

    if(!wrp_end_of_buf(buf, buf_sz, pos)){
        return WRP_ERR_MDLE_INVALID_BYTES;
    }

    return WRP_SUCCESS;
}
