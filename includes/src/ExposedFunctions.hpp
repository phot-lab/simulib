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
 * Created: 2022/4/14
 * Supported by: National Key Research and Development Program of China
 */

/**
 * Exposed function declarations
 */

#ifndef EXPOSEDFUNCTIONS_HPP
#define EXPOSEDFUNCTIONS_HPP

#include "IQModulator.h"
#include "Mzmodulator.hpp"
#include "RxFrontend.h"

namespace SimuLib {

namespace HARDWARE_TYPE {

tuple<MatrixXcd, double> digitalModulator(const MatrixXi &patBinary, double symbolRate, Par par, const string &modFormat, string pulseType);

tuple<MatrixXcd, double> electricAmplifier(MatrixXcd signal, double gainEA, double powerW, double oneSidedSpectralDensity);

tuple<double, MatrixXcd> evaluateEye(MatrixXi pattern, const MatrixXcd &signal, double symbolRate, const string &modFormat, const Fiber &fiber);

tuple<Out, E> fiberTransmit(E &e, Fiber fiber);

E iqModulator(E e, VectorXcd modSig, IqOption option);

E laserSource(RowVectorXd ptx, const RowVectorXd &lam, LaserOption option);

E laserSource(RowVectorXd ptx, RowVectorXd lam, double spac = 0.0, int NLAMBDA = 0, LaserOption options = LaserOption());

E mzModulator(E light, VectorXcd modSig);
E mzmodulator(E light, VectorXcd modsig, MzOption mzOption);

tuple<VectorXi, MatrixXi> genPattern(unsigned nSymbol, const string &patternType);
tuple<VectorXi, MatrixXi> genPattern(unsigned nSymbol, const string &patternType, string array[]);

E pbc(E ex, E ey);

std::tuple<E, E> pbs(E e);

MatrixXcd rxFrontend(E e, RowVectorXd lambda, int symbrate, const RxOption &rxOption);

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib

#endif  // EXPOSEDFUNCTIONS_HPP
