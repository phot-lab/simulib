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
 * Created: 2022/4/13
 * Supported by: National Key Research and Development Program of China
 */
#ifndef MATRIXCOMPLEX_HPP
#define MATRIXCOMPLEX_HPP

#ifdef PARMAT_USE_GPU
#include "FunctionsComplex.hpp"
#endif

namespace ParMat {

#ifdef PARMAT_USE_GPU
    template<int Rows, int Cols>
    class MatrixComplex : public Eigen::Matrix<std::complex<double>, Rows, Cols> {
        typedef Eigen::Matrix<std::complex<double>, Rows, Cols> Base;
        typedef ParMat::MatrixComplex<Rows, Cols> Derived;
        using Base::Base;

    public:
        using Base::operator*;
        using Base::operator+;
        using Base::operator-;

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
    };

    typedef MatrixComplex<Dynamic, Dynamic> MatrixXcd;
    typedef MatrixComplex<Dynamic, 1> VectorXcd;
    typedef MatrixComplex<1, Dynamic> RowVectorXcd;

#else
    using Eigen::MatrixXcd;
    using Eigen::RowVectorXcd;
    using Eigen::VectorXcd;
#endif

}  // namespace ParMat

#endif  // MATRIXCOMPLEX_HPP
