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

#include <assert.h>
#include <limits.h>
#include <stdint.h>

_Static_assert(CHAR_BIT == 8, "warp requires 8 bit bytes");

_Static_assert(sizeof(uint8_t) * CHAR_BIT == 8, "warp requires 8 bit uint8_t");

_Static_assert(sizeof(int8_t) * CHAR_BIT == 8, "warp requires 8 bit int8_t");

_Static_assert(sizeof(uint32_t) * CHAR_BIT == 32, "warp requires 32 bit uint32_t");

_Static_assert(sizeof(int32_t) * CHAR_BIT == 32, "warp requires 32 bit int32_t");

_Static_assert(sizeof(uint64_t) * CHAR_BIT == 64, "warp requires 64 bit uint64_t");

_Static_assert(sizeof(int64_t) * CHAR_BIT == 64, "warp requires 64 bit int64_t");

_Static_assert(sizeof(float) * CHAR_BIT == 32, "warp requires 32 bit float");

_Static_assert(sizeof(double) * CHAR_BIT == 64, "warp requires 64 bit double");
