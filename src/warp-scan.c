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

#include "warp-scan.h"
#include "warp-buf.h"
#include "warp-config.h"
#include "warp-error.h"
#include "warp-macros.h"
#include "warp-wasm.h"

static wrp_err_t scan_preamble(wrp_buf_t *buf)
{
    uint32_t magic_number = 0;
    WRP_CHECK(wrp_read_uint32(buf, &magic_number));

    uint32_t wasm_version = 0;
    WRP_CHECK(wrp_read_uint32(buf, &wasm_version));

    return WRP_SUCCESS;
}

static wrp_err_t scan_type_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_meta->num_types));

    for (uint32_t i = 0; i < out_meta->num_types; i++) {

        uint8_t form = 0;
        WRP_CHECK(wrp_read_varui7(buf, &form));

        uint32_t num_params = 0;
        WRP_CHECK(wrp_read_varui32(buf, &num_params));

        out_meta->num_type_params += num_params;

        for (uint32_t j = 0; j < num_params; j++) {
            int8_t value_type = 0;
            WRP_CHECK(wrp_read_vari7(buf, &value_type));
        }

        uint32_t num_results = 0;
        WRP_CHECK(wrp_read_varui32(buf, &num_results));

        out_meta->num_type_returns += num_results;

        for (uint8_t j = 0; j < num_results; j++) {
            int8_t value_type = 0;
            WRP_CHECK(wrp_read_vari7(buf, &value_type));
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_import_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    return WRP_SUCCESS;
}

static wrp_err_t scan_func_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_meta->num_funcs));

    for (uint32_t i = 0; i < out_meta->num_funcs; i++) {
        uint32_t type_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &type_idx));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_table_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_meta->num_tables));

    for (uint32_t i = 0; i < out_meta->num_tables; i++) {
        uint8_t elem_type = 0;
        WRP_CHECK(wrp_read_varui7(buf, &elem_type));

        uint32_t min_table_sz = 0;
        uint32_t max_table_sz = MAX_TABLE_SIZE;
        WRP_CHECK(wrp_read_limits(buf, &min_table_sz, &max_table_sz));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_memory_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_meta->num_memories));

    for (uint32_t i = 0; i < out_meta->num_memories; i++) {
        uint32_t min_pages = 0;
        uint32_t max_pages = MAX_MEMORY_SIZE;
        WRP_CHECK(wrp_read_limits(buf, &min_pages, &max_pages));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_global_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_meta->num_globals));

    for (uint32_t i = 0; i < out_meta->num_globals; i++) {
        int8_t value_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, &value_type));

        uint8_t mutability = 0;
        WRP_CHECK(wrp_read_varui1(buf, &mutability));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_export_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_meta->num_exports));

    char name[MAX_GLOBAL_NAME_SIZE + 1] = {0};

    for (uint32_t i = 0; i < out_meta->num_exports; i++) {
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, name, MAX_GLOBAL_NAME_SIZE + 1, &name_len));

        out_meta->export_name_buf_sz += name_len + 1;

        uint8_t kind = 0;
        WRP_CHECK(wrp_read_uint8(buf, &kind));

        uint32_t func_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &func_idx));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_start_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t start_func_idx;
    WRP_CHECK(wrp_read_varui32(buf, &start_func_idx));
    return WRP_SUCCESS;
}

static wrp_err_t scan_element_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    return WRP_SUCCESS;
}

