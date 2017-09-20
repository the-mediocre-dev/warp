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

#include "if-tests.h"
#include "test-common.h"

bool if_empty_test_01(struct wrp_vm *vm)
{
    wrp_reset_vm(vm);

    uint32_t func_idx = 0;
    if(wrp_get_func_idx(vm->mdle, "empty", &func_idx) != WRP_SUCCESS){
        return false;
    }

    if(!wrp_push_i32(vm, 0)){
        return false;
    }

    if(!wrp_call(vm, func_idx)){
        return false;
    }

    if(vm->call_stk_head != -1 || vm->operand_stk_head != -1 || vm->block_stk_head != -1){
        return false;
    }

    return true;
}

bool if_empty_test_02(struct wrp_vm *vm)
{
    wrp_reset_vm(vm);

    uint32_t func_idx = 0;
    if(wrp_get_func_idx(vm->mdle, "empty", &func_idx) != WRP_SUCCESS){
        return false;
    }

    if(!wrp_push_i32(vm, 1)){
        return false;
    }

    if(!wrp_call(vm, func_idx)){
        return false;
    }

    if(vm->call_stk_head != -1 || vm->operand_stk_head != -1 || vm->block_stk_head != -1){
        return false;
    }

    return true;
}

bool if_empty_test_03(struct wrp_vm *vm)
{
    wrp_reset_vm(vm);

    uint32_t func_idx = 0;
    if(wrp_get_func_idx(vm->mdle, "empty", &func_idx) != WRP_SUCCESS){
        return false;
    }

    if(!wrp_push_i32(vm, 100)){
        return false;
    }

    if(!wrp_call(vm, func_idx)){
        return false;
    }

    if(vm->call_stk_head != -1 || vm->operand_stk_head != -1 || vm->block_stk_head != -1){
        return false;
    }

    return true;
}

bool if_empty_test_04(struct wrp_vm *vm)
{
    wrp_reset_vm(vm);

    uint32_t func_idx = 0;
    if(wrp_get_func_idx(vm->mdle, "empty", &func_idx) != WRP_SUCCESS){
        return false;
    }

    if(!wrp_push_i32(vm, -2)){
        return false;
    }

    if(!wrp_call(vm, func_idx)){
        return false;
    }

    if(vm->call_stk_head != -1 || vm->operand_stk_head != -1 || vm->block_stk_head != -1){
        return false;
    }

    return true;
}

void run_if_tests(struct wrp_vm *vm,
    const char *dir,
    uint8_t *path_buf,
    size_t path_buf_sz,
    uint32_t *passed,
    uint32_t *failed)
{
    WRP_START_TESTS(vm, dir, path_buf, path_buf_sz, "if.wasm");
    WRP_RUN_TEST(vm, if_empty_test_01, *passed, *failed);
    WRP_RUN_TEST(vm, if_empty_test_02, *passed, *failed);
    WRP_RUN_TEST(vm, if_empty_test_03, *passed, *failed);
    WRP_RUN_TEST(vm, if_empty_test_04, *passed, *failed);
    WRP_END_TESTS(vm);
}
