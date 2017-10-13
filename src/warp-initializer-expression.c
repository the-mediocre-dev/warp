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

#include "warp-initializer-expression.h"
#include "warp-buf.h"
#include "warp-error.h"
#include "warp-macros.h"
#include "warp-wasm.h"

wrp_err_t wrp_init_expr_i32(wrp_buf_t *buf, int32_t *out_value)
{
    uint8_t opcode = 0;
    WRP_CHECK(wrp_read_uint8(buf, &opcode));

    if (opcode != OP_I32_CONST) {
        return WRP_ERR_INVALID_INITIALZER_EXPRESSION;
    }

    WRP_CHECK(wrp_read_vari32(buf, out_value));

    if (wrp_end_of_buf(buf)) {
        return WRP_ERR_UNKNOWN;
    }

    if (buf->bytes[buf->pos++] != OP_END) {
        return WRP_ERR_UNKNOWN;
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_init_expr_i64(wrp_buf_t *buf, int64_t *out_value)
{
    uint8_t opcode = 0;
    WRP_CHECK(wrp_read_uint8(buf, &opcode));

    if (opcode != OP_I64_CONST) {
        return WRP_ERR_INVALID_INITIALZER_EXPRESSION;
    }

    WRP_CHECK(wrp_read_vari64(buf, out_value));

    if (wrp_end_of_buf(buf)) {
        return WRP_ERR_UNKNOWN;
    }

    if (buf->bytes[buf->pos++] != OP_END) {
        return WRP_ERR_UNKNOWN;
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_init_expr_f32(wrp_buf_t *buf, float *out_value)
{
    uint8_t opcode = 0;
    WRP_CHECK(wrp_read_uint8(buf, &opcode));

    if (opcode != OP_F32_CONST) {
        return WRP_ERR_INVALID_INITIALZER_EXPRESSION;
    }

    WRP_CHECK(wrp_read_f32(buf, out_value));

    if (wrp_end_of_buf(buf)) {
        return WRP_ERR_UNKNOWN;
    }

    if (buf->bytes[buf->pos++] != OP_END) {
        return WRP_ERR_UNKNOWN;
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_init_expr_f64(wrp_buf_t *buf, double *out_value)
{
    uint8_t opcode = 0;
    WRP_CHECK(wrp_read_uint8(buf, &opcode));

    if (opcode != OP_F64_CONST) {
        return WRP_ERR_INVALID_INITIALZER_EXPRESSION;
    }

    WRP_CHECK(wrp_read_f64(buf, out_value));

    if (wrp_end_of_buf(buf)) {
        return WRP_ERR_UNKNOWN;
    }

    if (buf->bytes[buf->pos++] != OP_END) {
        return WRP_ERR_UNKNOWN;
    }

    return WRP_SUCCESS;
}

wrp_err_t wrp_init_expr_get_global(wrp_buf_t *buf, int32_t *out_value)
{
    return WRP_ERR_UNKNOWN;
}

wrp_err_t wrp_read_init_expr(wrp_buf_t *buf, uint8_t *out_expr, uint32_t *out_size)
{
    size_t start_pos = buf->pos;
    uint8_t opcode = 0;
    WRP_CHECK(wrp_read_uint8(buf, &opcode));

    if (opcode == OP_I32_CONST) {
        int32_t value = 0;
        WRP_CHECK(wrp_read_vari32(buf, &value));
    } else if(opcode == OP_I64_CONST){
        int64_t value = 0;
        WRP_CHECK(wrp_read_vari64(buf, &value));
    }else if(opcode == OP_F32_CONST){
        float value = 0;
        WRP_CHECK(wrp_read_f32(buf, &value));
    }else if(opcode == OP_F64_CONST){
        double value = 0;
        WRP_CHECK(wrp_read_f64(buf, &value));
    }else if(opcode == OP_GET_GLOBAL){
        int32_t value = 0;
        WRP_CHECK(wrp_read_vari32(buf, &value));
    } else{
        return WRP_ERR_INVALID_INITIALZER_EXPRESSION;
    }

    WRP_CHECK(wrp_read_uint8(buf, &opcode));

    if(opcode != OP_END){
        return WRP_ERR_INVALID_INITIALZER_EXPRESSION;
    }

    out_expr = &buf->bytes[start_pos];
    *out_size = buf->pos - start_pos;
    return WRP_SUCCESS;
}
