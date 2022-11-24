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

#include <iostream>

namespace SimuLib {

namespace GPU {

template<int Rows, int Cols>
class MatrixDouble : public Eigen::Matrix<double, Rows, Cols> {
    typedef Eigen::Matrix<double, Rows, Cols> Base;
    typedef MatrixDouble<Rows, Cols> Derived;
    using Base::Base;

public:
    using Base::operator*;
    using Base::operator+;
    using Base::operator-;

#ifdef SIMULIB_USE_GPU

    template<typename Type>
    Derived operator+(const MatrixBase<Type> &other) {
        return geam(*this, static_cast<Derived>(other), true);
    }

    template<typename Type>
    Derived operator-(const MatrixBase<Type> &other) {
        return geam(*this, static_cast<Derived>(other), false);
    }

    template<typename Type>
    Derived operator*(const MatrixBase<Type> &other) {
        return gemm(*this, static_cast<Derived>(other));
    }

#endif
};

typedef MatrixDouble<Dynamic, Dynamic> MatrixXd;
typedef MatrixDouble<Dynamic, 1> VectorXd;
typedef MatrixDouble<1, Dynamic> RowVectorXd;

}  // namespace GPU

namespace CPU {

template<int Rows, int Cols>
class MatrixDouble : public Eigen::Matrix<double, Rows, Cols> {
    typedef Eigen::Matrix<double, Rows, Cols> Base;
    using Base::Base;
};

typedef MatrixDouble<Dynamic, Dynamic> MatrixXd;
typedef MatrixDouble<Dynamic, 1> VectorXd;
typedef MatrixDouble<1, Dynamic> RowVectorXd;

}  // namespace CPU

}  // namespace SimuLib

#endif  // MATRIXDOUBLE_HPP
