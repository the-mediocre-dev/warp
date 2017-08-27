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

#include <stdbool.h>
#include <stdint.h>

#define STACK_SIZE 1024u

struct value_stack {
    uint64_t values[STACK_SIZE];
    uint8_t types[STACK_SIZE];
    uint32_t head;
};

bool warp_push_i32(struct value_stack *stk, int32_t value);

bool warp_push_i64(struct value_stack *stk, int64_t value);

bool warp_push_f32(struct value_stack *stk, float value);

bool warp_push_f64(struct value_stack *stk, double value);

bool warp_pop_i32(struct value_stack *stk, int32_t *value);

bool warp_pop_i64(struct value_stack *stk, int64_t *value);

bool warp_pop_f32(struct value_stack *stk, float *value);

bool warp_pop_f64(struct value_stack *stk, double *value);
