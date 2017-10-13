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

#include <stdint.h>

#include "warp-types.h"

wrp_err_t wrp_init_expr_i32(wrp_buf_t *buf, int32_t *out_value);

wrp_err_t wrp_init_expr_i64(wrp_buf_t *buf, int64_t *out_value);

wrp_err_t wrp_init_expr_f32(wrp_buf_t *buf, float *out_value);

wrp_err_t wrp_init_expr_f64(wrp_buf_t *buf, double *out_value);

wrp_err_t wrp_init_expr_get_global(wrp_buf_t *buf, int32_t *out_value);

wrp_err_t wrp_read_init_expr(wrp_buf_t *buf, uint8_t *out_expr, uint32_t *out_size);
