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

#include <Internal>

namespace SimuLib {

namespace HARDWARE_TYPE {
/**
 * @brief A polarization beam splitter.
 * @param E: contains information about the electric field.
 * @return two electric fields EX and EY with orthogonal polarizations at 45 degrees with respect to E.
 */
std::tuple<E, E> pbs(E e) {
    if (e.field.cols() != 2 * e.lambda.size())
        ERROR("A beam splitter can be used only in dual-polarization mode.");

    E ex = e;
    ex.field.setZero();
    E ey = e;
    ey.field.setZero();
    for (int col = 0; col < e.field.cols(); col++) {
        if (col % 2 == 0)
            ex.field.col(col) = (e.field.col(col) - e.field.col(col + 1)) / sqrt(2);
        else
            ey.field.col(col) = (e.field.col(col) + e.field.col(col - 1)) / sqrt(2);
    }
    return make_tuple(ex, ey);
}
}
}  // namespace SimuLib