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

namespace SimuLib {

namespace CPU {

VectorXcd fft(const VectorXcd &in);

VectorXcd ifft(const VectorXcd &in);

MatrixXcd fft2D(const MatrixXcd &in);

MatrixXcd ifft2D(const MatrixXcd &in);

MatrixXcd fftCol(const MatrixXcd &in);

MatrixXcd ifftCol(const MatrixXcd &in);

}  // namespace CPU

namespace GPU {

VectorXcd fft(const VectorXcd &in);

VectorXcd ifft(const VectorXcd &in);

MatrixXcd fft2D(const MatrixXcd &in);

MatrixXcd ifft2D(const MatrixXcd &in);

MatrixXcd fftCol(const MatrixXcd &in);

MatrixXcd ifftCol(const MatrixXcd &in);

}  // namespace GPU

}  // namespace SimuLib

#endif  // SIMULIB_FFT_H
