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

#include "value-stack.h"
#include "wasm.h"

static bool push_value(struct value_stack *stk, uint64_t value, uint8_t type)
{
    if (stk->head >= STACK_SIZE) {
        return false;
    }

    stk->head++;
    stk->values[stk->head] = value;
    stk->types[stk->head] = type;
    return true;
}

static bool pop_value(struct value_stack *stk, uint64_t *value, uint8_t type)
{
    if (stk->head == 0 || stk->types[stk->head] != type) {
        return false;
    }

    *value = stk->values[stk->head--];
    return true;
}

bool warp_push_i32(struct value_stack *stk, int32_t value)
{
    return push_value(stk, (uint64_t)value, I32);
}

bool warp_push_i64(struct value_stack *stk, int64_t value)
{
    return push_value(stk, (uint64_t)value, I64);
}

bool warp_push_f32(struct value_stack *stk, float value)
{
    return push_value(stk, (uint64_t)value, F32);
}

bool warp_push_f64(struct value_stack *stk, double value)
{
    return push_value(stk, (uint64_t)value, F64);
}

bool warp_pop_i32(struct value_stack *stk, int32_t *value)
{
    return pop_value(stk, (uint64_t *)value, I32);
}

bool warp_pop_i64(struct value_stack *stk, int64_t *value)
{
    return pop_value(stk, (uint64_t *)value, I64);
}

bool warp_pop_f32(struct value_stack *stk, float *value)
{
    return pop_value(stk, (uint64_t *)value, F32);
}

bool warp_pop_f64(struct value_stack *stk, double *value)
{
    return pop_value(stk, (uint64_t *)value, F64);
}
