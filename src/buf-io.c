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

#include "buf-io.h"

static bool read_LEB(uint8_t *buf,
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

        if (num_bytes > max_bytes) {
            return false;
        }
    }

    if (sign_extend && (shift < max_bytes) && (byte & 0x40)) {
        result |= -(1 << shift);
    }

    *value = result;
    return true;
}

bool wrp_seek(uint8_t *buf, size_t buf_sz, size_t *pos, size_t new_pos)
{
    //(pos == buf_sz) indicates end of stream.
    if (new_pos > buf_sz) {
        return false;
    }

    *pos = new_pos;
    return true;
}

bool wrp_end_of_buf(uint8_t *buf, size_t buf_sz, size_t pos)
{
    return (pos == buf_sz);
}

bool wrp_skip(uint8_t *buf, size_t buf_sz, size_t *pos, size_t num_bytes)
{
    //(pos == buf_sz) indicates end of stream.
    if (*pos + num_bytes > buf_sz) {
        return false;
    }

    *pos += num_bytes;
    return true;
}

bool wrp_read_uint8(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value)
{
    if (*pos + sizeof(uint8_t) >= buf_sz) {
        return false;
    }

    *value = buf[(*pos)++];
    return true;
}

bool wrp_read_uint32(uint8_t *buf, size_t buf_sz, size_t *pos, uint32_t *value)
{
    if (*pos + sizeof(uint32_t) >= buf_sz) {
        return false;
    }

    ((uint8_t *)(value))[0] = buf[(*pos)++];
    ((uint8_t *)(value))[1] = buf[(*pos)++];
    ((uint8_t *)(value))[2] = buf[(*pos)++];
    ((uint8_t *)(value))[3] = buf[(*pos)++];
    return true;
}

bool wrp_read_varui1(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value)
{
    if (*pos + sizeof(uint8_t) >= buf_sz) {
        return false;
    }

    uint64_t leb = 0;

    if (!read_LEB(buf, pos, &leb, 1, false)) {
        return false;
    }

    *value = (uint8_t)leb;
    return true;
}

bool wrp_read_varui7(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value)
{
    if (*pos + sizeof(uint8_t) >= buf_sz) {
        return false;
    }

    uint64_t leb = 0;

    if (!read_LEB(buf, pos, &leb, 7, false)) {
        return false;
    }

    *value = (uint8_t)leb;
    return true;
}

bool wrp_read_varui32(uint8_t *buf, size_t buf_sz, size_t *pos, uint32_t *value)
{
    if (*pos + sizeof(uint32_t) >= buf_sz) {
        return false;
    }

    uint64_t leb = 0;

    if (!read_LEB(buf, pos, &leb, 32, false)) {
        return false;
    }

    *value = (uint32_t)leb;
    return true;
}

bool wrp_read_vari7(uint8_t *buf, size_t buf_sz, size_t *pos, int8_t *value)
{
    if (*pos + sizeof(int8_t) >= buf_sz) {
        return false;
    }

    uint64_t leb = 0;

    if (!read_LEB(buf, pos, &leb, 7, true)) {
        return false;
    }

    *value = (int8_t)leb;
    return true;
}

bool wrp_read_vari32(uint8_t *buf, size_t buf_sz, size_t *pos, int32_t *value)
{
    if (*pos + sizeof(int32_t) >= buf_sz) {
        return false;
    }

    uint64_t leb = 0;

    if (!read_LEB(buf, pos, &leb, 32, true)) {
        return false;
    }

    *value = (int32_t)leb;
    return true;
}

bool wrp_read_vari64(uint8_t *buf, size_t buf_sz, size_t *pos, int64_t *value)
{
    if (*pos + sizeof(int64_t) >= buf_sz) {
        return false;
    }

    uint64_t leb = 0;

    if (!read_LEB(buf, pos, &leb, 64, true)) {
        return false;
    }

    *value = (int64_t)leb;
    return true;
}

bool wrp_read_string(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    char *str,
    size_t str_sz,
    uint32_t *str_len)
{
    uint32_t len = 0;
    if (!wrp_read_varui32(buf, buf_sz, pos, &len)) {
        return false;
    }

    if (len >= str_sz - 1) {
        return false;
    }

    for (uint32_t i = 0; i < len; i++) {
        str[i] = buf[(*pos)++];
    }

    str[len] = '\0';
    *str_len = len;
    return true;
}

bool wrp_read_limits(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    uint32_t *min,
    uint32_t *max,
    uint32_t default_max)
{
    uint32_t flags = 0;
    if (!wrp_read_varui32(buf, buf_sz, pos, &flags)) {
        return false;
    }

    if (!wrp_read_varui32(buf, buf_sz, pos, min)) {
        return false;
    }

    if (flags & 0x01) {
        if (!wrp_read_varui32(buf, buf_sz, pos, max)) {
            return false;
        }
    } else {
        *max = default_max;
    }

    if (*max < *min) {
        return false;
    }

    return true;
}

uint8_t wrp_get_uint8(uint8_t *buf, size_t *pos)
{
    return buf[(*pos)++];
}

uint32_t wrp_get_uint32(uint8_t *buf, size_t *pos)
{
    uint32_t result = 0;
    ((uint8_t *)(&result))[0] = buf[(*pos)++];
    ((uint8_t *)(&result))[1] = buf[(*pos)++];
    ((uint8_t *)(&result))[2] = buf[(*pos)++];
    ((uint8_t *)(&result))[3] = buf[(*pos)++];
    return result;
}

uint8_t wrp_get_varui1(uint8_t *buf, size_t *pos)
{
    uint64_t leb = 0;
    read_LEB(buf, pos, &leb, 1, false);
    return (uint8_t)leb;
}

uint8_t wrp_get_varui7(uint8_t *buf, size_t *pos)
{
    uint64_t leb = 0;
    read_LEB(buf, pos, &leb, 7, false);
    return (uint8_t)leb;
}

uint32_t wrp_get_varui32(uint8_t *buf, size_t *pos)
{
    uint64_t leb = 0;
    read_LEB(buf, pos, &leb, 32, false);
    return (uint32_t)leb;
}

int8_t wrp_get_vari7(uint8_t *buf, size_t *pos)
{
    uint64_t leb = 0;
    read_LEB(buf, pos, &leb, 7, true);
    return (uint8_t)leb;
}

int32_t wrp_get_vari32(uint8_t *buf, size_t *pos)
{
    uint64_t leb = 0;
    read_LEB(buf, pos, &leb, 32, true);
    return (int32_t)leb;
}

int64_t wrp_get_vari64(uint8_t *buf, size_t *pos)
{
    uint64_t leb = 0;
    read_LEB(buf, pos, &leb, 64, true);
    return (int64_t)leb;
}
