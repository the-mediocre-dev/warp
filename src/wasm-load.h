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

struct wasm_meta;
struct wasm_module;

struct wasm_module *wrp_load_module(uint8_t *buf,
    size_t buf_sz,
    struct wasm_meta *meta,
    warp_alloc_fn alloc_fn);

void wrp_unload_module(struct wasm_module *mdle, warp_free_fn free_fn);
