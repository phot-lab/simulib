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
 * Created: 2022/4/11
 * Supported by: National Key Research and Development Program of China
 */

#include <Internal>

namespace SimuLib {

namespace PARALLEL_TYPE {

static E awgn(E e, double reqSNR);

/**
 * Electrc Amplifier
 * @param e
 * @param gainEA: 增益倍数 (dB)
 * @param powerW: 输入信号功率
 * @param oneSidedSpectralDensity: 单边功率谱密度 A/Hz^(1/2)
 * @return
 */

tuple<E, double> electricAmplifier(E e, double gainEA, double powerW, double oneSidedSpectralDensity) {
    double powerWAfterEA = powerW * pow(10, gainEA / 10);
    e.field              = e.field * sqrt(powerWAfterEA);
    double snrGS         = 10 * log10(powerW / (oneSidedSpectralDensity / 2));
    e                    = awgn(e, snrGS);
    double gain          = gainEA;
    return make_tuple(e, gain);
}

/**
 * Additive White Gaussian Noise - "measure" type
 * @param e
 * @param reqSNR
 * @return
 */
static E awgn(E e, double reqSNR) {
    double signalPower = matrixToVec(e.field).cwiseAbs2().sum() / e.field.size();  // Linear

    // Convert signal power and SNR to linear scale
    reqSNR = pow(10, reqSNR / 10);

    double noisePower = signalPower / reqSNR;
    Index rows        = e.field.rows();
    Index cols        = e.field.cols();
    MatrixXcd noise(rows, cols);

    if (e.field.imag().isZero()) {
        noise = sqrt(noisePower) * normalRng(rows, cols);
    } else {
        MatrixXcd real = normalRng(rows, cols);
        complex<double> imagUnit(0, 1);
        MatrixXcd imag = (MatrixXcd) normalRng(rows, cols) * imagUnit;
        noise          = sqrt(noisePower / 2) * (real + imag);
    }
    e.field = e.field + noise;
    return e;
}

}  // namespace PARALLEL_TYPE

}  // namespace SimuLib