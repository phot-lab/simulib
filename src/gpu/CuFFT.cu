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
 * Created: 2022/3/31
 * Supported by: National Key Research and Development Program of China
 */

#include <Internal>
#include <complex>
#include <cublas_v2.h>
#include <cufft.h>
#include <iostream>

using namespace std;

#define IDX(i, j, ld) (((j) * (ld)) + (i))

static void cuFFT(complex<double> *data, int rows, int cols) {
    cufftHandle plan;
    cufftDoubleComplex *dataDev;
    HANDLE_ERROR(cudaMalloc((void **) &dataDev, sizeof(cufftDoubleComplex) * rows * cols));
    HANDLE_ERROR(cudaMemcpy(dataDev, data, sizeof(cufftDoubleComplex) * rows * cols, cudaMemcpyHostToDevice));

    if (cufftPlan1d(&plan, rows, CUFFT_Z2Z, cols) != CUFFT_SUCCESS) {
        fprintf(stderr, "CUFFT error: Plan creation failed");
        return;
    }

    // Notes: Identical pointers to input and output arrays implies in-place transformation
    if (cufftExecZ2Z(plan, dataDev, dataDev, CUFFT_FORWARD) != CUFFT_SUCCESS) {
        fprintf(stderr, "CUFFT error: ExecZ2Z Forward failed");
        return;
    }

    cudaMemcpy(data, dataDev, sizeof(cufftDoubleComplex) * rows * cols, cudaMemcpyDeviceToHost);
}

static void cuScale(complex<double> *data, complex<double> alpha, int rows, int cols) {
    cublasHandle_t handle;
    cublasStatus_t stat;
    stat = cublasCreate(&handle);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        cout << "CUBLAS initialization failed" << endl;
        return;
    }
    cufftDoubleComplex *dataDev;
    cudaMalloc(&dataDev, sizeof(cufftDoubleComplex) * rows * cols);

    cublasSetMatrix(rows, cols, sizeof(cufftDoubleComplex), data, rows, dataDev, rows);
    cufftDoubleComplex temp;
    temp.x = alpha.real();
    temp.y = alpha.imag();
    cublasZscal(handle, rows * cols, &temp, dataDev, 1);
    cublasGetMatrix(rows, cols, sizeof(cufftDoubleComplex), dataDev, rows, data, rows);

    cudaFree(dataDev);
    cublasDestroy(handle);
}

static void cuIFFT(complex<double> *data, int rows, int cols) {
    cufftHandle plan;
    cufftDoubleComplex *dataDev;
    HANDLE_ERROR(cudaMalloc((void **) &dataDev, sizeof(cufftDoubleComplex) * rows * cols));
    HANDLE_ERROR(cudaMemcpy(dataDev, data, sizeof(cufftDoubleComplex) * rows * cols, cudaMemcpyHostToDevice));

    if (cufftPlan1d(&plan, rows, CUFFT_Z2Z, cols) != CUFFT_SUCCESS) {
        fprintf(stderr, "CUFFT error: Plan creation failed");
        return;
    }

    // Notes: Identical pointers to input and output arrays implies in-place transformation
    if (cufftExecZ2Z(plan, dataDev, dataDev, CUFFT_INVERSE) != CUFFT_SUCCESS) {
        fprintf(stderr, "CUFFT error: ExecZ2Z Forward failed");
        return;
    }

    cudaMemcpy(data, dataDev, sizeof(cufftDoubleComplex) * rows * cols, cudaMemcpyDeviceToHost);
    cufftDestroy(plan);
    cudaFree(dataDev);
    complex<double> scalar((double) 1 / (rows), 0);
    cuScale(data, scalar, rows, cols);
}

namespace SimuLib {

namespace GPU{

VectorXcd fft(const VectorXcd &in) {
    VectorXcd out = in;
    cuFFT(out.data(), (int) out.rows(), (int) out.cols());
    return out;
}

VectorXcd ifft(const VectorXcd &in) {
    VectorXcd out = in;
    cuIFFT(out.data(), (int) out.rows(), (int) out.cols());
    return out;
}

MatrixXcd fftCol(const MatrixXcd &in) {
    MatrixXcd out(in.rows(), in.cols());
    for (Index i = 0; i < in.cols(); ++i) {
        out.col(i) = fft(in.col(i));
    }
    return out;
}

MatrixXcd ifftCol(const MatrixXcd &in) {
    MatrixXcd out(in.rows(), in.cols());
    for (Index i = 0; i < in.cols(); ++i) {
        out.col(i) = ifft(in.col(i));
    }
    return out;
}

}

}  // namespace SimuLib
