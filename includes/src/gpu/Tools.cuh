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
 * Created: 2022/3/29
 * Supported by: National Key Research and Development Program of China
 */

#ifndef CUDAPROJECT_TOOLS_H
#define CUDAPROJECT_TOOLS_H

#include <cuda_runtime.h>
#include <cublas_v2.h>

void handleError(cudaError_t err, const char *file, int line);
void handleStatus(cublasStatus_t status, const char *file, int line);

#define HANDLE_ERROR(err) (handleError(err, __FILE__, __LINE__))
#define HANDLE_STATUS(status) (handleStatus(status, __FILE__, __LINE__))

#endif  // CUDAPROJECT_TOOLS_H
