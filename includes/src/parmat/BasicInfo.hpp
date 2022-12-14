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
 * Created: 2022/4/12
 * Supported by: National Key Research and Development Program of China
 */
#ifndef BASICINFO_HPP
#define BASICINFO_HPP

#include "src/Eigen/Core"
#include "src/unsupported/Eigen/FFT"

namespace SimuLib {

using Eigen::ArrayXd;
using Eigen::Dynamic;
using Eigen::FFT;
using Eigen::Index;
using Eigen::Map;
using Eigen::Matrix;
using Eigen::MatrixBase;
using Eigen::nbThreads;
using Eigen::RowMajor;
using Eigen::Unaligned;

// Since the cuBLAS doesn't provide int matrix API, so I don't implement these
using Eigen::MatrixXi;
using Eigen::RowVectorXi;
using Eigen::VectorXi;


}  // namespace SimuLib

#endif  // BASICINFO_HPP
