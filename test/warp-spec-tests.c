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

#include "test-common.h"
#include "if-tests.h"

#define NUM_TESTS 1
#define MAX_FILE_PATH 4096

typedef void (*test_fn)(struct wrp_vm *vm, const char *path, uint8_t *buf, size_t buf_sz);

static const test_fn test_jump_table[] = {
    test_if
};

int main(int argc, char** argv)
{
    WRP_ASSERT(argc >= 2, "invalid args");

    struct wrp_vm *vm = wrp_open_vm(test_alloc, test_free);
    WRP_ASSERT(vm, "vm failed to initialise");

    uint8_t *path_buf = test_alloc(MAX_FILE_PATH + 1, 0);
    WRP_ASSERT(path_buf, "failed to allocate path buffer");

    for(uint32_t i = 0; i < NUM_TESTS; i++){
        test_jump_table[i](vm, argv[2], path_buf, MAX_FILE_PATH + 1);
    }

    test_free(path_buf);
    wrp_close_vm(vm);
    return 0;
}
