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

#include "warp-error.h"
#include "warp-load.h"
#include "warp-macros.h"
#include "warp-type-check.h"
#include "warp-wasm.h"
#include "warp.h"

static wrp_err_t check_preamble(wrp_buf_t *buf)
{
    if (buf->sz < 8) {
        return WRP_ERR_MDLE_MISSING_PREAMBLE;
    }

    uint32_t magic_number = 0;
    WRP_CHECK(wrp_read_uint32(buf, &magic_number));

    if (magic_number != WASM_MAGIC_NUMBER) {
        return WRP_ERR_MDLE_INVALID_MAGIC_NUMBER;
    }

    uint32_t wasm_version = 0;
    WRP_CHECK(wrp_read_uint32(buf, &wasm_version));

    if (wasm_version != WASM_VERSION) {
        return WRP_ERR_MDLE_UNSUPPORTED_VERSION;
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_type_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_mdle->num_types));

    uint32_t current_param = 0;
    uint32_t current_result = 0;

    for (uint32_t i = 0; i < out_mdle->num_types; i++) {
        WRP_CHECK(wrp_read_varui7(buf, &out_mdle->forms[i]));

        if (out_mdle->forms[i] != TYPE_FUNCTION) {
            return WRP_ERR_MDLE_INVALID_FORM;
        }

        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->param_counts[i]));

        if (out_mdle->param_counts[i] > MAX_FUNC_PARAMETERS) {
            return WRP_ERR_MDLE_FUNC_PARAMETER_OVERFLOW;
        }

        out_mdle->param_type_offsets[i] = current_param;

        for (uint32_t j = 0; j < out_mdle->param_counts[i]; j++) {
            WRP_CHECK(wrp_read_vari7(buf, &out_mdle->param_types[current_param]));

            if (!wrp_is_valid_value_type(out_mdle->param_types[current_param])) {
                return WRP_ERR_INVALID_TYPE;
            }

            current_param++;
        }

        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->result_counts[i]));

        if (out_mdle->result_counts[i] > MAX_FUNC_RESULTS) {
            return WRP_ERR_MDLE_FUNC_RETURN_OVERFLOW;
        }

        out_mdle->result_type_offsets[i] = current_result;

        for (uint32_t j = 0; j < out_mdle->result_counts[i]; j++) {
            WRP_CHECK(wrp_read_vari7(buf, &out_mdle->result_types[current_result]));

            if (!wrp_is_valid_value_type(out_mdle->result_types[current_result])) {
                return WRP_ERR_INVALID_TYPE;
            }

            current_result++;
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_import_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    return WRP_SUCCESS;
}

static wrp_err_t load_func_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_mdle->num_funcs));

    for (uint32_t i = 0; i < out_mdle->num_funcs; i++) {
        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->func_type_idxs[i]));

        if (out_mdle->func_type_idxs[i] >= out_mdle->num_types) {
            return WRP_ERR_INVALID_TYPE_IDX;
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_table_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    return WRP_SUCCESS;
}

static wrp_err_t load_memory_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    return WRP_SUCCESS;
}

static wrp_err_t load_global_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_mdle->num_globals));

    for (uint32_t i = 0; i < out_mdle->num_globals; i++) {

        out_mdle->global_values[i] = 0;
        WRP_CHECK(wrp_read_vari7(buf, &out_mdle->global_types[i]));

        if (!wrp_is_valid_value_type(out_mdle->global_types[i])) {
            return WRP_ERR_INVALID_TYPE;
        }

        uint8_t mutability = 0;
        WRP_CHECK(wrp_read_varui1(buf, &mutability));

        //WASM v1 forbids mutable globals
        if (mutability != GLOBAL_IMMUTABLE) {
            return WRP_ERR_INVALID_MUTIBILITY;
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_export_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_mdle->num_exports));

    uint32_t current_char = 0;

    for (uint32_t i = 0; i < out_mdle->num_exports; i++) {

        out_mdle->export_name_offsets[i] = current_char;
        char *name = &out_mdle->export_names[current_char];
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, name, MAX_GLOBAL_NAME_SIZE, &name_len));

        current_char += name_len + 1;

        uint8_t kind = 0;
        WRP_CHECK(wrp_read_varui7(buf, &kind));

        //WASM v1 forbids any but func exports
        if (kind != FUNC_EXPORT) {
            return WRP_ERR_INVALID_EXPORT;
        }

        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->export_func_idxs[i]));

        if (out_mdle->export_func_idxs[i] >= out_mdle->num_funcs) {
            return WRP_ERR_INVALID_FUNC_IDX;
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_start_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_mdle->start_func_idx));
    out_mdle->start_func_present = true;
    return WRP_SUCCESS;
}

