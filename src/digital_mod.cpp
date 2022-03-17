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

#include <simulib>

using namespace std;

VectorXd PulseDesign(string ptype, int nsps, unsigned long n_symb, Par par);

MatrixXcd ElecSrc(MatrixXcd level, string ptype, Par par, unsigned long n_symb, int nsps, int nd, unsigned long n_fft);

tuple<MatrixXcd, double> DigitalMod(MatrixXi pat_bin, double symbrate, Par par, string mod_format, string ptype) {
    unsigned long n_fft = gstate.NSAMP;
    double n_tini = gstate.SAMP_FREQ / symbrate;  // Wished samples per symbol
    Index n_symb = max(pat_bin.rows(), pat_bin.cols());
    double n_symbupdw = ceil((n_fft - 1) / n_tini);
    if (n_symb < n_symbupdw) {
        ERROR("Too few symbols to fill the required number of samples.");
    } else if (n_symb > n_symbupdw) {
        n_symb = n_symbupdw;
        WARNING("Too many symbols. Pattern truncated to %d symbols.");
    }
    int nt, nd;
    tie(nt, nd) = continued_fraction_approximation(n_tini);  // oversample, then downsample at the end
    int nsps;
    if (par.nsps != 0) {
        nsps = par.nsps;  // the function works with Nsps samples per symbol till the final resampling emulating the DAC
    } else {
        nsps = nt;
    }

    if ((nt / n_tini) > 10) {
        WARNING("resampling may take big times/memory. Consider using sampling");
    }
    par.mod_format = mod_format;

    // 1: convert the pattern into stars of the constellations
    MatrixXcd level = Pat2Samp(pat_bin, mod_format).cast<complex<double>>();

    // 2: create a linearly modulated digital signal
    MatrixXcd elec = ElecSrc(level, ptype, par, n_symb, nsps, nd, n_fft);

    // 3: resample if necessary
    if (nt != nsps) {
        // 未完成
    }

    // 4: Perform pre-emphasis
    double normf;
    if (par.emph != "") {
        normf = max(elec.real().array().abs().maxCoeff(), elec.imag().array().abs().maxCoeff());
        if (par.emph == "asin") {
            // thanks to normf, the result of each asin is for sure real, as
            // required by a M.ach-Zehnder modulator. However, to preserve the
            // energy, the Mach-Zehnder must know such a normalization factor.
            VectorXcd real = (elec.real() / normf).array().asin().conjugate();
            VectorXcd imag = (elec.imag() / normf).array().asin().conjugate() * 1i;
        }
    } else {
        normf = 1;
    }

    return make_tuple(elec, normf);
}

MatrixXcd ElecSrc(MatrixXcd level, string ptype, Par par, unsigned long n_symb, int nsps, int nd, unsigned long n_fft) {
    // The idea is the following: the pattern is first upsampled to par.nsps samples per symbol, and then filtered to create the PAM signal.

    bool flag;
    if (ptype != "rc" && ptype != "rootrc" && ptype != "userfir" && ptype != "dirac" && ptype != "rect" &&
        ptype != "costails") {
        flag = true; // the pulse is the impulse response of myfilter
    } else {
        flag = false;
    }
    /*********** Modulate ***********/
    if (ptype == "userfir") {
        // 未完成
    }

    MatrixXcd levelu = UpSample(level, nsps);
    levelu.conservativeResize(nsps, n_symb); // truncate if necessary

    MatrixXcd levelu_fft = FFTCol(levelu);

    VectorXcd hfir;
    if (flag) {
        // 未完成
    } else {
        VectorXcd elpulse = PulseDesign(ptype, nsps, n_symb, par); // single pulse
        hfir = FFT(FFTShift(elpulse));
        if (ptype == "rootrc") { // square-root raised cosine
            hfir = (hfir *
                    nsps).cwiseSqrt(); // Note: because I'm using filters normalized in peak spectrum (as if symbol time was 1)
        }
    }

    MatrixXcd elec = IFFTCol(levelu_fft.cwiseProduct(hfir)); // create PAM signal

    Index length = max(elec.rows(), elec.cols());
    if (length < n_fft) {
        ERROR("It is impossible to get the desired number of samples with the given pattern and sampling rate");
    } else if (length > n_fft) {
        VectorXcd temp = MatrixToVector(elec);
        VectorXd ceil = GenStepVector(1, nd, n_fft * nd).array().ceil();
        temp = TruncateVector(temp, ceil);
        elec = temp;
    }

    FormatInfo format_info{};
    double avge;
    // normalize to unit power
    if (par.norm == "iid") {
        // power spectra of linearly modulated signals
        format_info = ModFormatInfo(par.mod_format);
        double varak = format_info.symb_var; // expected variance
        double meanak = format_info.symb_mean; // expected value or mean
        avge = (varak * hfir.cwiseAbs2().sum() / n_symb + pow(abs(meanak), 2) * TruncateVector(hfir,
                                                                                               GenStepVector(1,
                                                                                                             n_symb,
                                                                                                             hfir.size() -
                                                                                                             1)).cwiseAbs2().sum()) /
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

enum Option {
    Costails,
    Userfir
};

Option ResolveOption(string ptype) {
    if (ptype == "costails") return Costails;
    if (ptype == "userfir") return Userfir;
    return Costails;
}

VectorXd PulseDesign(string ptype, int nsps, unsigned long n_symb, Par par) {
    VectorXd elpulse = VectorXd::Zero(nsps, n_symb);
    Option option = ResolveOption(ptype);
    switch (option) {
        case Costails: {
            double nl = round(0.5 * (1 - par.rolloff) * par.duty * nsps); // start index of cos roll-off
            double nr = par.duty * nsps - nl - 1; // end index of cos roll-off
            RowVectorXd nmark = GenVector(1, nl); // indices where the pulse is 1
            RowVectorXd ncos = GenVector(nl, nr); // transition region of cos roll-off
            SetValueIndices(elpulse, nsps * n_symb / 2 + nmark.array(), 1);
            double hperiod = par.duty * nsps - 2 * nl;
            if (hperiod != 0) {
                VectorXd indices = ncos.array() + nsps * n_symb / 2 + 1;
                VectorXd replace = (((ncos.array() - nl + 0.5) * M_PI / hperiod).cos() + 1) * 0.5;
                ReplaceVector(elpulse, indices, replace);
            }
            VectorXd replace = TruncateVector(elpulse, GenVector(nsps * n_symb / 2 + 1, nsps * n_symb)).reverse();
            VectorXd indices = GenVector(1, nsps * n_symb / 2);
            ReplaceVector(elpulse, indices, replace); // first half of the pulse
            break;
        }
        default:
            ERROR("The pulse ptype does not exist");
            break;
    }
    return elpulse;
}
