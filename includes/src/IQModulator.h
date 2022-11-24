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
 * Created: 2022/4/8
 * Supported by: National Key Research and Development Program of China
 */

#ifndef SIMULIB_IQMODULATOR_H
#define SIMULIB_IQMODULATOR_H

namespace SimuLib {

/**
 * @brief in-phase/quadrature optical modulator simulation option
 * @param iqratio: power ratio [dB] between I and Q arms (default = 0)
 * @param biasc : normalized bias between I and Q arms. The bias is normalized to the voltage
 *               yielding a phase shift of pi in both arms, usually called Vpi. (default = 0)
 * @param exratio: extinction ratio of the two nested modulators [dB]. (default = [inf inf])
 * @param biasl: bias of the lower arm of each Mach-Zehnder modulator only.(default = [-1 -1]).
 * @param biasu: bias of the upper arm of each Mach-Zehnder modulator only.(default = [-1 -1]).
 * @param vpi: voltage yielding a phase shift of pi in each MZ branch.
 * @param nch: channel (wavelength) index. This option must be used when
 *             E.field is a matrix and represents the column on which the
 *             function operates. In dual-polarization the function operates
 *             on columns [nch (nch+1)] representing x and y polarizations.
 * @param norm: normalization factor
 */

struct IqOption {
    double iqratio    = 0;
    double biasc      = 0;
    double exratio[2] = {INT_MAX, INT_MAX};
    double biasl[2]   = {-1, -1};
    double biasu[2]   = {-1, -1};
    double vpi        = INT_MAX;
    int nch           = 1;
    double norm       = INT_MAX;
};

}  // namespace SimuLib

#endif  // SIMULIB_IQMODULATOR_H
