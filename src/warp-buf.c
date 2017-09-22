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

static uint32_t read_LEB(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    uint64_t *value,
    uint32_t max_bits,
    bool sign_extend)
{
    size_t max_bytes = ((max_bits + 7 - 1) / 7);
    uint32_t num_bytes = 0;
    uint64_t result = 0;
    uint32_t shift = 0;
    uint64_t byte = 0;

    while (true) {
        byte = buf[(*pos)++];

        result |= ((byte & 0x7f) << shift);
        shift += 7;

        if ((byte & 0x80) == 0) {
            break;
        }

        num_bytes += 1;

        if (*pos + num_bytes > buf_sz){
            return WRP_ERR_INVALID_BUFFER_ACCESS;
        }

        if(num_bytes > max_bytes) {
            return WRP_ERR_INVALID_LEB;
        }
    }

    if (sign_extend && (shift < max_bits) && (byte & 0x40)) {
        result |= -(1 << shift);
    }

    *value = result;
    return WRP_SUCCESS;
}

uint32_t wrp_seek(uint8_t *buf, size_t buf_sz, size_t *pos, size_t new_pos)
{
    if (new_pos > buf_sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    *pos = new_pos;
    return WRP_SUCCESS;
}

uint32_t wrp_end_of_buf(uint8_t *buf, size_t buf_sz, size_t pos)
{
    //(pos == buf_sz) indicates end of stream.
    return (pos == buf_sz);
}

uint32_t wrp_skip(uint8_t *buf, size_t buf_sz, size_t *pos, size_t num_bytes)
{
    if (*pos + num_bytes > buf_sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    *pos += num_bytes;
    return WRP_SUCCESS;
}

uint32_t wrp_read_uint8(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value)
{
    if (*pos + sizeof(uint8_t) > buf_sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    *value = buf[(*pos)++];
    return WRP_SUCCESS;
}

uint32_t wrp_read_uint32(uint8_t *buf, size_t buf_sz, size_t *pos, uint32_t *value)
{
    if (*pos + sizeof(uint32_t) > buf_sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    ((uint8_t *)(value))[0] = buf[(*pos)++];
    ((uint8_t *)(value))[1] = buf[(*pos)++];
    ((uint8_t *)(value))[2] = buf[(*pos)++];
    ((uint8_t *)(value))[3] = buf[(*pos)++];
    return WRP_SUCCESS;
}

uint32_t wrp_read_varui1(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, buf_sz, pos, &leb, 1, false));

    *value = (uint8_t)leb;
    return WRP_SUCCESS;
}

uint32_t wrp_read_varui7(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, buf_sz, pos, &leb, 7, false));

    *value = (uint8_t)leb;
    return WRP_SUCCESS;
}

uint32_t wrp_read_varui32(uint8_t *buf, size_t buf_sz, size_t *pos, uint32_t *value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, buf_sz, pos, &leb, 32, false));

    *value = (uint32_t)leb;
    return WRP_SUCCESS;
}

uint32_t wrp_read_vari7(uint8_t *buf, size_t buf_sz, size_t *pos, int8_t *value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, buf_sz, pos, &leb, 7, true));

    *value = (int8_t)leb;
    return WRP_SUCCESS;
}

uint32_t wrp_read_vari32(uint8_t *buf, size_t buf_sz, size_t *pos, int32_t *value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, buf_sz, pos, &leb, 32, true));

    *value = (int32_t)leb;
    return WRP_SUCCESS;
}

uint32_t wrp_read_vari64(uint8_t *buf, size_t buf_sz, size_t *pos, int64_t *value)
{
    uint64_t leb = 0;
    WRP_CHECK(read_LEB(buf, buf_sz, pos, &leb, 64, true));

    *value = (int64_t)leb;
    return WRP_SUCCESS;
}

uint32_t wrp_read_f32(uint8_t *buf, size_t buf_sz, size_t *pos, float *value)
{
    if (*pos + sizeof(float) > buf_sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    ((uint8_t *)(value))[0] = buf[(*pos)++];
    ((uint8_t *)(value))[1] = buf[(*pos)++];
    ((uint8_t *)(value))[2] = buf[(*pos)++];
    ((uint8_t *)(value))[3] = buf[(*pos)++];
    return WRP_SUCCESS;
}

uint32_t wrp_read_f64(uint8_t *buf, size_t buf_sz, size_t *pos, double *value)
{
    if (*pos + sizeof(double) > buf_sz) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    ((uint8_t *)(value))[0] = buf[(*pos)++];
    ((uint8_t *)(value))[1] = buf[(*pos)++];
    ((uint8_t *)(value))[2] = buf[(*pos)++];
    ((uint8_t *)(value))[3] = buf[(*pos)++];
    ((uint8_t *)(value))[4] = buf[(*pos)++];
    ((uint8_t *)(value))[5] = buf[(*pos)++];
    ((uint8_t *)(value))[6] = buf[(*pos)++];
    ((uint8_t *)(value))[7] = buf[(*pos)++];
    return WRP_SUCCESS;
}

uint32_t wrp_read_string(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    char *str,
    size_t str_sz,
    uint32_t *str_len)
{
    uint32_t len = 0;
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &len));

    if (len >= str_sz - 1) {
        return WRP_ERR_INVALID_BUFFER_ACCESS;
    }

    for (uint32_t i = 0; i < len; i++) {
        str[i] = buf[(*pos)++];
    }

    str[len] = '\0';
    *str_len = len;
    return WRP_SUCCESS;
}

uint32_t wrp_read_limits(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    uint32_t *min,
    uint32_t *max,
    uint32_t default_max)
{
    uint32_t flags = 0;
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, &flags));
    WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, min));

    if (flags & 0x01) {
        WRP_CHECK(wrp_read_varui32(buf, buf_sz, pos, max));
    } else {
        *max = default_max;
    }

    if (*max < *min) {
        return WRP_ERR_INVALID_LIMIT;
    }

    return WRP_SUCCESS;
}
