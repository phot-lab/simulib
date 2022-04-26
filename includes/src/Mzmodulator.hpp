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
 * Author: Shihao Xie
 * Created: 2022/3/17
 * Supported by: National Key Research and Development Program of China
 */

#ifndef SIMULIB_MZMODULATOR_H
#define SIMULIB_MZMODULATOR_H

#include "LaserSource.hpp"
#include <cfloat>
#include <climits>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <random>

namespace SimuLib {

/**
 * @brief Mach-Zehnder modulator simulation option
 * @param exratio: extinction ratio [dB]
 * @param bias: bias, normalized to Vpi.
 * @param biasl: bias of the lower arm only.
 * @param biau: bias of the upper arm only.
 * @param mode: operating mode. It can be 'push-push' where the same MODSIG
 *              is applied to both arms, or 'push-pull' where a phase shift of
 *              pi is induced in the lower arm
 * @param nch: channel (wavelength) index. This option must be used when
 *             E.field is a matrix and represents the column on which the
 *             function operates. In dual-polarization the function operates
 *             on columns [nch (nch+1)] representing x and y polarizations.
 * @param vpi: voltage yielding a phase shift of pi in each MZ branch.
 * @param norm: normalization factor
 */

struct MzOption {
    double exratio = INT_MAX;
    double bias    = INT_MAX;
    double biasl   = INT_MAX;
    double biasu   = INT_MAX;
    enum { push_push = 1,
           push_pull = 0 };
    int mode    = push_pull;
    int nch     = 1;
    double vpi  = INT_MAX;
    double norm = INT_MAX;
};

}  // namespace SimuLib

#endif  // SIMULIB_MZMODULATOR_H
