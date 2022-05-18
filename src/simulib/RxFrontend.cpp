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

#include "Internal"

using namespace std;

namespace SimuLib {

namespace HARDWARE_TYPE {

VectorXcd rxFilter(string filterType, const VectorXd &freq, double bandwidth, double p);
E filterEnv(E e, RowVectorXd lambda, const VectorXcd &hf);
MatrixXcd opti2Elec(const E& e, double nt, const RxOption& rxOption);

/**
 * @brief receiver front-end
 * @param e: electric field, is a struct of fields.
 *        E.lambda: central wavelength [nm] of the electric field, i.e.,
 *          wavelength that relates the lowpass equivalent signal to the
 *          corresponding bandpass signal.
 *        E.field: time samples of the electric field, with polarizations (if
 *          existing) alternated on columns
 * @param lambda: the central wavelength of the optical bandpass filter
 * @param symbrate: the symbol rate [Gbaud] of the signal.
 * @param rxOption: the connected rxOption.
 * @return electric wave.
 */

/**
 * Frontend receiver module
 */

MatrixXcd rxFrontend(E e, RowVectorXd lambda, int symbrate, const RxOption &rxOption) {

    // Create linear optical filters: OBPF (+rxOption)
    VectorXd fNorm = gstate.FN / symbrate;
    VectorXcd hf;
    if (rxOption.dcum != INT_MIN) {
        // Hf = postrxOption(x.dcum,x.slopecum,x.lambda,lam,E.lambda);
        // Hf = Hf .* myfilter(x.oftype,Fnorm,0.5*x.obw,x.filterParameter);
        // 未完成
    } else {
        hf = rxFilter(rxOption.ofType, fNorm, 0.5 * rxOption.obw, rxOption.filterParameter);
    }

    // 1: apply optical filter
    e = filterEnv(e, lambda, hf);

    // 2: optical to electrical conversion
    double nt      = gstate.SAMP_FREQ / symbrate;  // number of points per symbol
    MatrixXcd iric = opti2Elec(e, nt, rxOption);
    return iric;
}

MatrixXcd opti2Elec(const E& e, double nt, const RxOption& rxOption) {
    Index nFFT = e.field.size();
    MatrixXcd iric;
    if (rxOption.modFormat == "ook") {
        iric = sumRow((MatrixXd) e.field.cwiseAbs2());  // PD. sum is over polarizations
    } else if (rxOption.modFormat == "dpsk") {
        VectorXd nDel  = nModulusEigen(genVector(1, nFFT).array() - round(rxOption.mzDelay * nt), nFFT);  // interferometer delay
        VectorXcd temp = matrixToVec(e.field);
        temp           = truncateVec(temp, nDel).conjugate();
        iric           = sumRow((MatrixXd) e.field.cwiseProduct(temp).real());  // MZI + PD
    } else if (rxOption.modFormat == "dqpsk") {
        VectorXd nDel     = nModulusEigen(genVector(1, nFFT).array() - round(rxOption.mzDelay * nt), nFFT);  // interferometer delay
        VectorXcd temp    = matrixToVec(e.field);
        temp              = truncateVec(temp, nDel).conjugate();
        MatrixXcd sumTemp = fastExp(-M_PI / 4) * e.field.cwiseProduct(temp).real();
        complex<double> imagUnit(0, 1);
        iric = sumRow(sumTemp) + (MatrixXcd) (imagUnit * sumRow(sumTemp));
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

E filterEnv(E e, RowVectorXd lambda, const VectorXcd &hf) {
    double freqc   = e.lambda(0, 0) * LIGHT_SPEED;      // central frequency [GHz] (corresponding to the zero frequency of the lowpass equivalent signal by convention)
    double frec    = lambda[0] * LIGHT_SPEED;           // carrier frequency [GHz]
    double deltaFN = freqc - frec;                      // carrier frequency spacing [GHz]
    double minFreq = gstate.FN(1) - gstate.FN(0);       // Resolution [GHz]
    int ndfn       = (int) round((deltaFN / minFreq));  // Spacing in points
    e.field        = fftCol(e.field);
    e.field        = circShift(e.field, ndfn);  // Undo what did in MULTIPLEXER
    e.field        = ifftCol(matVecProduct(e.field, hf));
    return e;
}

VectorXcd rxFilter(string filterType, const VectorXd &freq, double bandwidth, double p) {

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

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib