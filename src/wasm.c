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

#include "wasm.h"

bool wrp_get_func_idx(struct wrp_wasm_mdle *mdle,
    const char *func_name,
    uint32_t *func_idx)
{
    for (uint32_t i = 0; i < mdle->num_exports; i++) {
        char *name = &mdle->export_names[mdle->export_name_offsets[i]];

        if (strcmp(name, func_name) == 0) {
            *func_idx = mdle->export_func_idxs[i];
            return true;
        }
    }

    return false;
}
