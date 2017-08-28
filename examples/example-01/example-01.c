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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <buf-io.h>
#include <example-common.h>
#include <warp.h>
#include <wasm-execution.h>
#include <wasm-load.h>
#include <wasm-validation.h>
#include <wasm.h>

int main(int argc, char **argv)
{
    struct wrp_vm *vm = wrp_open_vm(example_alloc, example_free, example_trap);

    if (vm == NULL) {
        fprintf(stderr, "opening vm failed");
        abort();
    }

    uint8_t *buf = NULL;
    size_t buf_sz;

    if (!load_buf("square.wasm", &buf, &buf_sz)) {
        fprintf(stderr, "opening stream from 'square.wasm' failed");
        abort();
    }

    struct wrp_wasm_meta meta = {};

    if (!wrp_validate_module(buf, buf_sz, &meta)) {
        fprintf(stderr, "validation of module 'square.wasm' failed");
        abort();
    }

    struct wrp_wasm_mdle *mdle = wrp_instantiate_mdle(buf, buf_sz, &meta, example_alloc);

    if (mdle == NULL) {
        fprintf(stderr, "loading module 'square.wasm' failed");
        abort();
    }

    free_buf(buf);

    uint32_t func_idx = 0;
    if (!wrp_get_func_idx(mdle, "square", &func_idx)) {
        fprintf(stderr, "insert some error message");
        abort();
    }

    wrp_attach_mdle(vm, mdle);

    wrp_push_i32(vm, 3);

    wrp_call(vm, func_idx);

    int32_t result = 0;
    wrp_pop_i32(vm, &result);

    wrp_destroy_mdle(mdle, example_free);
    wrp_close_vm(vm);
    return 0;
}
