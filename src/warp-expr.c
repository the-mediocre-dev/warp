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

#include "warp-expr.h"
#include "warp-buf.h"
#include "warp-error.h"
#include "warp-macros.h"
#include "warp-wasm.h"

bool wrp_is_valid_init_expr_opcode(uint8_t opcode)
{
    bool valid = false;

    if (opcode == OP_I32_CONST) {
        valid = true;
    }
    if (opcode == OP_I64_CONST) {
        valid = true;
    }
    if (opcode == OP_F32_CONST) {
        valid = true;
    }
    if (opcode == OP_F64_CONST) {
        valid = true;
    }
    if (opcode == OP_GET_GLOBAL) {
        valid = true;
    }
    if (opcode == OP_END) {
        valid = true;
    }

    return valid;
}

wrp_err_t wrp_skip_expr(wrp_buf_t *buf, uint8_t *out_opcode, size_t *out_expr_sz)
{
    size_t start_pos = buf->pos;
    uint8_t opcode = 0;
    WRP_CHECK(wrp_read_uint8(buf, &opcode));

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

    *out_opcode = opcode;
    *out_expr_sz = buf->pos - start_pos;
    return WRP_SUCCESS;
}

wrp_err_t wrp_skip_init_expr(wrp_buf_t *buf, size_t *out_init_expr_sz)
{
    size_t start_pos = buf->pos;
    uint8_t opcode = 0;
    size_t expr_sz = 0;

    while (opcode != OP_END) {
        WRP_CHECK(wrp_skip_expr(buf, &opcode, &expr_sz));
    }

    *out_init_expr_sz = buf->pos - start_pos;
    return WRP_SUCCESS;
}
