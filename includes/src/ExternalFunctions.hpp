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
#ifndef EXTERNALFUNCTIONS_HPP
#define EXTERNALFUNCTIONS_HPP

#include "IQModulator.h"
#include "Mzmodulator.hpp"

namespace SimuLib {

namespace PARALLEL_TYPE {

tuple<MatrixXcd, double> digitalModulator(const MatrixXi &patBinary, double symbrate, Par par, const string &modFormat, string ptype);

tuple<E, double> electricAmplifier(E e, double gainEA, double powerW, double oneSidedSpectralDensity);

tuple<complex<double>, MatrixXcd> evaluateEye(MatrixXi patternBinary, const MatrixXcd &signal, double symbrate, const string &modFormat, const Fiber &fiber);

tuple<Out, E> fiberTransmit(E &e, Fiber fiber);

E IQModulator(E e, VectorXcd modSig, IQOption option);

E laserSource(RowVectorXd ptx, const RowVectorXd &lam, Option options);

E laserSource(RowVectorXd ptx, RowVectorXd lam, double spac = 0.0, int NLAMBDA = 0, Option options = Option());

E mzmodulator(E light, VectorXcd modSig);
E mzmodulator(E light, VectorXcd modsig, Mzoption options);

tuple<VectorXi, MatrixXi> pattern(unsigned nsymb, const string &ptype);
tuple<VectorXi, MatrixXi> pattern(unsigned nsymb, const string &ptype, string array[]);

E pbc(E ex, E ey);

std::tuple<E, E> pbs(E e);

MatrixXcd rxFrontend(E e, RowVectorXd lambda, int symbrate, const Fiber &fiber);

}  // namespace PARALLEL_TYPE

}  // namespace SimuLib

#endif  // EXTERNALFUNCTIONS_HPP
