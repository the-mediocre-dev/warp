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
#include "warp-initializer-expression.h"
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
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_mdle->num_types = count;
    uint32_t current_param = 0;
    uint32_t current_result = 0;

    for (uint32_t i = 0; i < count; i++) {
        WRP_CHECK(wrp_read_varui7(buf, &out_mdle->types[i].form));

        if (out_mdle->types[i].form != TYPE_FUNCTION) {
            return WRP_ERR_MDLE_INVALID_FORM;
        }

        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->types[i].num_params));

        if (out_mdle->types[i].num_params > MAX_FUNC_PARAMETERS) {
            return WRP_ERR_MDLE_FUNC_PARAMETER_OVERFLOW;
        }

        out_mdle->types[i].param_types = &out_mdle->param_type_buf[current_param];

        for (uint32_t j = 0; j < out_mdle->types[i].num_params; j++) {
            WRP_CHECK(wrp_read_vari7(buf, &out_mdle->param_type_buf[current_param]));

            if (!wrp_is_valid_value_type(out_mdle->param_type_buf[current_param])) {
                return WRP_ERR_INVALID_TYPE;
            }

            current_param++;
        }

        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->types[i].num_results));

        if (out_mdle->types[i].num_results > MAX_FUNC_RESULTS) {
            return WRP_ERR_MDLE_FUNC_RETURN_OVERFLOW;
        }

        out_mdle->types[i].result_types = &out_mdle->result_type_buf[current_result];

        for (uint32_t j = 0; j < out_mdle->types[i].num_results; j++) {
            WRP_CHECK(wrp_read_vari7(buf, &out_mdle->result_type_buf[current_result]));

            if (!wrp_is_valid_value_type(out_mdle->result_type_buf[current_result])) {
                return WRP_ERR_INVALID_TYPE;
            }

            current_result++;
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_import_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_mdle->num_imports = count;
    uint32_t current_name_char = 0;
    uint32_t current_field_char = 0;

    for (uint32_t i = 0; i < count; i++) {
        out_mdle->imports[i].name = &out_mdle->import_name_buf[current_name_char];
        out_mdle->imports[i].name = &out_mdle->import_field_buf[current_field_char];

        //TODO remove magic number
        uint32_t name_len = 0;
        uint32_t field_len = 0;
        WRP_CHECK(wrp_read_string(buf, out_mdle->imports[i].name, 1024, &name_len));
        WRP_CHECK(wrp_read_string(buf, out_mdle->imports[i].name, 1024, &field_len));
        WRP_CHECK(wrp_read_uint8(buf, &out_mdle->imports[i].kind));

        current_name_char += current_name_char + 1;
        current_field_char += field_len + 1;

        if (out_mdle->imports[i].kind == EXTERNAL_FUNC) {
            return WRP_ERR_INVALID_IMPORT;
        } else if (out_mdle->imports[i].kind == EXTERNAL_TABLE) {
            return WRP_ERR_INVALID_IMPORT;
        } else if (out_mdle->imports[i].kind == EXTERNAL_MEMORY) {
            return WRP_ERR_INVALID_IMPORT;
        } else if (out_mdle->imports[i].kind == EXTERNAL_GLOBAL) {
            out_mdle->imports[out_mdle->num_globals].idx = out_mdle->num_globals;
            WRP_CHECK(wrp_read_vari7(buf, &out_mdle->globals[out_mdle->num_globals].type));
            WRP_CHECK(wrp_read_varui1(buf, &out_mdle->globals[out_mdle->num_globals].mutability));
            out_mdle->num_globals++;

            //WASM v1 forbids mutable globals
            if (out_mdle->globals[out_mdle->num_globals].mutability != GLOBAL_IMMUTABLE) {
                return WRP_ERR_INVALID_MUTIBILITY;
            }
        }
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_func_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = out_mdle->num_funcs; i < out_mdle->num_funcs + count; i++) {
        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->funcs[i].type_idx));

        if (out_mdle->funcs[i].type_idx >= out_mdle->num_types) {
            return WRP_ERR_INVALID_TYPE_IDX;
        }
    }

    out_mdle->num_funcs += count;

    return WRP_SUCCESS;
}

static wrp_err_t load_table_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    return WRP_SUCCESS;
}

static wrp_err_t load_memory_section(wrp_vm_t *vm,
    wrp_buf_t *buf,
    wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = out_mdle->num_memories; i < out_mdle->num_memories + count; i++) {
        uint32_t min_pages = 0;
        uint32_t max_pages = MAX_MEMORY_PAGES;
        WRP_CHECK(wrp_read_limits(buf, &min_pages, &max_pages));

        if (min_pages > 0) {
            out_mdle->memories[i].bytes = vm->alloc_fn(min_pages * PAGE_SIZE, 64);

            if (out_mdle->memories[i].bytes == NULL) {
                return WRP_ERR_MEMORY_ALLOCATION_FAILED;
            }
        }

        out_mdle->memories[i].num_pages = min_pages;
        out_mdle->memories[i].max_pages = max_pages;
    }

    out_mdle->num_memories += count;

    return WRP_SUCCESS;
}

