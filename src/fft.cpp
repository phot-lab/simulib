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

#include <iostream>
#include <mkl.h>
#include <simulib>
#include <vector>

using namespace Eigen;
using namespace std;

// The implementation of FFTShift referred to this website
// https://kerpanic.wordpress.com/2016/04/08/more-efficient-ifftshift-fftshift-in-c/
VectorXd FFTShift(const VectorXd &in) {
    Index size = in.size();
    VectorXd out(size);

    unsigned pivot     = (size % 2 == 0) ? (size / 2) : ((size + 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf  = pivot;

    memcpy(out.data(), in.data() + pivot, sizeof(double) * rightHalf);
    memcpy(out.data() + rightHalf, in.data(), sizeof(double) * leftHalf);
    return out;
}

VectorXd IFFTShift(const VectorXd &in) {
    Index size = in.size();
    VectorXd out(size);

    unsigned pivot     = (size % 2 == 0) ? (size / 2) : ((size - 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf  = pivot;

    memcpy(out.data(), in.data() + pivot, sizeof(double) * rightHalf);
    memcpy(out.data() + rightHalf, in.data(), sizeof(double) * leftHalf);
    return out;
}

// The implementation of FFT referred to this website
// https://stackoverflow.com/questions/29805767/is-there-any-simple-c-example-on-how-to-use-intel-mkl-fft
VectorXcd FFT(const VectorXcd &in) {
    VectorXcd out(in.size());
    DFTI_DESCRIPTOR_HANDLE descriptor;
    MKL_LONG status;
    (void) status; // Suppress the warning about unused variables

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
    (void) status; // Suppress the warning about unused variables

    status = DftiCreateDescriptor(&descriptor, DFTI_DOUBLE, DFTI_COMPLEX, 1, in.size());  // Specify size and precision
    status = DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_NOT_INPLACE);                  // Out of place FFT
    status = DftiSetValue(descriptor, DFTI_BACKWARD_SCALE, 1.0 / (double) in.size());     // Scale down the output
    status = DftiCommitDescriptor(descriptor);                                            // Finalize the descriptor
    status = DftiComputeBackward(descriptor, (void *) in.data(), out.data());             // Compute the Forward FFT
    status = DftiFreeDescriptor(&descriptor);                                             // Free the descriptor

    return out;
}
