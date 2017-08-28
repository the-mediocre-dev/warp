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

#include "wasm-validation.h"
#include "buf-io.h"
#include "wasm.h"

static bool validate_preamble(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (buf_sz < 8) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    uint32_t magic_number = 0;
    if (!wrp_read_uint32(buf, buf_sz, pos, &magic_number)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (magic_number != WASM_MAGIC_NUMBER) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    uint32_t wasm_version = 0;
    if (!wrp_read_uint32(buf, buf_sz, pos, &wasm_version)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (wasm_version != WASM_VERSION) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    return true;
}

static bool validate_type_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_types)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_types > MAX_TYPES) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    for (uint32_t i = 0; i < meta->num_types; i++) {
        meta->types[i] = *pos;

        uint8_t form = 0;
        if (!wrp_read_varui7(buf, buf_sz, pos, &form)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (form != TYPE_FUNCTION) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint32_t num_params = 0;
        if (!wrp_read_varui32(buf, buf_sz, pos, &num_params)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (num_params > MAX_FUNCTION_PARAMETERS) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        meta->num_type_params += num_params;

        for (uint32_t j = 0; j < num_params; j++) {
            uint8_t value_type = 0;
            if (!wrp_read_varui7(buf, buf_sz, pos, &value_type)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }

            if (!wrp_is_valid_value_type(value_type)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }
        }

        uint32_t num_results = 0;
        if (!wrp_read_varui32(buf, buf_sz, pos, &num_results)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (num_results > MAX_FUNCTION_RESULTS) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        meta->num_type_returns += num_results;

        for (uint8_t j = 0; j < num_results; j++) {
            uint8_t value_type = 0;
            if (!wrp_read_varui7(buf, buf_sz, pos, &value_type)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }

            if (!wrp_is_valid_value_type(value_type)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }
        }
    }

    return true;
}

static bool validate_import_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    return true;
}

static bool validate_func_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_funcs)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_funcs > MAX_FUNCS) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    for (uint32_t i = 0; i < meta->num_funcs; i++) {
        meta->funcs[i] = *pos;

        uint32_t type_idx = 0;
        if (!wrp_read_varui32(buf, buf_sz, pos, &type_idx)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (type_idx >= meta->num_types) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }
    }

    return true;
}

static bool validate_table_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_tables)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_tables > MAX_TABLES) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    for (uint32_t i = 0; i < meta->num_tables; i++) {
        meta->tables[i] = *pos;

        uint8_t elem_type = 0;
        if (!wrp_read_varui7(buf, buf_sz, pos, &elem_type)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (elem_type != ANY_FUNC) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint32_t min_table_sz = 0;
        uint32_t max_table_sz = 0;
        if (!wrp_read_limits(buf, buf_sz, pos, &min_table_sz, &max_table_sz, MAX_TABLE_SIZE)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }
    }

    return true;
}

static bool validate_memory_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_memories)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_memories > MAX_MEMORIES) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    for (uint32_t i = 0; i < meta->num_memories; i++) {
        meta->memories[i] = *pos;

        uint32_t min_pages = 0;
        uint32_t max_pages = 0;
        if (!wrp_read_limits(buf, buf_sz, pos, &min_pages, &max_pages, MAX_MEMORY_SIZE)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }
    }

    return true;
}

static bool validate_global_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_globals)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_globals > MAX_GLOBALS) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    for (uint32_t i = 0; i < meta->num_globals; i++) {
        meta->globals[i] = *pos;

        uint8_t value_type = 0;
        if (!wrp_read_varui7(buf, buf_sz, pos, &value_type)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (!wrp_is_valid_value_type(value_type)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint8_t mutability = 0;
        if (!wrp_read_varui1(buf, buf_sz, pos, &mutability)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        //WASM v1 forbids mutable globals
        if (mutability != GLOBAL_IMMUTABLE) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }
    }

    return true;
}

static bool validate_export_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_exports)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_exports > MAX_EXPORTS) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    char name[MAX_GLOBAL_NAME_SIZE + 1] = {};

    for (uint32_t i = 0; i < meta->num_exports; i++) {
        meta->exports[i] = *pos;

        uint32_t name_len = 0;
        if (!wrp_read_string(buf, buf_sz, pos, name, MAX_GLOBAL_NAME_SIZE, &name_len)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        meta->export_name_len += name_len + 1;

        uint8_t kind = 0;
        if (!wrp_read_uint8(buf, buf_sz, pos, &kind)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        //WASM v1 forbids any but func exports
        if (kind != FUNC_EXPORT) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint32_t func_idx = 0;
        if (!wrp_read_varui32(buf, buf_sz, pos, &func_idx)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (func_idx >= meta->num_funcs) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }
    }

    return true;
}

static bool validate_start_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    meta->start_func_present = true;
    meta->start_func = *pos;

    uint32_t start_func_idx;
    if (!wrp_read_varui32(buf, buf_sz, pos, &start_func_idx)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    return true;
}

static bool validate_element_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    return true;
}

