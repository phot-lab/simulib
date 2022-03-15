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

void DigitalMod(MatrixXi pat_bin, double symbrate, Par par, string mod_format, string ptype) {
    unsigned long n_fft = gstate.NSAMP;
    double n_tini       = gstate.SAMP_FREQ / symbrate;  // Wished samples per symbol
    Index n_symb        = max(pat_bin.rows(), pat_bin.cols());
    double n_symbupdw   = ceil((n_fft - 1) / n_tini);
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
    MatrixXi level = Pat2Samp(pat_bin, mod_format);
}

void ElecSrc(MatrixXi level, string ptype, Par par, unsigned long n_symb, int nsps, int nd, unsigned long n_fft) {
    // The idea is the following: the pattern is first upsampled to par.nsps samples per symbol, and then filtered to create the PAM signal.

    // flag未完成
    /*********** Modulate ***********/
    if (ptype == "userfir") {
        // 未完成
    }


}
