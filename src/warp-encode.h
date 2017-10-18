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

#include <stdint.h>

uint64_t wrp_encode_i32(int32_t value);

uint64_t wrp_encode_i64(int64_t value);

uint64_t wrp_encode_f32(float value);

uint64_t wrp_encode_f64(double value);

int32_t wrp_decode_i32(uint64_t value);

int64_t wrp_decode_i64(uint64_t value);

float wrp_decode_f32(uint64_t value);

double wrp_decode_f64(uint64_t value);
