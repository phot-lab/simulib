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
#ifndef MATRIXDOUBLE_HPP
#define MATRIXDOUBLE_HPP

#ifdef SIMULIB_USE_GPU
#include "FunctionsDouble.hpp"
#endif

namespace ParMat {

#ifdef SIMULIB_USE_GPU
    template<int Rows, int Cols>
    class Test : public Eigen::Matrix<double, Rows, Cols> {
        typedef Eigen::Matrix<double, Rows, Cols> Base;
        using Base::Base;

    public:
        using Base::operator*;
        using Base::operator+;
        using Base::operator-;

        Test<Rows, Cols> operator+(const Test<Rows, Cols> &other) {
            return geam(*this, other, true);
        }

        Test<Rows, Cols> operator-(const Test<Rows, Cols> &other) {
            return geam(*this, other, false);
        }

        Test<Rows, Cols> operator*(const Test<Rows, Cols> &other) {
            return gemm(*this, other);
        }
    };

    typedef Test<Dynamic, Dynamic> MatrixXd;
    typedef Test<Dynamic, 1> VectorXd;
    typedef Test<1, Dynamic> RowVectorXd;

#else
    using Eigen::MatrixXd;
    using Eigen::RowVectorXd;
    using Eigen::VectorXd;
#endif

}  // namespace ParMat

#endif  // MATRIXDOUBLE_HPP
