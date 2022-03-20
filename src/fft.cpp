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
 * Created: 2022/3/8
 * Supported by: National Key Research and Development Program of China
 */

#include "simulib"
#include <iostream>
#include <mkl.h>
#include <vector>

using namespace Eigen;
using namespace std;

// The implementation of FFT referred to this website
// https://stackoverflow.com/questions/29805767/is-there-any-simple-c-example-on-how-to-use-intel-mkl-fft
VectorXcd FFT(const VectorXcd &in) {
    VectorXcd out(in.size());
    DFTI_DESCRIPTOR_HANDLE descriptor;
    MKL_LONG status;

    // Note after each operation status should be 0 on success
    status = DftiCreateDescriptor(&descriptor, DFTI_DOUBLE, DFTI_COMPLEX, 1, in.size());  // Specify size and precision
    status = DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_NOT_INPLACE);                  // Out of place FFT
    status = DftiCommitDescriptor(descriptor);                                            // Finalize the descriptor
    status = DftiComputeForward(descriptor, (void *) in.data(), out.data());              // Compute the Forward FFT
    status = DftiFreeDescriptor(&descriptor);                                             // Free the descriptor
    return out;
}

VectorXcd IFFT(const VectorXcd &in) {
    VectorXcd out(in.size());

    DFTI_DESCRIPTOR_HANDLE descriptor;
    MKL_LONG status;

    status = DftiCreateDescriptor(&descriptor, DFTI_DOUBLE, DFTI_COMPLEX, 1, in.size());  // Specify size and precision
    status = DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_NOT_INPLACE);                  // Out of place FFT
    status = DftiSetValue(descriptor, DFTI_BACKWARD_SCALE, 1.0 / (double) in.size());     // Scale down the output
    status = DftiCommitDescriptor(descriptor);                                            // Finalize the descriptor
    status = DftiComputeBackward(descriptor, (void *) in.data(), out.data());             // Compute the Forward FFT
    status = DftiFreeDescriptor(&descriptor);                                             // Free the descriptor

    return out;
}

MatrixXcd FFT2D(const MatrixXcd &in) {
    MatrixXcd out(in.rows(), in.cols());
    DFTI_DESCRIPTOR_HANDLE descriptor;
    MKL_LONG status;
    MKL_LONG dim_sizes[2] = {in.rows(), in.cols()};

    // Note after each operation status should be 0 on success
    status = DftiCreateDescriptor(&descriptor, DFTI_DOUBLE, DFTI_COMPLEX, 2, dim_sizes);  // Specify size and precision
    status = DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_NOT_INPLACE);                  // Out of place FFT
    status = DftiCommitDescriptor(descriptor);                                            // Finalize the descriptor
    status = DftiComputeForward(descriptor, (void *) in.data(), out.data());              // Compute the Forward FFT
    status = DftiFreeDescriptor(&descriptor);                                             // Free the descriptor

    return out;
}

MatrixXcd IFFT2D(const MatrixXcd &in) {
    return FFT2D(in) / in.size();
}

MatrixXcd FFTCol(const MatrixXcd &in) {
    MatrixXcd out(in.rows(), in.cols());
    for (Index i = 0; i < in.cols(); ++i) {
        out.col(i) = FFT(in.col(i));
    }
    return out;
}

MatrixXcd IFFTCol(const MatrixXcd &in) {
    MatrixXcd out(in.rows(), in.cols());
    for (Index i = 0; i < in.cols(); ++i) {
        out.col(i) = IFFT(in.col(i));
    }
    return out;
}