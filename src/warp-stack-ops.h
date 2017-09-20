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

#include <stddef.h>
#include <stdint.h>

struct wrp_vm;

uint32_t wrp_push_operand(struct wrp_vm *vm, uint64_t value, int8_t type);

uint32_t wrp_pop_operand(struct wrp_vm *vm, uint64_t *value, int8_t type);

uint32_t wrp_push_block(struct wrp_vm *vm, size_t label, int8_t type);

uint32_t wrp_pop_block(struct wrp_vm *vm);

uint32_t wrp_push_call(struct wrp_vm *vm, uint32_t func_idx);

uint32_t wrp_pop_call(struct wrp_vm *vm);
