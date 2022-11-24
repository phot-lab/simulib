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
 * Created: 2022/4/12
 * Supported by: National Key Research and Development Program of China
 */
#ifndef FUNCTIONSDOUBLE_HPP
#define FUNCTIONSDOUBLE_HPP

#include "../gpu/Tools.cuh"
#include <cublas_v2.h>
#include <cuda_runtime.h>

namespace SimuLib {
    template<int Rows, int Cols>
    Matrix<double, Rows, Cols> geam(const Matrix<double, Rows, Cols> &m, const Matrix<double, Rows, Cols> &other, bool addition) {
        int rows = m.rows();
        int cols = other.cols();
        int size = rows * cols;
        cublasHandle_t handle;
        HANDLE_STATUS(cublasCreate(&handle));

        Matrix<double, Rows, Cols> result(rows, cols);
        double *dev1;
        double *dev2;
        double *devResult;
        size_t sizeOfType = sizeof(double);
        HANDLE_ERROR(cudaMalloc(&dev1, size * sizeOfType));
        HANDLE_ERROR(cudaMalloc(&dev2, size * sizeOfType));
        HANDLE_ERROR(cudaMalloc(&devResult, size * sizeOfType));

        HANDLE_ERROR(cudaMemcpy(dev1, m.data(), size * sizeOfType, cudaMemcpyHostToDevice));
        HANDLE_ERROR(cudaMemcpy(dev2, other.data(), size * sizeOfType, cudaMemcpyHostToDevice));

        if (addition) {
            double alpha = 1;
            double beta  = 1;
            HANDLE_STATUS(cublasDgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, rows, cols, &alpha, dev1,
                                      rows,
                                      &beta, dev2, rows, devResult, rows));
        } else {
            double alpha = 1;
            double beta  = -1;
            HANDLE_STATUS(cublasDgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, rows, cols, &alpha, dev1,
                                      rows,
                                      &beta, dev2, rows, devResult, rows));
        }

        HANDLE_ERROR(cudaMemcpy(result.data(), devResult, size * sizeOfType, cudaMemcpyDeviceToHost));
        HANDLE_ERROR(cudaFree(dev1));
        HANDLE_ERROR(cudaFree(dev2));
        HANDLE_ERROR(cudaFree(devResult));
        HANDLE_STATUS(cublasDestroy(handle));
        return result;
    }

    template<int Rows, int Cols>
    Matrix<double, Rows, Cols> gemm(const Matrix<double, Rows, Cols> &thisMatrix, const Matrix<double, Rows, Cols> &other) {

        assert(thisMatrix.cols() == other.rows());
        int m = thisMatrix.rows();
        int n = thisMatrix.cols();
        int k = other.cols();

        cublasHandle_t handle;
        HANDLE_STATUS(cublasCreate(&handle));

        Matrix<double, Rows, Cols> result(m, k);
        double *dev1;
        double *dev2;
        double *devResult;
        size_t sizeOfType = sizeof(double);
        HANDLE_ERROR(cudaMalloc(&dev1, m * n * sizeOfType));
        HANDLE_ERROR(cudaMalloc(&dev2, n * k * sizeOfType));
        HANDLE_ERROR(cudaMalloc(&devResult, m * k * sizeOfType));

        HANDLE_ERROR(cudaMemcpy(dev1, thisMatrix.data(), m * n * sizeOfType, cudaMemcpyHostToDevice));
        HANDLE_ERROR(cudaMemcpy(dev2, other.data(), n * k * sizeOfType, cudaMemcpyHostToDevice));

        double alpha = 1;
        double beta  = 0;
        HANDLE_STATUS(cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, &alpha, dev1, m, dev2, n, &beta, devResult, m));

        HANDLE_ERROR(cudaMemcpy(result.data(), devResult, m * k * sizeOfType, cudaMemcpyDeviceToHost));
        HANDLE_ERROR(cudaFree(dev1));
        HANDLE_ERROR(cudaFree(dev2));
        HANDLE_ERROR(cudaFree(devResult));
        HANDLE_STATUS(cublasDestroy(handle));
        return result;
    }
}  // namespace ParMat

#endif  // FUNCTIONSDOUBLE_HPP
