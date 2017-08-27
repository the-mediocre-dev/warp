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

#include "warp.h"
#include "wasm.h"

struct warp_vm *wrp_open_vm(warp_alloc_fn alloc_fn,
    warp_free_fn free_fn,
    warp_trap_fn trap_fn)
{
    if (!alloc_fn || !free_fn || !trap_fn) {
        return NULL;
    }

    struct warp_vm *vm = alloc_fn(sizeof(struct warp_vm), 0);

    if (!vm) {
        return NULL;
    }

    vm->alloc_fn = alloc_fn;
    vm->free_fn = free_fn;
    vm->trap_fn = trap_fn;
    return vm;
}

void wrp_close_vm(struct warp_vm *vm)
{
    vm->free_fn(vm);
}
