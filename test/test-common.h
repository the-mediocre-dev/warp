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

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <warp.h>

#define WRP_ASSERT(condition, format, ...)      \
if (!(!!(condition))) {                         \
    fprintf(stderr, format, __VA_ARGS__);       \
    abort();                                    \
}                                               \

void *test_alloc(size_t size, size_t align);

void test_free(void *ptr);

bool load_buf(uint8_t *path, uint8_t **buf, size_t *buf_sz);

void free_buf(uint8_t *buf);

bool make_path(const char*path, const char *file, uint8_t *buf, size_t buf_sz);
