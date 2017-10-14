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

static bool load_buf(uint8_t *path, wrp_buf_t *buf)
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

    buf->bytes = malloc(len);

    if (buf->bytes == NULL) {
        fclose(file);
        return false;
    }

    buf->sz = len;

    rewind(file);
    fread(buf->bytes, buf->sz, 1, file);
    fclose(file);
    return true;
}

void *test_alloc(size_t size, size_t align)
{
    size_t actual_sz = size + align;

    void *ptr = malloc(actual_sz);
    memset(ptr, 0, actual_sz);

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

void load_mdle(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    const char *mdle_name)
{
    wrp_reset_vm(vm);

    printf("loading test module %s\n", mdle_name);

    ASSERT(make_path(dir, mdle_name, path_buf, path_buf_sz), "failed to make path to \"%s\"", mdle_name);

    wrp_buf_t buf = {0};
    ASSERT(load_buf(path_buf, &buf), "failed to load \"%s\"", mdle_name);

    wrp_wasm_mdle_t *mdle = wrp_instantiate_mdle(vm, &buf);
    ASSERT(mdle, "failed to instantiate \"%s\"", mdle_name);
    ASSERT(wrp_link_mdle(vm, mdle) == WRP_SUCCESS, "failed to attach module \"%s\"", mdle_name);

    free(buf.bytes);
}

wrp_err_t validate_mdle(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    const char *mdle_name)
{
    wrp_reset_vm(vm);

    ASSERT(make_path(dir, mdle_name, path_buf, path_buf_sz), "failed to make path to \"%s\"", mdle_name);

    wrp_buf_t buf = {0};
    ASSERT(load_buf(path_buf, &buf), "failed to load \"%s\"", mdle_name);

    wrp_wasm_mdle_t *mdle = wrp_instantiate_mdle(vm, &buf);

    if (mdle != NULL) {
        wrp_destroy_mdle(vm, mdle);
    }

    return vm->err;
}

wrp_err_t link_mdle(wrp_vm_t *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    const char *mdle_name)
{
    wrp_reset_vm(vm);

    ASSERT(make_path(dir, mdle_name, path_buf, path_buf_sz), "failed to make path to \"%s\"", mdle_name);

    wrp_buf_t buf = {0};
    ASSERT(load_buf(path_buf, &buf), "failed to load \"%s\"", mdle_name);

    wrp_wasm_mdle_t *mdle = wrp_instantiate_mdle(vm, &buf);

    if (mdle == NULL) {
        return vm->err;
    }
}

void unload_mdle(wrp_vm_t *vm)
{
    printf("unloading test module\n\n");

    wrp_wasm_mdle_t *mdle = vm->mdle;
    ASSERT(wrp_unlink_mdle(vm) == WRP_SUCCESS, "failed to detach modile");
    wrp_destroy_mdle(vm, mdle);
}
