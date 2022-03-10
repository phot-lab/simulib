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

VectorXd FFTShift(const VectorXd &in);
VectorXd IFFTShift(const VectorXd &in);
VectorXcd FFT(const VectorXcd &in);
VectorXcd IFFT(const VectorXcd &in);

#endif  // SIMULIB_FFT_H
