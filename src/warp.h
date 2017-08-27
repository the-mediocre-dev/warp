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
#include "warp-config.h"

struct wasm_module;
struct value_stack;

struct warp_vm {
    warp_alloc_fn alloc_fn;
    warp_free_fn free_fn;
    warp_trap_fn trap_fn;
    struct wasm_module *mdl;
    struct value_stack *stk;
} warp_vm;

struct warp_vm *wrp_open_vm(warp_alloc_fn alloc_fn,
    warp_free_fn free_fn,
    warp_trap_fn trap_fn);

void wrp_close_vm(struct warp_vm *vm);
