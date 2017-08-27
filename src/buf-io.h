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

#include <stdbool.h>
#include <stdint.h>

bool wrp_seek(uint8_t *buf, size_t buf_sz, size_t *pos, size_t new_pos);

bool wrp_end_of_buf(uint8_t *buf, size_t buf_sz, size_t pos);

bool wrp_skip(uint8_t *buf, size_t buf_sz, size_t *pos, size_t num_bytes);

bool wrp_read_uint8(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value);

bool wrp_read_uint32(uint8_t *buf, size_t buf_sz, size_t *pos, uint32_t *value);

bool wrp_read_varui1(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value);

bool wrp_read_varui7(uint8_t *buf, size_t buf_sz, size_t *pos, uint8_t *value);

bool wrp_read_varui32(uint8_t *buf, size_t buf_sz, size_t *pos, uint32_t *value);

bool wrp_read_vari7(uint8_t *buf, size_t buf_sz, size_t *pos, int8_t *value);

bool wrp_read_vari32(uint8_t *buf, size_t buf_sz, size_t *pos, int32_t *value);

bool wrp_read_vari64(uint8_t *buf, size_t buf_sz, size_t *pos, int64_t *value);

bool wrp_read_string(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    char *str,
    size_t str_sz,
    uint32_t *str_len);

bool wrp_read_limits(uint8_t *buf,
    size_t buf_sz,
    size_t *pos,
    uint32_t *min,
    uint32_t *max,
    uint32_t default_max);

uint8_t wrp_get_uint8(uint8_t *buf, size_t *pos);

uint32_t wrp_get_uint32(uint8_t *buf, size_t *pos);

uint8_t wrp_get_varui1(uint8_t *buf, size_t *pos);

uint8_t wrp_get_varui7(uint8_t *buf, size_t *pos);

uint32_t wrp_get_varui32(uint8_t *buf, size_t *pos);

int8_t wrp_get_vari7(uint8_t *buf, size_t *pos);

int32_t wrp_get_vari32(uint8_t *buf, size_t *pos);

int64_t wrp_get_vari64(uint8_t *buf, size_t *pos);
