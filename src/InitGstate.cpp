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
 * Created: 2022/3/1
 * Supported by: National Key Research and Development Program of China
 */

#include "simulib"

using namespace std;

void initGstate(double Nsamp, double Fs) {
    if (!IsInt(Nsamp))
        ERROR("The number of samples must be an integer");
    gstate.NSAMP     = (unsigned long) Nsamp;  // Number of samples
    double stepf     = Fs / Nsamp;             // Minimum frequency [GHz]
    VectorXd vec     = GenStepVector(-Fs / 2, stepf, Fs / 2 - stepf);
    gstate.FN        = FFTShift(vec);  // Frequencies [GHz]
    gstate.SAMP_FREQ = Fs;             // Sampling frequency [GHz]
}