static wrp_err_t skip_immediates(wrp_buf_t *buf, uint8_t opcode)
{
    if (opcode >= OP_BLOCK && opcode <= OP_IF) {
        int8_t block_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, &block_type));
    } else if (opcode >= OP_BR && opcode <= OP_BR_IF) {
        uint32_t depth = 0;
        WRP_CHECK(wrp_read_varui32(buf, &depth));
    } else if (opcode == OP_BR_TABLE) {
        uint32_t target_count = 0;
        WRP_CHECK(wrp_read_varui32(buf, &target_count));

        for (uint32_t i = 0; i < target_count; i++) {
            uint32_t target = 0;
            WRP_CHECK(wrp_read_varui32(buf, &target));
        }

        uint32_t default_target = 0;
        WRP_CHECK(wrp_read_varui32(buf, &default_target));
    } else if (opcode == OP_CALL) {
        uint32_t func_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &func_idx));
    } else if (opcode == OP_CALL_INDIRECT) {
        uint32_t type_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &type_idx));
        int8_t indirect_reserved = 0;
        WRP_CHECK(wrp_read_vari7(buf, &indirect_reserved));
    } else if (opcode >= OP_GET_LOCAL && opcode <= OP_SET_LOCAL) {
        uint32_t local_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &local_idx));
    } else if (opcode >= OP_GET_GLOBAL && opcode <= OP_SET_GLOBAL) {
        uint32_t global_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &global_idx));
    } else if (opcode >= OP_I32_LOAD && opcode <= OP_I64_STORE_32) {
        uint32_t memory_immediate_flags = 0;
        WRP_CHECK(wrp_read_varui32(buf, &memory_immediate_flags));
        uint32_t memory_immediate_offset = 0;
        WRP_CHECK(wrp_read_varui32(buf, &memory_immediate_offset));
    } else if (opcode >= OP_CURRENT_MEMORY && opcode <= OP_GROW_MEMORY) {
        int8_t memory_reserved = 0;
        WRP_CHECK(wrp_read_vari7(buf, &memory_reserved));
    } else if (opcode == OP_I32_CONST) {
        int32_t i32_const = 0;
        WRP_CHECK(wrp_read_vari32(buf, &i32_const));
    } else if (opcode == OP_I64_CONST) {
        int64_t i64_const = 0;
        WRP_CHECK(wrp_read_vari64(buf, &i64_const));
    } else if (opcode == OP_F32_CONST) {
        float f32_const = 0;
        WRP_CHECK(wrp_read_f32(buf, &f32_const));
    } else if (opcode == OP_F64_CONST) {
        double f64_const = 0;
        WRP_CHECK(wrp_read_f64(buf, &f64_const));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_code_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t num_code_segments = 0;
    WRP_CHECK(wrp_read_varui32(buf, &num_code_segments));

    for (uint32_t i = 0; i < out_meta->num_funcs; i++) {
        size_t code_segment_pos = buf->pos;

        uint32_t body_sz;
        WRP_CHECK(wrp_read_varui32(buf, &body_sz));

        uint32_t num_local_entry = 0;
        WRP_CHECK(wrp_read_varui32(buf, &num_local_entry));

        uint32_t total_locals = 0;
        for (uint32_t j = 0; j < num_local_entry; j++) {
            uint32_t num_locals = 0;
            WRP_CHECK(wrp_read_varui32(buf, &num_locals));

            int8_t value_type = 0;
            WRP_CHECK(wrp_read_vari7(buf, &value_type));

            if (!wrp_is_valid_value_type(value_type)) {
                return WRP_ERR_INVALID_TYPE;
            }

            total_locals += num_locals;
        }

        size_t code_sz = body_sz - (buf->pos - code_segment_pos) + 1;
        size_t end_pos = buf->pos + code_sz - 1;

        out_meta->num_code_locals += total_locals;
        out_meta->code_buf_sz += code_sz;

        while (buf->pos <= end_pos) {
            uint8_t opcode = 0;
            WRP_CHECK(wrp_read_uint8(buf, &opcode));
            WRP_CHECK(skip_immediates(buf, opcode));

            if (opcode == OP_IF) {
                out_meta->num_if_ops++;
            }

            if (opcode == OP_BLOCK) {
                out_meta->num_block_ops++;
            }
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_data_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    return WRP_SUCCESS;
}

wrp_err_t wrp_scan_mdle(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    buf->pos = 0;

    WRP_CHECK(scan_preamble(buf));

    while (buf->pos < buf->sz) {
        uint8_t section_id = 0;
        WRP_CHECK(wrp_read_varui7(buf, &section_id));

        uint32_t section_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, &section_sz));

        switch (section_id) {
        case SECTION_CUSTOM:
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_TYPE:
            WRP_CHECK(scan_type_section(buf, out_meta));
            break;

        case SECTION_IMPORT:
            //WRP_CHECK(scan_import_section(buf, out_meta));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_FUNC:
            WRP_CHECK(scan_func_section(buf, out_meta));
            break;

        case SECTION_TABLE:
            WRP_CHECK(scan_table_section(buf, out_meta));
            break;

        case SECTION_MEMORY:
            WRP_CHECK(scan_memory_section(buf, out_meta));
            break;

        case SECTION_GLOBAL:
            WRP_CHECK(scan_global_section(buf, out_meta));
            break;

        case SECTION_EXPORT:
            WRP_CHECK(scan_export_section(buf, out_meta));
            break;

        case SECTION_START:
            WRP_CHECK(scan_start_section(buf, out_meta));
            break;

        case SECTION_ELEMENT:
            //WRP_CHECK(scan_element_section(buf, out_meta));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_CODE:
            WRP_CHECK(scan_code_section(buf, out_meta));
            break;

        case SECTION_DATA:
            //WRP_CHECK(scan_data_section(buf, out_meta));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        default:
            return WRP_ERR_MDLE_INVALID_SECTION_ID;
        }
    }

    if (!wrp_end_of_buf(buf)) {
        return WRP_ERR_MDLE_INVALID_BYTES;
    }

    return WRP_SUCCESS;
}
