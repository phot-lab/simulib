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
 * Created: 2022/3/22
 * Supported by: National Key Research and Development Program of China
 */

/**
 * Evaluate eye module implementation
 */

#include "Internal"

using namespace std;

namespace SimuLib {

namespace HARDWARE_TYPE {

static VectorXi sortMapIndex(VectorXd vec, VectorXd sorted);

/**
 * @brief Evaluate the eye-opening
 * @param patternBinary: genPattern sample.
 * @param signal: signal sample.
 * @param symbolRate: the symbol rate [Gbaud] of the signal.
 * @param modFormat: indicating the modulation format.(In DigitalModulator.cpp)
 * @param fiber: the fiber that transfer light.
 * @return eyeOpening:
 * @return iricMat:
 */

tuple<double, MatrixXcd> evaluateEye(MatrixXi pattern, const MatrixXcd &signal, double symbolRate, const string &modFormat, const Fiber &fiber) {

    double nt = gstate.SAMP_FREQ / symbolRate;  // Number of points per symbol
    if (!isInt(nt))
        ERROR("Number of points per symbol is not an integer.");
    double nSymb  = (double) gstate.NSAMP / nt;
    Index nPol    = signal.cols();
    double nShift = round(nt / 2);  // the first bit is centered at index 1

    // Only calculate the real component of signal
    MatrixXd iricMat = circShift(signal, (int) nShift).reshaped(nt, nSymb * (double) nPol).transpose().real();

    FormatInfo formatInfo = modFormatInfo(modFormat);

    MatrixXd botVec = MatrixXd ::Zero((Index) formatInfo.digit, (Index) nt);
    MatrixXd topVec = MatrixXd ::Zero((Index) formatInfo.digit, (Index) nt);

    for (int i = 0; i < formatInfo.digit; ++i) {
        VectorXi select    = pattern.array().cwiseEqual(i).reshaped().cast<int>();
        MatrixXd eyeSignal = selectRows(iricMat, select);
        topVec.row(i)      = eyeSignal.colwise().minCoeff();  // Top of eye
        botVec.row(i)      = eyeSignal.colwise().maxCoeff();  // Bottom of eye
    }

    VectorXd topVecRowMax = topVec.rowwise().maxCoeff();
    VectorXd botVecRowMin = botVec.rowwise().minCoeff();

    VectorXd topVecSorted = sortEigen(topVecRowMax);
    VectorXd botVecSorted = sortEigen(botVecRowMin);

    VectorXi indexTop = sortMapIndex(topVecRowMax, topVecSorted);  // Sort because of pattern
    VectorXi indexBot = sortMapIndex(botVecRowMin, botVecSorted);

    // Eye opening: best of the worst among symbols
    // VectorXcd eyeOpeningVec    = maxCol((MatrixXcd) (truncateMatrix(topVec, excludeFirst(indexTop)) - (MatrixXcd) truncateMatrix(botVec, excludeLast(indexBot))));  // Among samples
    VectorXd eyeOpeningVec(topVec.rows() - 1);
    for (int i = 0; i < eyeOpeningVec.rows(); i++)
        eyeOpeningVec(i) = (topVec.row(indexTop(i + 1)) - botVec.row(indexBot(i))).maxCoeff();

    //    complex<double> eyeOpening = eyeOpeningVec.redux([](const complex<double> &a, const complex<double> &b) {
    //        if (a.real() < b.real())
    //            return a;
    //        return b;
    //    });

    double eyeOpening = eyeOpeningVec.maxCoeff();

    // 单位转换为dBm，暂时不用这个
    //    if (eyeOpening < 0)
    //        eyeOpening = NAN;
    //    eyeOpening = 10 * log10(eyeOpening);

    return make_tuple(eyeOpening, iricMat);
}

static VectorXi sortMapIndex(VectorXd vec, VectorXd sorted) {
    map<double, int> indexMap;
    VectorXi index(sorted.size());
    for (int i = 0; i < vec.size(); i++)
        indexMap[vec(i)] = i;
    for (int i = 0; i < sorted.size(); i++)
        index(i) = indexMap[sorted(i)];
    return index;
}

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib