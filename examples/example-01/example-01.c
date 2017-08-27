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

#include <stdio.h>
#include <stdlib.h>

#include <buf-io.h>
#include <example-common.h>
#include <warp.h>
#include <wasm-load.h>
#include <wasm-validation.h>
#include <wasm.h>

int main(int argc, char **argv)
{
    struct warp_vm *vm = wrp_open_vm(example_alloc, example_free, example_trap);

    if (vm == NULL) {
        fprintf(stderr, "opening vm from failed");
        abort();
    }

    uint8_t *buf = NULL;
    size_t buf_sz;

    if (!load_buf("square.wasm", &buf, &buf_sz)) {
        fprintf(stderr, "opening stream from 'square.wasm' failed");
        abort();
    }

    struct wasm_meta meta = {};

    if (!wrp_validate_module(buf, buf_sz, &meta)) {
        fprintf(stderr, "validation of module 'square.wasm' failed");
        abort();
    }

    struct wasm_module *mdle = wrp_load_module(buf, buf_sz, &meta, example_alloc);

    if (mdle == NULL) {
        fprintf(stderr, "loading module 'square.wasm' failed");
        abort();
    }

    free_buf(buf);

    //TODO run script

    wrp_unload_module(mdle, example_free);
    wrp_close_vm(vm);

    return 0;
}
