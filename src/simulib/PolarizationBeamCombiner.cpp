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

#include "Internal"

namespace SimuLib {

namespace HARDWARE_TYPE {

/**
 * @brief polarization beam combiner.
 * @param ex: contains information about the electric field.
 * @param ey: contains information about the electric field.
 * @return E struct describing electric fields
 */
E pbc(E ex, E ey) {
    int npol = 1;

    if (ex.lambda != ey.lambda)
        ERROR("different wavelengths: use a multiplexer");

    if (ex.field.cols() == 2 * ex.lambda.size())
        npol = 2;

    E e;
    e.lambda = ex.lambda;
    e.field  = MatrixXcd(ex.field.rows(), ex.field.cols()).setZero();

    for (int i = 0; i < ex.field.cols(); i++) {
        if (i % 2 == 0) {
            e.field.col(i) = ex.field.col(i);
        } else if (npol == 2) {
            e.field.col(i) = ey.field.col(i);
        }
    }
    return e;
}

}

}  // namespace SimuLib