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
#include "warp-expr.h"
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
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_types = count;

    for (uint32_t i = 0; i < count; i++) {

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
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_imports = count;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_varui32(buf, &name_len));
        WRP_CHECK(wrp_skip(buf, name_len));

        out_meta->import_name_buf_sz += name_len + 1;

        uint32_t field_len = 0;
        WRP_CHECK(wrp_read_varui32(buf, &field_len));
        WRP_CHECK(wrp_skip(buf, field_len));

        out_meta->import_field_buf_sz += name_len + 1;

        uint8_t kind = 0;
        WRP_CHECK(wrp_read_uint8(buf, &kind));

        if (kind == EXTERNAL_FUNC) {
            out_meta->num_funcs++;

            uint32_t type_idx = 0;
            WRP_CHECK(wrp_read_varui32(buf, &type_idx));
        } else if (kind == EXTERNAL_TABLE) {
            out_meta->num_tables++;

            uint8_t elem_type = 0;
            WRP_CHECK(wrp_read_varui7(buf, &elem_type));

            uint32_t min_table_sz = 0;
            uint32_t max_table_sz = MAX_TABLE_SIZE;
            WRP_CHECK(wrp_read_limits(buf, &min_table_sz, &max_table_sz));
        } else if (kind == EXTERNAL_MEMORY) {
            out_meta->num_memories++;

            uint32_t min_pages = 0;
            uint32_t max_pages = MAX_MEMORY_PAGES;
            WRP_CHECK(wrp_read_limits(buf, &min_pages, &max_pages));
        } else if (kind == EXTERNAL_GLOBAL) {
            out_meta->num_globals++;

            int8_t value_type = 0;
            WRP_CHECK(wrp_read_vari7(buf, &value_type));

            uint8_t mutability = 0;
            WRP_CHECK(wrp_read_varui1(buf, &mutability));
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_func_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_funcs += count;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t type_idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &type_idx));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_table_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_tables += count;

    for (uint32_t i = 0; i < count; i++) {
        int8_t elem_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, &elem_type));

        uint32_t min_table_elem = 0;
        uint32_t max_table_elem = MAX_TABLE_SIZE;
        WRP_CHECK(wrp_read_limits(buf, &min_table_elem, &max_table_elem));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_memory_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_memories += count;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t min_pages = 0;
        uint32_t max_pages = MAX_MEMORY_PAGES;
        WRP_CHECK(wrp_read_limits(buf, &min_pages, &max_pages));
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_global_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_globals += count;

    for (uint32_t i = 0; i < count; i++) {
        int8_t value_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, &value_type));

        uint8_t mutability = 0;
        WRP_CHECK(wrp_read_varui1(buf, &mutability));

        //TODO skip init expr
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_export_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_exports = count;
    char name[MAX_GLOBAL_NAME_SIZE + 1] = {0};

    for (uint32_t i = 0; i < count; i++) {
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, name, MAX_GLOBAL_NAME_SIZE + 1, &name_len));

        out_meta->export_name_buf_sz += name_len + 1;

        uint8_t kind = 0;
        WRP_CHECK(wrp_read_uint8(buf, &kind));

        uint32_t idx = 0;
        WRP_CHECK(wrp_read_varui32(buf, &idx));
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
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_elem_segments = count;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t table_idx = 0;
        size_t expr_sz = 0;
        uint32_t num_elems = 0;
        WRP_CHECK(wrp_read_varui32(buf, &table_idx));
        WRP_CHECK(wrp_skip_init_expr(buf, &expr_sz));
        WRP_CHECK(wrp_read_varui32(buf, &num_elems));

        for(uint32_t j = 0; j < num_elems; j++){
            uint32_t func_idx = 0;
            WRP_CHECK(wrp_read_varui32(buf, &func_idx));
        }

        out_meta->elem_expr_buf_sz += expr_sz;
        out_meta->num_elem += num_elems;
    }

    return WRP_SUCCESS;
}

static wrp_err_t scan_code_section(wrp_buf_t *buf, wrp_wasm_meta_t *out_meta)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = 0; i < count; i++) {
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
            size_t expr_sz = 0;
            WRP_CHECK(wrp_skip_expr(buf, &opcode, &expr_sz));

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
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_meta->num_data_segments = count;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t mem_idx = 0;
        size_t expr_sz = 0;
        uint32_t data_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, &mem_idx));
        WRP_CHECK(wrp_skip_init_expr(buf, &expr_sz));
        WRP_CHECK(wrp_read_varui32(buf, &data_sz));

        out_meta->data_expr_buf_sz += expr_sz;
        out_meta->data_buf_sz += data_sz;

        WRP_CHECK(wrp_skip(buf, data_sz));
    }

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
            WRP_CHECK(scan_import_section(buf, out_meta));
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
            WRP_CHECK(scan_element_section(buf, out_meta));
            break;

        case SECTION_CODE:
            WRP_CHECK(scan_code_section(buf, out_meta));
            break;

        case SECTION_DATA:
            WRP_CHECK(scan_data_section(buf, out_meta));
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
