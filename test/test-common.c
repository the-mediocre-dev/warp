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

#include <string.h>

#include "test-common.h"

static bool make_path(const char *path, const char *file, uint8_t *buf, size_t buf_sz)
{
    size_t path_len = strlen(path);
    size_t file_len = strlen(file);

    if (path_len + file_len + 1 >= buf_sz)
        return false;

    size_t current_char = 0;

    for (size_t i = 0; i < path_len; i++)
        buf[current_char++] = path[i];

    for (size_t i = 0; i < file_len; i++)
        buf[current_char++] = file[i];

    buf[current_char] = '\0';
    return true;
}

static bool load_buf(uint8_t *path, uint8_t **buf, size_t *buf_sz)
{
    FILE *file = fopen((char *)path, "rb");

    if (file == NULL) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);

    if (len < 0) {
        fclose(file);
        return false;
    }

    *buf = malloc(len);

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

void *test_alloc(size_t size, size_t align)
{
    void *ptr = malloc(size + align);
    memset(ptr, 0, size);

    //0 alignment not supported. Need the extra byte for storing adjustment.
    size_t actual_align = align > 1 ? align : 1;

    uintptr_t address = (uintptr_t)ptr;
    uintptr_t misalign = address & (actual_align - 1);
    ptrdiff_t adjustment = actual_align - misalign;
    uintptr_t aligned_address = address + adjustment;

    //store adjustment for use when freeing memory
    uint8_t *aligned_ptr = (void *)aligned_address;
    aligned_ptr[-1] = adjustment;

    return aligned_ptr;
}

void test_free(void *ptr)
{
    //retrieve the adjustment from the preceding byte
    uint8_t *aligned_ptr = ptr;
    ptrdiff_t adjustment = aligned_ptr[-1];

    //adjust address
    uintptr_t aligned_address = (uintptr_t)ptr;
    uintptr_t address = aligned_address - adjustment;

    void *unaligned_ptr = (void *)address;

    free(unaligned_ptr);
}

void load_test_mdle(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    const char* mdle_name)
{
    WRP_ASSERT(make_path(dir, mdle_name, path_buf, path_buf_sz), "failed to make path to \"%s\"", mdle_name);

    uint8_t *buf = NULL;
    size_t buf_sz = 0;
    WRP_ASSERT(load_buf(path_buf, &buf, &buf_sz), "failed to load \"%s\"", mdle_name);

    struct wrp_wasm_mdle *mdle = wrp_instantiate_mdle(vm, buf, buf_sz);
    WRP_ASSERT(mdle, "failed to instantiate \"%s\"", mdle_name);
    WRP_ASSERT(wrp_attach_mdle(vm, mdle), "failed to attach module \"%s\"", mdle_name);

    free(buf);
}

void unload_test_mdle(struct wrp_vm *vm)
{
    struct wrp_wasm_mdle *mdle = vm->mdle;
    WRP_ASSERT(wrp_detach_mdle(vm), "failed to detach modle");
    wrp_destroy_mdle(vm, mdle);
}
