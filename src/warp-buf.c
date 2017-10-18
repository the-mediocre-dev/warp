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

#include <stdbool.h>

#include "warp-buf.h"
#include "warp-error.h"
#include "warp-macros.h"

static wrp_err_t read_LEB(wrp_buf_t *buf,
    uint32_t max_bits,
    bool sign_extend,
    uint64_t *out_value)
{
    size_t max_bytes = ((max_bits + 7 - 1) / 7);
    uint32_t num_bytes = 0;
    uint64_t result = 0;
    uint32_t shift = 0;
    uint64_t byte = 0;

    while (true) {
        if (wrp_end_of_buf(buf)) {
            return WRP_ERR_INVALID_BUFFER_ACCESS;
        }

        byte = buf->bytes[buf->pos++];

        result |= ((byte & 0x7f) << shift);
        shift += 7;

        if ((byte & 0x80) == 0) {
            break;
        }

        num_bytes += 1;

        if (num_bytes > max_bytes) {
            return WRP_ERR_INVALID_LEB;
        }
    }

    if (sign_extend && (shift < max_bits) && (byte & 0x40)) {
        result |= -(1 << shift);
    }

    *out_value = result;
    return WRP_SUCCESS;
}

wrp_err_t wrp_seek(wrp_buf_t *buf, size_t new_pos)
{
    if (new_pos > buf->sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    buf->pos = new_pos;
    return WRP_SUCCESS;
}

bool wrp_end_of_buf(wrp_buf_t *buf)
{
    //(pos == buf_sz) indicates end of stream.
    return (buf->pos == buf->sz);
}

wrp_err_t wrp_skip(wrp_buf_t *buf, size_t num_bytes)
{
    if (buf->pos + num_bytes > buf->sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    buf->pos += num_bytes;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_uint8(wrp_buf_t *buf, uint8_t *out_value)
{
    if (buf->pos + sizeof(uint8_t)  > buf->sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    *out_value = buf->bytes[buf->pos++];
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_uint32(wrp_buf_t *buf, uint32_t *out_value)
{
    if (buf->pos + sizeof(uint32_t)  > buf->sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    ((uint8_t *)(out_value))[0] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[1] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[2] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[3] = buf->bytes[buf->pos++];
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_varui1(wrp_buf_t *buf, uint8_t *out_value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, 1, false, &leb));

    *out_value = (uint8_t)leb;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_varui7(wrp_buf_t *buf, uint8_t *out_value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, 7, false, &leb));

    *out_value = (uint8_t)leb;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_varui32(wrp_buf_t *buf, uint32_t *out_value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, 32, false, &leb));

    *out_value = (uint32_t)leb;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_vari7(wrp_buf_t *buf, int8_t *out_value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, 7, true, &leb));

    *out_value = (int8_t)leb;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_vari32(wrp_buf_t *buf, int32_t *out_value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, 32, true, &leb));

    *out_value = (int32_t)leb;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_vari64(wrp_buf_t *buf, int64_t *out_value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, 64, true, &leb));

    *out_value = (int64_t)leb;
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_f32(wrp_buf_t *buf, float *out_value)
{
    if (buf->pos + sizeof(float)  > buf->sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    ((uint8_t *)(out_value))[0] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[1] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[2] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[3] = buf->bytes[buf->pos++];
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_f64(wrp_buf_t *buf, double *out_value)
{
    if (buf->pos + sizeof(double)  > buf->sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    ((uint8_t *)(out_value))[0] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[1] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[2] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[3] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[4] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[5] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[6] = buf->bytes[buf->pos++];
    ((uint8_t *)(out_value))[7] = buf->bytes[buf->pos++];
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_string(wrp_buf_t *buf,
    char *out_str,
    size_t str_sz,
    uint32_t *out_str_len)
{
    WRP_CHECK(wrp_read_varui32(buf, out_str_len));

    if (*out_str_len >= str_sz - 1) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    for (uint32_t i = 0; i < *out_str_len; i++) {
        out_str[i] = buf->bytes[buf->pos++];
    }

    out_str[*out_str_len] = '\0';
    return WRP_SUCCESS;
}

wrp_err_t wrp_read_limits(wrp_buf_t *buf, uint32_t *out_min, uint32_t *out_max)
{
    uint32_t flags = 0;
    WRP_CHECK(wrp_read_varui32(buf, &flags));
    WRP_CHECK(wrp_read_varui32(buf, out_min));

    if (flags & 0x01) {
        WRP_CHECK(wrp_read_varui32(buf, out_max));
    }

    return WRP_SUCCESS;
}
