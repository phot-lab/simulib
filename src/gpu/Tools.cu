/**
* Copyright (c) 2022 Beijing Jiaotong University
* OpticaLab is licensed under [Open Source License].
* You can use this software according to the terms and conditions of the [Open Source License].
* You may obtain a copy of [Open Source License] at: [https://open.source.license/]
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the [Open Source License] for more details.
*/
/**
* Author: Chunyu Li
* Created: 2022/4/1
* Supported by: National Key Research and Development Program of China
*/

#include <cstdio>

void handleError(cudaError_t err, const char *file, int line) {

    // CUDA error handling from the "CUDA by example" book
    if (err != cudaSuccess) {
        printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
        exit(EXIT_FAILURE);
    }
}