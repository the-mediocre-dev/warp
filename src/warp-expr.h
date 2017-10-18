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

#include "warp-types.h"

bool wrp_is_valid_init_expr_opcode(uint8_t opcode);

wrp_err_t wrp_skip_expr(wrp_buf_t *buf, uint8_t *out_opcode, size_t *out_expr_sz);

wrp_err_t wrp_skip_init_expr(wrp_buf_t *buf, size_t *out_init_expr_sz);

