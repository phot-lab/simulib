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
 * Created: 2022/3/21
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>

using namespace std;

VectorXcd myFilter(string filterType, const VectorXd &freq, double bandwidth, double p);
E filterEnv(E e, RowVectorXd lambda, VectorXcd hf);
MatrixXcd oToE(E e, double nt, Fiber fiber);

MatrixXcd rxFrontend(E e, RowVectorXd lambda, int symbrate, const Fiber& fiber) {

    // Create linear optical filters: OBPF (+fiber)
    VectorXd fNorm = gstate.FN / symbrate;
    VectorXcd hf;
    if (fiber.dcum != nullptr) {
        // Hf = postfiber(x.dcum,x.slopecum,x.lambda,lam,E.lambda);
        // Hf = Hf .* myfilter(x.oftype,Fnorm,0.5*x.obw,x.opar);
        // 未完成
    } else {
        hf = myFilter(fiber.ofType, fNorm, 0.5 * (*fiber.obw), fiber.opar);
    }

    // 1: apply optical filter
    e = filterEnv(e, lambda, hf);

    // 2: optical to electrical conversion
    double nt      = gstate.SAMP_FREQ / symbrate;  // number of points per symbol
    MatrixXcd iric = oToE(e, nt, fiber);
    return iric;
}

MatrixXcd oToE(E e, double nt, Fiber fiber) {
    Index nFFT = e.field.size();
    MatrixXcd iric;
    if (fiber.modFormat == "ook") {
        iric = sumRow((MatrixXd) e.field.cwiseAbs2());  // PD. sum is over polarizations
    } else if (fiber.modFormat == "dpsk") {
        VectorXd nDel  = nModulusEigen(genVector(1, nFFT).array() - round(fiber.mzdel * nt), nFFT);  // interferometer delay
        VectorXcd temp = matrixToVec(e.field);
        temp           = truncateVec(temp, nDel).conjugate();
        iric           = sumRow((MatrixXd) e.field.cwiseProduct(temp).real());  // MZI + PD
    } else if (fiber.modFormat == "dqpsk") {
        VectorXd nDel     = nModulusEigen(genVector(1, nFFT).array() - round(fiber.mzdel * nt), nFFT);  // interferometer delay
        VectorXcd temp    = matrixToVec(e.field);
        temp              = truncateVec(temp, nDel).conjugate();
        MatrixXcd sumTemp = fastExp(-M_PI / 4) * e.field.cwiseProduct(temp).real();
        iric              = sumRow(sumTemp) + 1i * sumRow(sumTemp);
    } else {
        iric = e.field;
    }
    return iric;
}

/**
 * FILTERENV filter around a given wavelength E = FILTERENV(E,LAM,HF)
 * first aligns the channel's carrier wavelength LAM to the central
 * wavelength of the optical filter of frequency response HF, then applies the filter.
 * @param e
 * @param lambda
 * @param hf
 * @return
 */
E filterEnv(E e, RowVectorXd lambda, VectorXcd hf) {
    double freqc   = e.lambda(0, 0) * LIGHT_SPEED;      // central frequency [GHz] (corresponding to the zero frequency of the lowpass equivalent signal by convention)
    double frec    = lambda[0] * LIGHT_SPEED;           // carrier frequency [GHz]
    double deltaFN = freqc - frec;                      // carrier frequency spacing [GHz]
    double minFreq = gstate.FN(1) - gstate.FN(0);       // Resolution [GHz]
    int ndfn       = (int) round((deltaFN / minFreq));  // Spacing in points
    e.field        = fftCol(e.field);
    e.field        = circShift(e.field, ndfn);  // Undo what did in MULTIPLEXER
    e.field        = ifftCol(mvProduct(e.field, std::move(hf)));
    return e;
}

VectorXcd myFilter(string filterType, const VectorXd &freq, double bandwidth, double p) {

    /*** CONSTANTS ***/
    const double r4p2r2 = 2.61312592975275;  // =sqrt(2*(2+sqrt(2))); used in Butterworth 4th order
    const double b1     = 3.86370330515627315;
    const double b2     = 7.4641016151377546;
    const double b3     = 9.1416201726856413;
    const double b4     = b2;
    const double b5     = b1;
    const double Bb     = 0.3863;
    const double d0     = 945;
    const double d1     = 945;
    const double d2     = 420;
    const double d3     = 105;
    const double d4     = 15;

    VectorXd x = freq / bandwidth;  // frequency normalized to the bandwidth
    filterType = toLower(filterType);

    VectorXcd hf;
    if (filterType == "gauss") {
        hf = (-0.5 * log(2) * (x.cwiseProduct(x))).array().exp();
    } else if (filterType == "") {
        // 未完成
    } else {
        ERROR("The filter ftype does not exist.");
    }
    return hf;
}