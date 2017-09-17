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

void *test_alloc(size_t size, size_t align)
{
    void *ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void test_free(void *ptr)
{
    free(ptr);
}

bool load_buf(uint8_t *path, uint8_t **buf, size_t *buf_sz)
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

void free_buf(uint8_t *buf)
{
    free(buf);
}

bool make_path(const char *path, const char *file, uint8_t *buf, size_t buf_sz)
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
