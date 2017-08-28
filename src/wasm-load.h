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

#include "warp-common.h"

struct wrp_wasm_meta;
struct wrp_wasm_mdle;

struct wrp_wasm_mdle *wrp_instantiate_mdle(uint8_t *buf,
    size_t buf_sz,
    struct wrp_wasm_meta *meta,
    wrp_alloc_fn alloc_fn);

void wrp_destroy_mdle(struct wrp_wasm_mdle *mdle, wrp_free_fn free_fn);
