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

enum {
    WRP_SUCCESS = 0,
    WRP_ERR_UNKNOWN,
    WRP_ERR_INVALID_TYPE,
    WRP_ERR_INVALID_BLOCK_TYPE,
    WRP_ERR_INVALID_TYPE_IDX,
    WRP_ERR_INVALID_FUNC_IDX,
    WRP_ERR_INVALID_GLOBAL_IDX,
    WRP_ERR_INVALID_LOCAL_IDX,
    WRP_ERR_INVALID_STK_OPERATION,
    WRP_ERR_INVALID_LIMIT,
    WRP_ERR_INVALID_BUFFER_ACCESS,
    WRP_ERR_INVALID_LEB,
    WRP_ERR_INVALID_OPCODE,
    WRP_ERR_INVALID_MUTIBILITY,
    WRP_ERR_INVALID_EXPORT,
    WRP_ERR_INVALID_RESERVED,
    WRP_ERR_UNKNOWN_BLOCK,
    WRP_ERR_UNKNOWN_FUNC,
    WRP_ERR_UNKNOWN_IF,
    WRP_ERR_OP_STK_UNDERFLOW,
    WRP_ERR_OP_STK_OVERFLOW,
    WRP_ERR_OP_MISMATCH,
    WRP_ERR_FRAME_OP_UNDERFLOW,
    WRP_ERR_BLOCK_STK_UNDERFLOW,
    WRP_ERR_BLOCK_STK_OVERFLOW,
    WRP_ERR_FRAME_BLOCK_UNDERFLOW,
    WRP_ERR_CALL_STK_UNDERFLOW,
    WRP_ERR_CALL_STK_OVERFLOW,
    WRP_ERR_CALL_MISSING_ARGS,
    WRP_ERR_CALL_INVALID_ARGS,
    WRP_ERR_CALL_MISSING_RESULTS,
    WRP_ERR_CALL_INVALID_RESULTS,
    WRP_ERR_MDLE_MISSING_PREAMBLE,
    WRP_ERR_MDLE_INVALID_MAGIC_NUMBER,
    WRP_ERR_MDLE_UNSUPPORTED_VERSION,
    WRP_ERR_MDLE_INVALID_SECTION_ID,
    WRP_ERR_MDLE_SECTION_ORDER,
    WRP_ERR_MDLE_TYPE_OVERFLOW,
    WRP_ERR_MDLE_INVALID_FORM,
    WRP_ERR_MDLE_FUNC_PARAMETER_OVERFLOW,
    WRP_ERR_MDLE_FUNC_RETURN_OVERFLOW,
    WRP_ERR_MDLE_FUNC_OVERFLOW,
    WRP_ERR_MDLE_TABLE_OVERFLOW,
    WRP_ERR_MDLE_INVALID_TABLE_ELEMENT,
    WRP_ERR_MDLE_MEMORY_OVERFLOW,
    WRP_ERR_MDLE_GLOBAL_OVERFLOW,
    WRP_ERR_MDLE_EXPORT_OVERFLOW,
    WRP_ERR_MDLE_CODE_MISMATCH,
    WRP_ERR_MDLE_LOCALS_OVERFLOW,
    WRP_ERR_MDLE_INVALID_END_OPCODE,
    WRP_ERR_MDLE_BLOCK_OVERFLOW,
    WRP_ERR_MDLE_IF_ELSE_MISMATCH,
    WRP_ERR_MDLE_BLOCK_UNDERFLOW,
    WPR_ERR_MDLE_BLOCK_END_MISMATCH,
    WRP_ERR_MDLE_INVALID_BYTES,
    WRP_ERR_MDLE_BRANCH_TABLE_OVERFLOW,
    WRP_ERR_UNREACHABLE_CODE_EXECUTED,
    WRP_ERR_INSTRUCTION_OVERFLOW,
    WRP_ERR_I32_DIVIDE_BY_ZERO,
    WRP_ERR_I32_OVERFLOW
};
