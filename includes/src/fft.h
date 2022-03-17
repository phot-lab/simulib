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
 * Created: 2022/3/9
 * Supported by: National Key Research and Development Program of China
 */
#ifndef SIMULIB_FFT_H
#define SIMULIB_FFT_H

#include "Eigen/Core"

using namespace Eigen;

// The implementation of FFTShift referred to this website
// https://kerpanic.wordpress.com/2016/04/08/more-efficient-ifftshift-fftshift-in-c/
template<typename T>
Matrix<T, Dynamic, 1> FFTShift(const Matrix<T, Dynamic, 1> &in) {
    Index size = in.rows();
    Matrix<T, Dynamic, 1> out(size, 1);

    unsigned pivot = (size % 2 == 0) ? (size / 2) : ((size + 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf = pivot;

    memcpy(out.data(), in.data() + pivot, sizeof(double) * rightHalf);
    memcpy(out.data() + rightHalf, in.data(), sizeof(double) * leftHalf);
    return out;
}

template<typename T>
Matrix<T, Dynamic, 1> IFFTShift(const Matrix<T, Dynamic, 1> &in) {
    Index size = in.rows();
    Matrix<T, Dynamic, 1> out(size, 1);

    unsigned pivot = (size % 2 == 0) ? (size / 2) : ((size - 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf = pivot;

    memcpy(out.data(), in.data() + pivot, sizeof(double) * rightHalf);
    memcpy(out.data() + rightHalf, in.data(), sizeof(double) * leftHalf);
    return out;
}

VectorXcd FFT(const VectorXcd &in);

VectorXcd IFFT(const VectorXcd &in);

MatrixXcd FFT2D(const MatrixXcd &in);

MatrixXcd IFFT2D(const MatrixXcd &in);

MatrixXcd FFTCol(const MatrixXcd &in);

MatrixXcd IFFTCol(const MatrixXcd &in);

#endif  // SIMULIB_FFT_H
