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

// mandated by standard
#define WASM_MAGIC_NUMBER       0x6d736100
#define WASM_VERSION            0x01
#define PAGE_SIZE               (1u << 16)  // 64kb page size
#define MAX_TABLES              1u
#define MAX_MEMORIES            1u
#define MAX_FUNC_RESULTS        1u

// implementation limitations
#define MAX_MODULE_NAME         64u
#define MAX_TYPES               32u
#define MAX_FUNCS               128u
#define MAX_GLOBALS             32u
#define MAX_ELEMENT_SEGMENTS    32u
#define MAX_DATA_SEGMENTS       32u
#define MAX_IMPORTS             128u
#define MAX_EXPORTS             128u
#define MAX_FUNC_PARAMETERS     128u
#define MAX_FUNC_LOCALS         128u
#define MAX_CODE_SIZE           4096u
#define MAX_TABLE_SIZE          4096u
#define MAX_MEMORY_SIZE         1u
#define MAX_GLOBAL_NAME_SIZE    128u
#define MAX_LOCALS              1024u
#define MAX_BLOCK_DEPTH         512
#define MAX_BRANCH_TABLE_SIZE   32u

//vm config
#define WRP_OPERAND_STK_SZ      4096
#define WRP_BLOCK_STK_SZ        4096
#define WRP_CALL_STK_SZ         4096
#define WRP_ERROR_BUF_SZ        1024u