static wrp_err_t load_element_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    return WRP_SUCCESS;
}

static wrp_err_t load_code_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t num_code_segments = 0;
    WRP_CHECK(wrp_read_varui32(buf, &num_code_segments));

    if (num_code_segments != out_mdle->num_funcs) {
        return WRP_ERR_MDLE_CODE_MISMATCH;
    }

    uint32_t current_local = 0;
    size_t code_pos = 0;

    for (uint32_t i = 0; i < out_mdle->num_funcs; i++) {
        size_t code_segment_pos = buf->pos;
        out_mdle->local_type_offsets[i] = current_local;

        uint32_t body_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, &body_sz));

        uint32_t num_local_entries = 0;
        WRP_CHECK(wrp_read_varui32(buf, &num_local_entries));

        for (uint32_t j = 0; j < num_local_entries; j++) {
            uint32_t num_locals = 0;
            WRP_CHECK(wrp_read_varui32(buf, &num_locals));

            int8_t value_type;
            WRP_CHECK(wrp_read_vari7(buf, &value_type));

            if (!wrp_is_valid_value_type(value_type)) {
                return WRP_ERR_INVALID_TYPE;
            }

            for (uint32_t k = 0; k < num_locals; k++) {
                out_mdle->local_types[current_local++] = value_type;
            }

            out_mdle->local_counts[i] += num_locals;
        }

        size_t code_sz = body_sz - (buf->pos - code_segment_pos) + 1;

        out_mdle->code_bodies[i] = &out_mdle->code[code_pos];
        out_mdle->code_bodies_sz[i] = code_sz;

        memcpy(&out_mdle->code[code_pos], &buf->bytes[buf->pos], code_sz);

        buf->pos += code_sz;
        code_pos += code_sz;
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_data_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    return WRP_SUCCESS;
}

wrp_err_t wrp_load_mdle(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    buf->pos = 0;

    WRP_CHECK(check_preamble(buf));

    uint32_t prev_section_id = 0;

    while (buf->pos < buf->sz) {
        uint8_t section_id = 0;
        WRP_CHECK(wrp_read_varui7(buf, &section_id));

        uint32_t section_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, &section_sz));

        if (section_id > SECTION_DATA) {
            return WRP_ERR_MDLE_INVALID_SECTION_ID;
        }

        if (section_id > 0 && section_id <= prev_section_id) {
            return WRP_ERR_MDLE_SECTION_ORDER;
        }

        switch (section_id) {
        case SECTION_CUSTOM:
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_TYPE:
            WRP_CHECK(load_type_section(buf, out_mdle));
            break;

        case SECTION_IMPORT:
            //WRP_CHECK(load_import_section(buf, out_mdle));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_FUNC:
            WRP_CHECK(load_func_section(buf, out_mdle));
            break;

        case SECTION_TABLE:
            WRP_CHECK(load_table_section(buf, out_mdle));
            break;

        case SECTION_MEMORY:
            // WRP_CHECK(load_memory_section(buf, out_mdle));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_GLOBAL:
            WRP_CHECK(load_global_section(buf, out_mdle));
            break;

        case SECTION_EXPORT:
            WRP_CHECK(load_export_section(buf, out_mdle));
            break;

        case SECTION_START:
            WRP_CHECK(load_start_section(buf, out_mdle));
            break;

        case SECTION_ELEMENT:
            //WRP_CHECK(load_element_section(buf, out_mdle));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        case SECTION_CODE:
            WRP_CHECK(load_code_section(buf, out_mdle));
            break;

        case SECTION_DATA:
            //WRP_CHECK(load_data_section(buf, out_meta));
            WRP_CHECK(wrp_skip(buf, section_sz));
            break;

        default:
            break;
        }

        prev_section_id = section_id;
    }

    if (!wrp_end_of_buf(buf)) {
        return WRP_ERR_MDLE_INVALID_BYTES;
    }

    return WRP_SUCCESS;
}
