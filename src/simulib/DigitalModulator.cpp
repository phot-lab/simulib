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
 * Created: 2022/3/14
 * Supported by: National Key Research and Development Program of China
 */

#include "Internal"

using namespace std;

namespace SimuLib {

namespace HARDWARE_TYPE {

static VectorXd pulseDesign(string ptype, int nsps, unsigned long nSymbol, Par par);

static MatrixXcd elecSrc(MatrixXcd level, const string &pulseType, Par par, unsigned long nSymbol, int nsps, int nd, unsigned long n_fft);

/**
 * @brief linearly modulated digital signal
 * @param patBinary: a matrix containing the genPattern. PAT can be a matrix of bits, of size number of
 *        bits-by-log2(alphabet size), or a column vector containing the decimal representation.
 * @param symbolRate: the signal symbol-rate in [Gbaud].
 * @param par: a struct with the following fields:
 *        PAR.rolloff = roll-off (0<=PAR.rolloff<=1) of raised cosine and root-raised cosine pulses.
 *        PAR.duty = pulse duty cycle (0 < PAR.duty <= 1). The time axis is scaled by PAR.duty. Default: PAR.duty = 1.
 *        PAR.norm. power normalization.
 *            'iid': the output signal is normalized to unit average power under the assumption that the symbols are
 *            independent and identically distributed symbols with uniform distribution.[Default].
 *            'mean': the power is normalized to the average power of the
 *            current signal.
 *            'no': no normalization is applied.
 *        PAR.nsps = number of samples per symbol.This option allows using a different sampling rate for internal
 *           operations before the final setting to match the global GSTATE.FSAMPLING. The transmitted sequence is
 *           upsampled to PAR.Nsps samples, and then filtered with a FIR filter with impulse response equal to the
 *           desired pulse shape. Default value: GSTATE.FSAMPLING/SYMBRATE. If not an integer, a rational approximation
 *           with RAT is used.
 *        PAR.firtaps = taps of the FIR filter creating the pulse. The corresponding sampling rate is set by PAR.nsps.
 *           Default: PAR.firtaps = length(PAT). This field is used only with PTYPE='userfir'.
 *        PAR.emph = emphasis type. After creation, the signal may experience an emphasis.
 *        PAR.bw = bandwidth, normalized to the symbol rate SYMBRATE, of the filter used by filter when PTYPE is a
 *           valid string supported by filter (in RxFrontedn.cpp).
 *        PAR.par = optional parameters of MYFILTER when PTYPE is a valid string supported by filter (in RxFrontedn.cpp).
 * @param modFormat: a string with options below:
 *        'ook': on-off keying [Mach Zehnder modulator]
 *        'bpsk': binary phase-shift keying (PSK) [Mach Zehnder modulator]
 *        'qpsk': quadrature phase-shift keying (QPSK) [iq modulator]
 *        'psk': PSK [iq modulator + (possibly) phase modulator(s)]
 *        '[num]qam': quadrature amplitude modulation. [num] is the alphabet size[iq modulator]
 *        '[num]pam': M-ary (power of two) pulse amplitude modulation. [num] is the alphabet size. [Mach Zehnder modulator]
 * @param pulseType: the pulse type:
 *        'rc': raised cosine pulse.
 *        'rootrc': root raised-cosine pulse.
 *        'userfir': the shape of the pulse is given by the user as a column vector.
 *        'costails': a non-square pulse with cosine-shaped tails.
 *        'dirac': the pulse is a Dirac delta.
 *        'rect': rectangular pulse.
 * @return E: a struct of wave, details in Fiber.hpp
 * @return norm: Normalization factor
 */

tuple<MatrixXcd, double> digitalModulator(const MatrixXi &patBinary, double symbolRate, Par par, const string &modFormat, string pulseType) {
    unsigned long n_fft = gstate.NSAMP;
    double nTini        = gstate.SAMP_FREQ / symbolRate;  // Wished samples per symbol
    double nSymbol      = (double) max(patBinary.rows(), patBinary.cols());
    double nSymbupdw    = ceil((double) (n_fft - 1) / nTini);
    if (nSymbol < nSymbupdw) {
        ERROR("Too few symbols to fill the required number of samples.");
    } else if (nSymbol > nSymbupdw) {
        nSymbol = nSymbupdw;
        WARNING("Too many symbols. genPattern truncated to %d symbols.");
    }
    int nt, nd;
    tie(nt, nd) = continuedFractionApproximation(nTini);  // oversample, then downsample at the end
    int nsps;
    if (par.nsps != 0) {
        nsps = par.nsps;  // the function works with Nsps samples per symbol till the final resampling emulating the DAC
    } else {
        nsps = nt;
    }

    if ((nt / nTini) > 10) {
        WARNING("resampling may take big times/memory. Consider using sampling");
    }
    par.modFormat = modFormat;

    // 1: convert the pattern into stars of the constellations
    MatrixXcd level = Pat2Samp(patBinary, modFormat).cast<complex<double>>();

    // 2: create a linearly modulated digital signal
    MatrixXcd signal = elecSrc(level, pulseType, par, nSymbol, nsps, nd, n_fft);

    // 3: resample if necessary
    if (nt != nsps) {
        // 未完成
    }

    // 4: Perform pre-emphasis
    double norm;
    if (!par.emph.empty()) {
        norm = max(signal.real().array().abs().maxCoeff(), signal.imag().array().abs().maxCoeff());
        if (par.emph == "asin") {
            // thanks to norm, the result of each asin is for sure real, as
            // required by a M.ach-Zehnder modulator. However, to preserve the
            // energy, the Mach-Zehnder must know such a normalization factor.
            VectorXcd real = (signal.real() / norm).array().asin().conjugate();
            VectorXcd imag = (signal.imag() / norm).array().asin().conjugate();
            complex<double> imagUnit(0, 1);
            imag   = imag * imagUnit;
            signal = real + imag;
        }
    } else {
        norm = 1;
    }

    return make_tuple(signal, norm);
}

static MatrixXcd elecSrc(MatrixXcd level, const string &pulseType, Par par, unsigned long nSymbol, int nsps, int nd, unsigned long n_fft) {
    // The idea is the following: the pattern is first upsampled to par.nsps samples per symbol, and then filtered to create the PAM signal.

    bool flag;
    if (pulseType != "rc" && pulseType != "rootrc" && pulseType != "userfir" && pulseType != "dirac" && pulseType != "rect" &&
        pulseType != "costails") {
        flag = true;  // the pulse is the impulse response of myfilter
    } else {
        flag = false;
    }
    /*********** Modulate ***********/
    if (pulseType == "userfir") {
        // 未完成
    }

    MatrixXcd levelu = upSample(level, nsps);

    //    levelu.conservativeResize(1, n_symb * nsps);  // truncate if necessary
    VectorXcd temp = matrixToVec(levelu);
    levelu         = truncateVec(temp, genVector(1, nSymbol * nsps));  // truncate if necessary
                                                                       //    cout << "levelu:" << levelu << endl;
    MatrixXcd levelu_fft = fftCol(levelu);

    VectorXcd hfir;
    if (flag) {
        // 未完成
    } else {
        VectorXd elpulse = pulseDesign(pulseType, nsps, nSymbol, par);  // single pulse
                                                                        //        cout << "elpulse:\n" << elpulse << endl;
        hfir = fft(fftShift(elpulse));
        //        cout << "hfir:\n" << hfir << endl;

        if (pulseType == "rootrc") {  // square-root raised cosine
            hfir = (hfir *
                    nsps)
                           .cwiseSqrt();  // Note: because I'm using filters normalized in peak spectrum (as if symbol time was 1)
        }
    }

    for (Index i = 0; i < levelu_fft.cols(); ++i) {
        levelu_fft.col(i) = levelu_fft.col(i).cwiseProduct(hfir);
    }

    MatrixXcd elec = ifftCol(levelu_fft);  // create PAM signal

    Index length = max(elec.rows(), elec.cols());
    if (length < (long) n_fft) {
        ERROR("It is impossible to get the desired number of samples with the given genPattern and sampling rate");
    } else if (length > (long) n_fft) {
        VectorXcd temp = matrixToVec(elec);
        VectorXd ceil  = genStepVector(1, nd, n_fft * nd).array().ceil();
        temp           = truncateVec(temp, ceil);
        elec           = temp;
    }

    FormatInfo format_info{};
    double avge;
    // normalize to unit power
    if (par.norm == "iid") {
        // power spectra of linearly modulated signals
        format_info   = modFormatInfo(par.modFormat);
        double varak  = format_info.symb_var;   // expected variance
        double meanak = format_info.symb_mean;  // expected value or mean
        avge          = (varak * hfir.cwiseAbs2().sum() / nSymbol + pow(abs(meanak), 2) * truncateVec(hfir,
                                                                                                      genStepVector(1,
                                                                                                                    nSymbol,
                                                                                                                    hfir.size() -
                                                                                                                            1))
                                                                                         .cwiseAbs2()
                                                                                         .sum()) /
               pow(nsps, 2);
    } else if (par.norm == "mean") {
        avge = elec.cwiseAbs2().mean();
    } else if (par.norm == "no") {
        avge = 1;
    } else {
        ERROR("Unknwon normalization method");
    }

    return elec / sqrt(avge);
}

namespace {

enum PTypeOption {
    Costails,
    Userfir,
    rc,
    rootrc
};

}

PTypeOption ResolveOption(const string &pulseType) {
    if (pulseType == "costails") return Costails;
    if (pulseType == "userfir") return Userfir;
    if (pulseType == "rc") return rc;
    if (pulseType == "rootrc") return rootrc;
    return Costails;
}

static VectorXd pulseDesign(string ptype, int nsps, unsigned long nSymbol, Par par) {
    VectorXd elpulse   = VectorXd::Zero(nsps * nSymbol, 1);
    PTypeOption option = ResolveOption(ptype);
    switch (option) {
        case Costails: {
            double nl         = round(0.5 * (1 - par.rolloff) * par.duty * nsps);  // start index of cos roll-off
            double nr         = par.duty * nsps - nl - 1;                          // end index of cos roll-off
            RowVectorXd nmark = genVector(1, nl);                                  // indices where the pulse is 1
            RowVectorXd ncos  = genVector(nl, nr);                                 // transition region of cos roll-off
            setValueIndices(elpulse, nsps * nSymbol / 2 + nmark.array(), 1);
            double hperiod = par.duty * nsps - 2 * nl;
            if (hperiod != 0) {
                VectorXd indices = ncos.array() + nsps * nSymbol / 2 + 1;
                VectorXd replace = (((ncos.array() - nl + 0.5) * M_PI / hperiod).cos() + 1) * 0.5;
                replaceVector(elpulse, indices, replace);
            }
            VectorXd replace = truncateVec(elpulse, genVector(nsps * nSymbol / (double) 2 + 1, nsps * nSymbol)).reverse();
            VectorXd indices = genVector(1, nsps * nSymbol / (double) 2);
            replaceVector(elpulse, indices, replace);  // first half of the pulse
            break;
        }
        case rc:
        case rootrc: {
            VectorXd tfir      = VectorXd(nsps * nSymbol);
            tfir               = ArrayXd(nsps * nSymbol).setLinSpaced(-((double) nSymbol / (double) 2), (double) nSymbol / (double) 2 - 1 / (double) nsps);
            tfir               = (1 / par.duty) * tfir;
            VectorXd sinc_tfit = tfir;
            sinc_tfit          = sinc_tfit.unaryExpr([](double x) {
                if (abs(x) < 0.00000000001)
                    return (double) 1.0;
                else
                    return sin(M_PI * x) / (M_PI * x);
            });

            VectorXd cos_tfir    = PI * par.rolloff * tfir;
            cos_tfir             = cos_tfir.array().cos();
            VectorXd unit        = VectorXd(tfir.size()).setOnes();
            VectorXd devide_tfir = 2 * par.rolloff * tfir;
            devide_tfir          = devide_tfir.array().pow(2);
            devide_tfir          = unit - devide_tfir;
            std::vector<int> index;

            for (int i = 0; i < devide_tfir.size(); i++) {
                if (devide_tfir(i) == 0) {
                    index.insert(index.end(), i);
                }
            }

            elpulse = sinc_tfit.cwiseProduct(cos_tfir);
            elpulse = elpulse.cwiseProduct(devide_tfir.cwiseInverse());
            for (int i = 0; i < (int) index.size(); i++)
                elpulse(index[i]) = par.rolloff / 2 * sin(M_PI / 2 / par.rolloff);

            break;
        }
        default:
            ERROR("The pulse ptype does not exist");
            break;
    }
    return elpulse;
}

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib