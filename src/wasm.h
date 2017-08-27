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

#pragma once

#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

// mandated by standard
#define WASM_MAGIC_NUMBER 0x6d736100
#define WASM_VERSION 0x01
#define PAGE_SIZE (1u << 16) // 64kb page size
#define MAX_TABLES 1
#define MAX_MEMORIES 1

// implementation limitations
#define MAX_MODULE_NAME 64u
#define MAX_TYPES 32u
#define MAX_FUNCS 128u
#define MAX_GLOBALS 32u
#define MAX_ELEMENT_SEGMENTS 32u
#define MAX_DATA_SEGMENTS 32u
#define MAX_IMPORTS 32u
#define MAX_EXPORTS 32u
#define MAX_FUNCTION_PARAMETERS 32u
#define MAX_FUNCTION_RESULTS 32u
#define MAX_FUNCTION_LOCALS 32u
#define MAX_FUNCTION_BODY_SIZE 32u
#define MAX_TABLE_SIZE 4096u
#define MAX_MEMORY_SIZE 1
#define MAX_GLOBAL_NAME_SIZE 128u
#define MAX_CODE_SEGMENTS 128u
#define MAX_LOCALS 1024u

// section encodings
#define SECTION_CUSTOM 0x00
#define SECTION_TYPE 0x01
#define SECTION_IMPORT 0x02
#define SECTION_FUNC 0x03
#define SECTION_TABLE 0x04
#define SECTION_MEMORY 0x05
#define SECTION_GLOBAL 0x06
#define SECTION_EXPORT 0x07
#define SECTION_START 0x08
#define SECTION_ELEMENT 0x09
#define SECTION_CODE 0x0A
#define SECTION_DATA 0x0B

// type signatures
#define TYPE_FUNCTION 0x60

// type encodings
#define I32 0x7f      // -0x01
#define I64 0x7e      // -0x02
#define F32 0x7d      // -0x03
#define F64 0x7c      // -0x04
#define ANY_FUNC 0x70 // -0x10
#define FUNC 0x60     // -0x20
#define VOID 0x40     // -0x40

// export types
#define FUNC_EXPORT 0x00
#define TABLE_EXPORT 0x01
#define MEMORY_EXPORT 0x03
#define GLOBAL_EXPORT 0x04

// module validation result codes
#define VALID_MODULE 0x00
#define INVALID_MODULE 0x01
#define INVALID_MAGIC_NUMBER 0x02
#define INVALID_VERSION 0x03

// mutability
#define GLOBAL_IMMUTABLE 0x00
#define GLOBAL_MUTABLE 0x01

#define OP_END 0x0B

struct wasm_meta {
    size_t types[MAX_TYPES];
    uint32_t num_types;
    uint32_t num_type_params;
    uint32_t num_type_returns;
    size_t funcs[MAX_FUNCS];
    uint32_t num_funcs;
    size_t tables[MAX_TABLES];
    uint32_t num_tables;
    size_t memories[MAX_MEMORIES];
    uint32_t num_memories;
    size_t globals[MAX_GLOBALS];
    uint32_t num_globals;
    size_t exports[MAX_EXPORTS];
    uint32_t num_exports;
    uint32_t export_name_len;
    bool start_func_present;
    size_t start_func;
    size_t elements[MAX_ELEMENT_SEGMENTS];
    uint32_t num_elements;
    size_t code[MAX_CODE_SEGMENTS];
    uint32_t num_code_segments;
    uint32_t num_code_locals;
    size_t code_body_sz;
    uint32_t validation_result;
};

struct wasm_module {
    //force struct alignment at member level, as struct level
    //usage of alignas only works in C++, for some magical reason
    alignas(64) uint32_t *forms;
    uint8_t *param_types;
    uint32_t *param_type_offsets;
    uint32_t *param_counts;
    uint8_t *result_types;
    uint32_t *result_type_offsets;
    uint32_t *result_counts;
    uint32_t num_types;
    uint32_t *func_type_idxs;
    uint32_t num_funcs;
    uint64_t *global_values;
    uint8_t *global_types;
    uint32_t num_globals;
    uint32_t start_func_idx;
    bool start_func_present;
    char *export_names;
    uint32_t *export_name_offsets;
    uint32_t *export_func_idxs;
    uint32_t num_exports;
    uint8_t *local_types;
    uint32_t *local_type_offsets;
    uint32_t *local_counts;
    uint8_t *code;
    size_t *start_instructions;
    uint32_t num_code_segments;
};
