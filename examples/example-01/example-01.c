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

#include <example-common.h>
#include <warp.h>

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
        fprintf(stderr, "opening 'square.wasm' failed");
        abort();
    }

    struct wrp_wasm_mdle *mdle = wrp_instantiate_mdle(vm, buf, buf_sz);

    if(mdle == NULL){
        fprintf(stderr, "instantiating 'square.wasm' failed");
        abort();
    }

    free_buf(buf);
    wrp_attach_mdle(vm, mdle);
    wrp_push_i32(vm, 3);
    wrp_call(vm, 0);

    int32_t result = 0;
    wrp_pop_i32(vm, &result);

    wrp_destroy_mdle(vm, mdle);
    wrp_close_vm(vm);
    return 0;
}
