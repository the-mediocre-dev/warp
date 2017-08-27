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

#include "buf-io.h"
#include "example-common.h"
#include <warp.h>

void *example_alloc(size_t size, size_t align)
{
    return malloc(size);
}

void example_free(void *ptr)
{
    free(ptr);
}

void example_trap(struct warp_vm *vm, int err)
{
    fprintf(stderr, "trap: %d", err);
}

bool load_buf(const char *path, uint8_t **buf, size_t *buf_sz)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);

    if (len < 0) {
        fclose(file);
        return false;
    }

    *buf = example_alloc(len, 0);

    if (*buf == NULL) {
        fclose(file);
        return false;
    }

    *buf_sz = len;

    rewind(file);
    fread(*buf, *buf_sz, 1, file);
    fclose(file);
    return true;
}

void free_buf(uint8_t *buf)
{
    example_free(buf);
}
