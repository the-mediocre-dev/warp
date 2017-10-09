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
#include <stddef.h>
#include <stdint.h>

#include "warp-types.h"

typedef struct wrp_buf {
    uint8_t *bytes;
    size_t sz;
    size_t pos;
} wrp_buf_t;

wrp_err_t wrp_seek(wrp_buf_t *buf, size_t new_pos);

bool wrp_end_of_buf(wrp_buf_t *buf);

wrp_err_t wrp_skip(wrp_buf_t *buf, size_t num_bytes);

wrp_err_t wrp_read_uint8(wrp_buf_t *buf, uint8_t *out_value);

wrp_err_t wrp_read_uint32(wrp_buf_t *buf, uint32_t *out_value);

wrp_err_t wrp_read_varui1(wrp_buf_t *buf, uint8_t *out_value);

wrp_err_t wrp_read_varui7(wrp_buf_t *buf, uint8_t *out_value);

wrp_err_t wrp_read_varui32(wrp_buf_t *buf, uint32_t *out_value);

wrp_err_t wrp_read_vari7(wrp_buf_t *buf, int8_t *out_value);

wrp_err_t wrp_read_vari32(wrp_buf_t *buf, int32_t *out_value);

wrp_err_t wrp_read_vari64(wrp_buf_t *buf, int64_t *out_value);

wrp_err_t wrp_read_f32(wrp_buf_t *buf, float *out_value);

wrp_err_t wrp_read_f64(wrp_buf_t *buf, double *out_value);

wrp_err_t wrp_read_string(wrp_buf_t *buf,
    char *out_str,
    size_t str_sz,
    uint32_t *out_str_len);

wrp_err_t wrp_read_limits(wrp_buf_t *buf, uint32_t *out_min, uint32_t *out_max);