static bool validate_code_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    if (!wrp_read_varui32(buf, buf_sz, pos, &meta->num_code_segments)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    if (meta->num_code_segments > MAX_CODE_SEGMENTS) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    for (uint32_t i = 0; i < meta->num_code_segments; i++) {
        meta->code[i] = *pos;
        size_t start_pos = *pos;

        uint32_t body_sz;
        if (!wrp_read_varui32(buf, buf_sz, pos, &body_sz)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint32_t num_local_entry = 0;
        if (!wrp_read_varui32(buf, buf_sz, pos, &num_local_entry)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint32_t total_locals = 0;
        for (uint32_t j = 0; j < num_local_entry; j++) {
            uint32_t num_locals = 0;
            if (!wrp_read_varui32(buf, buf_sz, pos, &num_locals)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }

            uint8_t value_type = 0;
            if (!wrp_read_varui7(buf, buf_sz, pos, &value_type)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }
            if (!wrp_is_valid_value_type(value_type)) {
                meta->validation_result = INVALID_MODULE;
                return false;
            }

            total_locals += num_locals;
        }

        if (total_locals > MAX_LOCALS) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        meta->num_code_locals += total_locals;

        size_t code_sz = body_sz - (*pos - meta->code[i]);
        meta->code_body_sz += code_sz;

        if (!wrp_skip(buf, buf_sz, pos, code_sz)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint8_t end_opcode = 0;
        if (!wrp_read_uint8(buf, buf_sz, pos, &end_opcode)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (end_opcode != OP_END) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }
    }

    return true;
}

static bool validate_data_section(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    struct wrp_wasm_meta *meta)
{
    return true;
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

bool wrp_validate_module(uint8_t *buf, size_t buf_sz, struct wrp_wasm_meta *meta)
{
    meta->validation_result = INVALID_MODULE;
    size_t pos = 0;

    if (!validate_preamble(buf, buf_sz, &pos, meta)) {
        return false;
    }

    uint32_t prev_section_id = 0;

    while (pos < buf_sz) {
        uint8_t section_id = 0;

        if (!wrp_read_varui7(buf, buf_sz, &pos, &section_id)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        uint32_t section_sz = 0;

        if (!wrp_read_varui32(buf, buf_sz, &pos, &section_sz)) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (section_id > SECTION_DATA) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        if (section_id > 0 && section_id <= prev_section_id) {
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        switch (section_id) {
        case SECTION_CUSTOM:
            if (!wrp_skip(buf, buf_sz, &pos, section_sz)) {
                return false;
            }
            break;

        case SECTION_TYPE:
            if (!validate_type_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_IMPORT:

            //TODO
            // if (!validate_data_section(buf, buf_sz, &pos, meta)) {
            //     return false;
            // }

            if (!wrp_skip(buf, buf_sz, &pos, section_sz)) {
                return false;
            }
            break;

        case SECTION_FUNC:
            if (!validate_func_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_TABLE:
            if (!validate_table_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_MEMORY:
            if (!validate_memory_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_GLOBAL:
            if (!validate_global_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_EXPORT:
            if (!validate_export_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_START:
            if (!validate_start_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_ELEMENT:

            //TODO
            // if (!validate_element_section(buf, buf_sz, &pos, meta)) {
            //     return false;
            // }

            if (!wrp_skip(buf, buf_sz, &pos, section_sz)) {
                return false;
            }
            break;

        case SECTION_CODE:
            if (!validate_code_section(buf, buf_sz, &pos, meta)) {
                return false;
            }
            break;

        case SECTION_DATA:

            //TODO
            // if (!validate_data_section(buf, buf_sz, &pos, meta)) {
            //     return false;
            // }

            if (!wrp_skip(buf, buf_sz, &pos, section_sz)) {
                return false;
            }
            break;

        default:
            meta->validation_result = INVALID_MODULE;
            return false;
        }

        prev_section_id = section_id;
    }

    if (!wrp_end_of_buf(buf, buf_sz, pos)) {
        meta->validation_result = INVALID_MODULE;
        return false;
    }

    return true;
}
