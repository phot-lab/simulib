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

#include <SimuLib>

using namespace std;
using namespace Eigen;

complex<double> evaluateEye(MatrixXi patternBinary, MatrixXcd signal, double symbrate, string modFormat, Fiber fiber) {
    double nt = gstate.SAMP_FREQ / symbrate;  // Number of points per symbol
    if (!isInt(nt))
        ERROR("Number of points per symbol is not an integer.");
    double nSymb          = (double) gstate.NSAMP / nt;
    Index nPol            = signal.cols();
    double nShift         = round(nt / 2);  // the first bit is centered at index 1
    MatrixXcd iricMat     = circShift(signal, (int) nShift).reshaped(nt, nSymb * (double) nPol);
    FormatInfo formatInfo = modFormatInfo(modFormat);
    MatrixXcd botVec      = MatrixXcd ::Zero((Index) formatInfo.digit, (Index) nt);
    MatrixXcd topVec      = MatrixXcd ::Zero((Index) formatInfo.digit, (Index) nt);

    for (int i = 0; i < formatInfo.digit; ++i) {
        VectorXi select     = patternBinary.array().cwiseEqual(i - 1).reshaped().cast<int>();
        MatrixXcd eyeSignal = selectRows(iricMat, select);
        topVec.row(i)       = minRow(eyeSignal);  // Top of eye
        botVec.row(i)       = maxRow(eyeSignal);  // Bottom of eye
    }

    VectorXcd topVecSorted = sortEigen(maxCol(topVec));
    VectorXcd botVecSorted = sortEigen(maxCol(botVec));
    VectorXi indexTop      = indexOfSorted(maxCol(topVec), topVecSorted);  // Sort because of pattern
    VectorXi indexBot      = indexOfSorted(maxCol(botVec), botVecSorted);

    // Eye opening: best of the worst among symbols

    VectorXcd eyeOpeningVec    = maxCol((MatrixXcd) (selectRows(topVec, excludeFirst(indexTop)) - selectRows(botVec, excludeLast(indexBot))));  // Among samples
    complex<double> eyeOpening = eyeOpeningVec.redux([](const complex<double> &a, const complex<double> &b) {
        if (a.real() < b.real())
            return a;
        return b;
    });
    if (eyeOpening.real() < 0)
        eyeOpening = NAN;

    eyeOpening = complexProduct(10, log10(eyeOpening));  // // [dBm]
    return eyeOpening;
}