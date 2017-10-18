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

#include <stdalign.h>
#include <string.h>

#include "warp-buf.h"
#include "warp-error.h"
#include "warp-expr.h"
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
        wrp_type_t *type = &out_mdle->types[i];

        WRP_CHECK(wrp_read_varui7(buf, &type->form));

        if (type->form != TYPE_FUNCTION) {
            return WRP_ERR_MDLE_INVALID_FORM;
        }

        WRP_CHECK(wrp_read_varui32(buf, &type->num_params));

        if (type->num_params > MAX_FUNC_PARAMETERS) {
            return WRP_ERR_MDLE_FUNC_PARAMETER_OVERFLOW;
        }

        type->param_types = &out_mdle->param_type_buf[current_param];

        for (uint32_t j = 0; j < type->num_params; j++) {
            WRP_CHECK(wrp_read_vari7(buf, &out_mdle->param_type_buf[current_param]));

            if (!wrp_is_valid_value_type(out_mdle->param_type_buf[current_param])) {
                return WRP_ERR_INVALID_TYPE;
            }

            current_param++;
        }

        WRP_CHECK(wrp_read_varui32(buf, &type->num_results));

        if (type->num_results > MAX_FUNC_RESULTS) {
            return WRP_ERR_MDLE_FUNC_RETURN_OVERFLOW;
        }

        type->result_types = &out_mdle->result_type_buf[current_result];

        for (uint32_t j = 0; j < type->num_results; j++) {
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
        wrp_import_t *import = &out_mdle->imports[i];
        import->name = &out_mdle->import_name_buf[current_name_char];
        import->field = &out_mdle->import_field_buf[current_field_char];

        //TODO remove magic number
        uint32_t name_len = 0;
        uint32_t field_len = 0;
        WRP_CHECK(wrp_read_string(buf, import->name, 1024, &name_len));
        WRP_CHECK(wrp_read_string(buf, import->name, 1024, &field_len));
        WRP_CHECK(wrp_read_uint8(buf, &import->kind));

        current_name_char += current_name_char + 1;
        current_field_char += field_len + 1;

        if (import->kind == EXTERNAL_FUNC) {
            return WRP_ERR_INVALID_IMPORT;
        } else if (import->kind == EXTERNAL_TABLE) {
            return WRP_ERR_INVALID_IMPORT;
        } else if (import->kind == EXTERNAL_MEMORY) {
            return WRP_ERR_INVALID_IMPORT;
        } else if (import->kind == EXTERNAL_GLOBAL) {
            import->idx = out_mdle->num_globals;
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
        wrp_func_t *func = &out_mdle->funcs[i];

        WRP_CHECK(wrp_read_varui32(buf, &func->type_idx));

        if (func->type_idx >= out_mdle->num_types) {
            return WRP_ERR_INVALID_TYPE_IDX;
        }
    }

    out_mdle->num_funcs += count;

    return WRP_SUCCESS;
}

static wrp_err_t load_table_section(wrp_vm_t *vm,
    wrp_buf_t *buf,
    wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = out_mdle->num_tables; i < out_mdle->num_tables + count; i++) {
        wrp_table_t *table = &out_mdle->tables[i];

        int8_t elem_type = 0;
        WRP_CHECK(wrp_read_vari7(buf, &elem_type));

        if (!wrp_is_valid_elem_type(elem_type)) {
            return WRP_ERR_INVALID_ELEMENT_TYPE;
        }

        uint32_t min_table_elem = 0;
        uint32_t max_table_elem = MAX_TABLE_SIZE;
        WRP_CHECK(wrp_read_limits(buf, &min_table_elem, &max_table_elem));

        if (min_table_elem > max_table_elem || max_table_elem > MAX_TABLE_SIZE) {
            return WRP_ERR_INVALID_TABLE_LIMIT;
        }

        if (min_table_elem > 0) {
            table->elem = vm->alloc_fn(min_table_elem * sizeof(uint32_t), alignof(uint32_t));

            if (table->elem == NULL) {
                return WRP_ERR_MEMORY_ALLOCATION_FAILED;
            }
        }

        table->type = elem_type;
        table->num_elem = min_table_elem;
        table->max_elem = max_table_elem;
    }

    out_mdle->num_tables += count;

    return WRP_SUCCESS;
}

