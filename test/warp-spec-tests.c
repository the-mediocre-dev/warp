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

#include <stdalign.h>
#include <stdint.h>

#include "test-common.h"
#include "i32-tests.h"
#include "if-tests.h"

#define MAX_FILE_PATH 4096

void print_header()
{
    printf("--------------------------------------------------------------------------------\n");
    printf("                                                                                \n");
    printf("                             WARP SPEC TESTS                                    \n");
    printf("                                                                                \n");
    printf("--------------------------------------------------------------------------------\n");
}

void print_footer(uint32_t passed, uint32_t failed)
{
    printf(GREEN_TEXT("passed:%d  "), passed);
    printf(RED_TEXT("failed:%d\n\n"), failed);

    if(failed == 0){
        printf(GREEN_TEXT("spec tests passed"));
    }else{
        printf(RED_TEXT("spec tests failed"));
    }
}

int main(int argc, char** argv)
{
    WRP_ASSERT(argc >= 2, "invalid args");

    print_header();

    struct wrp_vm *vm = wrp_open_vm(test_alloc, test_free);
    WRP_ASSERT(vm, "vm failed to initialise");

    uint8_t *path_buf = test_alloc(MAX_FILE_PATH + 1, alignof(uint8_t));
    WRP_ASSERT(path_buf, "failed to allocate path buffer");

    uint32_t passed = 0;
    uint32_t failed = 0;
    run_i32_tests(vm, argv[1], path_buf, MAX_FILE_PATH + 1, &passed, &failed);
    run_if_tests(vm, argv[1], path_buf, MAX_FILE_PATH + 1, &passed, &failed);

    test_free(path_buf);
    wrp_close_vm(vm);

    print_footer(passed, failed);

    return (failed == 0) ? 0 : 1;
}