static wrp_err_t load_global_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = out_mdle->num_globals; i < out_mdle->num_globals + count; i++) {

        out_mdle->globals[i].value = &out_mdle->global_buf[i];

        out_mdle->globals[i].value = 0;
        WRP_CHECK(wrp_read_vari7(buf, &out_mdle->globals[i].type));
        WRP_CHECK(wrp_read_varui1(buf, &out_mdle->globals[i].mutability));

        if (!wrp_is_valid_value_type(out_mdle->globals[i].type)) {
            return WRP_ERR_INVALID_TYPE;
        }

        //WASM v1 forbids mutable globals
        if (out_mdle->globals[i].mutability != GLOBAL_IMMUTABLE) {
            return WRP_ERR_INVALID_MUTIBILITY;
        }
    }

    out_mdle->num_globals += count;

    return WRP_SUCCESS;
}

static wrp_err_t load_export_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_mdle->num_exports += count;
    uint32_t current_char = 0;

    for (uint32_t i = 0; i < count; i++) {
        out_mdle->exports[i].name = &out_mdle->export_name_buf[current_char];

        //TODO remove magic number
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, out_mdle->exports[i].name, 1024, &name_len));
        WRP_CHECK(wrp_read_varui7(buf, &out_mdle->exports[i].kind));
        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->exports[i].idx));

        current_char += name_len + 1;

        if (out_mdle->exports[i].kind == EXTERNAL_FUNC && out_mdle->exports[i].idx >= out_mdle->num_funcs) {
            return WRP_ERR_INVALID_FUNC_IDX;
        } else if (out_mdle->exports[i].kind == EXTERNAL_TABLE) {
            return WRP_ERR_INVALID_EXPORT;
        } else if (out_mdle->exports[i].kind == EXTERNAL_MEMORY) {
            return WRP_ERR_INVALID_EXPORT;
        } else if (out_mdle->exports[i].kind == EXTERNAL_GLOBAL) {
            return WRP_ERR_INVALID_EXPORT;
        } else {
            return WRP_ERR_INVALID_EXPORT;
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
    uint32_t count = 0;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    if (count != out_mdle->num_funcs) {
        return WRP_ERR_MDLE_CODE_MISMATCH;
    }

    uint32_t current_local = 0;
    size_t code_offset = 0;
    uint32_t func_offset = 0;

    for(uint32_t i = 0; i < out_mdle->num_imports; i++){
        if(out_mdle->imports[i].kind == EXTERNAL_FUNC){
            func_offset++;
        }
    }

    for (uint32_t i = 0; i < out_mdle->num_funcs; i++) {
        size_t code_segment_pos = buf->pos;

        out_mdle->funcs[func_offset + i].local_types = &out_mdle->local_type_buf[current_local];

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
                out_mdle->funcs[func_offset + i].local_types[current_local++] = value_type;
            }

            out_mdle->funcs[func_offset + i].num_locals += num_locals;
        }

        size_t code_sz = body_sz - (buf->pos - code_segment_pos) + 1;

        memcpy(&out_mdle->code_buf[code_offset], &buf->bytes[buf->pos], code_sz);
        out_mdle->funcs[func_offset + i].code = &out_mdle->code_buf[code_offset];
        out_mdle->funcs[func_offset + i].code_sz = code_sz;

        WRP_CHECK(wrp_skip(buf, code_sz));
        code_offset += code_sz;
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_data_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    WRP_CHECK(wrp_read_varui32(buf, &out_mdle->num_data_segments));

    size_t data_offset = 0;
    size_t init_expr_offset = 0;
    for (uint32_t i = 0; i < out_mdle->num_data_segments; i++) {
        out_mdle->data_segments[i].init_expr = &out_mdle->data_init_expr_buf[init_expr_offset];

        uint32_t data_sz = 0;
        uint32_t init_expr_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, &out_mdle->data_segments[i].mem_idx));
        WRP_CHECK(wrp_read_init_expr(buf, out_mdle->data_segments[i].init_expr, &init_expr_sz));
        WRP_CHECK(wrp_read_varui32(buf, &data_sz));

        //MVP only allows one memory
        if (out_mdle->data_segments[i].mem_idx != 0) {
            return WRP_ERR_INVALID_MEM_IDX;
        }

        memcpy(&out_mdle->data_buf[data_offset], &buf->bytes[buf->pos], data_sz);
        out_mdle->data_segments[i].data = &out_mdle->data_buf[data_offset];
        out_mdle->data_segments[i].data_sz = (size_t)data_sz;

        WRP_CHECK(wrp_skip(buf, data_sz));

        init_expr_offset += init_expr_sz;
        data_offset += data_sz;
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_load_mdle(wrp_vm_t *vm,
    wrp_buf_t *buf,
    wrp_wasm_mdle_t *out_mdle)
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
            WRP_CHECK(load_memory_section(vm, buf, out_mdle));
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
            WRP_CHECK(load_data_section(buf, out_mdle));
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