static wrp_err_t load_memory_section(wrp_vm_t *vm,
    wrp_buf_t *buf,
    wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = out_mdle->num_memories; i < out_mdle->num_memories + count; i++) {
        wrp_memory_t *memory = &out_mdle->memories[i];

        uint32_t min_pages = 0;
        uint32_t max_pages = MAX_MEMORY_PAGES;
        WRP_CHECK(wrp_read_limits(buf, &min_pages, &max_pages));

        if (min_pages > max_pages || max_pages > MAX_MEMORY_PAGES) {
            return WRP_ERR_INVALID_MEM_LIMIT;
        }

        if (min_pages > 0) {
            memory->bytes = vm->alloc_fn(min_pages * PAGE_SIZE, 64);

            if (memory->bytes == NULL) {
                return WRP_ERR_MEMORY_ALLOCATION_FAILED;
            }
        }

        memory->num_pages = min_pages;
        memory->max_pages = max_pages;
    }

    out_mdle->num_memories += count;

    return WRP_SUCCESS;
}

static wrp_err_t load_global_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    for (uint32_t i = out_mdle->num_globals; i < out_mdle->num_globals + count; i++) {
        wrp_global_t *global = &out_mdle->globals[i];
        global->value = &out_mdle->global_buf[i];
        *global->value = 0;
        WRP_CHECK(wrp_read_vari7(buf, &global->type));
        WRP_CHECK(wrp_read_varui1(buf, &global->mutability));

        //TODO skip init expr

        if (!wrp_is_valid_value_type(global->type)) {
            return WRP_ERR_INVALID_TYPE;
        }

        //WASM v1 forbids mutable globals
        if (global->mutability != GLOBAL_IMMUTABLE) {
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
        wrp_export_t *export = &out_mdle->exports[i];
        export->name = &out_mdle->export_name_buf[current_char];

        //TODO remove magic number
        uint32_t name_len = 0;
        WRP_CHECK(wrp_read_string(buf, export->name, 1024, &name_len));
        WRP_CHECK(wrp_read_varui7(buf, &export->kind));
        WRP_CHECK(wrp_read_varui32(buf, &export->idx));

        current_char += name_len + 1;

        if (export->kind == EXTERNAL_FUNC && export->idx >= out_mdle->num_funcs) {
            return WRP_ERR_INVALID_FUNC_IDX;
        } else if (export->kind == EXTERNAL_TABLE) {
            return WRP_ERR_INVALID_EXPORT;
        } else if (export->kind == EXTERNAL_MEMORY) {
            return WRP_ERR_INVALID_EXPORT;
        } else if (export->kind == EXTERNAL_GLOBAL) {
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
    uint32_t count;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_mdle->num_elem_segments = count;
    size_t expr_offset = 0;
    uint32_t elem_offset = 0;

    for (uint32_t i = 0; i < count; i++) {
        wrp_elem_segment_t *segment = &out_mdle->elem_segments[i];
        segment->offset_expr.code = &out_mdle->elem_expr_buf[expr_offset];
        segment->elem = &out_mdle->elem_buf[elem_offset];

        WRP_CHECK(wrp_read_varui32(buf, &segment->table_idx));

        //MVP only allows one table
        if (segment->table_idx != 0) {
            return WRP_ERR_INVALID_MEM_IDX;
        }

        if (segment->table_idx >= out_mdle->num_tables) {
            return WRP_ERR_INVALID_TABLE_IDX;
        }

        size_t expr_pos = buf->pos;
        size_t expr_sz = 0;
        WRP_CHECK(wrp_skip_init_expr(buf, &expr_sz));

        memcpy(segment->offset_expr.code, &buf->bytes[expr_pos], expr_sz);
        segment->offset_expr.sz = expr_sz;
        segment->offset_expr.value_type = I32;

        WRP_CHECK(wrp_read_varui32(buf, &segment->num_elem));

        for(uint32_t j = 0; j < segment->num_elem; j++){
            WRP_CHECK(wrp_read_varui32(buf, &segment->elem[j]));
        }

        expr_offset += expr_sz;
        elem_offset += segment->num_elem;
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_code_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count = 0;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    uint32_t num_func_imports = 0;
    for (uint32_t i = 0; i < out_mdle->num_imports; i++) {
        if (out_mdle->imports[i].kind == EXTERNAL_FUNC) {
            num_func_imports++;
        }
    }

    if (count != out_mdle->num_funcs - num_func_imports) {
        return WRP_ERR_MDLE_CODE_MISMATCH;
    }

    uint32_t current_local = 0;
    size_t code_offset = 0;
    for (uint32_t i = 0; i < count; i++) {
        size_t code_segment_pos = buf->pos;

        wrp_func_t *func = &out_mdle->funcs[num_func_imports + i];
        func->local_types = &out_mdle->local_type_buf[current_local];
        func->num_locals = 0;

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
                func->local_types[func->num_locals] = value_type;
                func->num_locals++;
            }
        }

        size_t code_sz = body_sz - (buf->pos - code_segment_pos) + 1;

        memcpy(&out_mdle->code_buf[code_offset], &buf->bytes[buf->pos], code_sz);
        func->code = &out_mdle->code_buf[code_offset];
        func->code_sz = code_sz;

        WRP_CHECK(wrp_skip(buf, code_sz));
        code_offset += code_sz;

        current_local += func->num_locals;
    }

    return WRP_SUCCESS;
}

static wrp_err_t load_data_section(wrp_buf_t *buf, wrp_wasm_mdle_t *out_mdle)
{
    uint32_t count = 0;
    WRP_CHECK(wrp_read_varui32(buf, &count));

    out_mdle->num_data_segments = count;
    size_t expr_offset = 0;
    size_t data_offset = 0;

    for (uint32_t i = 0; i < count; i++) {
        wrp_data_segment_t *segment = &out_mdle->data_segments[i];
        segment->offset_expr.code = &out_mdle->data_expr_buf[expr_offset];
        segment->data = &out_mdle->data_buf[data_offset];

        WRP_CHECK(wrp_read_varui32(buf, &segment->mem_idx));

        //MVP only allows one memory
        if (segment->mem_idx != 0) {
            return WRP_ERR_INVALID_MEM_IDX;
        }

        if (segment->mem_idx >= out_mdle->num_memories) {
            return WRP_ERR_INVALID_MEM_IDX;
        }

        size_t expr_pos = buf->pos;
        size_t expr_sz = 0;
        WRP_CHECK(wrp_skip_init_expr(buf, &expr_sz));

        memcpy(segment->offset_expr.code, &buf->bytes[expr_pos], expr_sz);
        segment->offset_expr.sz = expr_sz;
        segment->offset_expr.value_type = I32;

        uint32_t data_sz = 0;
        WRP_CHECK(wrp_read_varui32(buf, &data_sz));

        memcpy(segment->data, &buf->bytes[buf->pos], data_sz);
        segment->sz = (size_t)data_sz;

        WRP_CHECK(wrp_skip(buf, data_sz));

        expr_offset += expr_sz;
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
            WRP_CHECK(load_import_section(buf, out_mdle));
            break;

        case SECTION_FUNC:
            WRP_CHECK(load_func_section(buf, out_mdle));
            break;

        case SECTION_TABLE:
            WRP_CHECK(load_table_section(vm, buf, out_mdle));
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
            WRP_CHECK(load_element_section(buf, out_mdle));
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
