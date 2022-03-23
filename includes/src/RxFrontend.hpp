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
 * Created: 2022/3/22
 * Supported by: National Key Research and Development Program of China
 */
#ifndef RXFRONTEND_HPP
#define RXFRONTEND_HPP

#include "Eigen/Core"
#include "LaserSource.hpp"
#include "Fiber.hpp"

using namespace Eigen;

MatrixXcd rxFrontend(E e, RowVectorXd lambda, int symbrate, const Fiber& fiber);

#endif  // RXFRONTEND_HPP
