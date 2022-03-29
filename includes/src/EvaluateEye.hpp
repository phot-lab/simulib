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
 * Created: 2022/3/29
 * Supported by: National Key Research and Development Program of China
 */
#ifndef EVALUATEEYE_HPP
#define EVALUATEEYE_HPP

#include "Eigen/Core"
#include "Fiber.hpp"
#include <complex>

using namespace Eigen;
using namespace std;

tuple<complex<double>, MatrixXcd> evaluateEye(MatrixXi patternBinary, const MatrixXcd& signal, double symbrate, const string& modFormat, const Fiber& fiber);

#endif  // EVALUATEEYE_HPP
