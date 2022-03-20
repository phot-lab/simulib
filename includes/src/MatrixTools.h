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
 * Created: 2022/3/18
 * Supported by: National Key Research and Development Program of China
 */
#ifndef SIMULIB_MATRIX_TOOLS_H
#define SIMULIB_MATRIX_TOOLS_H

#include "Eigen/Core"
#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;

template<typename T>
Matrix<T, Dynamic, 1> STLToEigen(vector<T> v) {
    return Map<Matrix<T, Dynamic, 1>, Unaligned>(v.data(), (long) v.size());
}

template<typename T>
vector<T> EigenToSTL(const Matrix<T, Dynamic, 1> &v) {
    vector<T> vec(v.data(), v.data() + v.size());
    return vec;
}

template<typename T>
MatrixXi GetUnit(Matrix<T, Dynamic, Dynamic> m) {
    return MatrixXi::Ones(m.rows(), m.cols());
}

VectorXd Diff(const VectorXd &v);

VectorXd RemoveZero(const VectorXd &a, const VectorXd &b);

RowVectorXd GenStepVector(const double &start, const double &step, const double &end);

RowVectorXd GenVector(const double &start, const double &end);

MatrixXcd FastExp(const MatrixXcd &m);

void SetValueIndices(VectorXd &vec, const VectorXd &indices, double value);

void ReplaceVector(VectorXd &vec, VectorXd indices, VectorXd replace);

template<typename T>
Matrix<T, Dynamic, 1> MatrixToVector(Matrix<T, Dynamic, Dynamic> m) {
    return Map<Matrix<T, Dynamic, 1>>(m.data(), m.cols() * m.rows());
}

template<typename T>
Matrix<T, Dynamic, 1> TruncateVector(Matrix<T, Dynamic, 1> &vec, VectorXd indices) {
    assert(vec.size() >= indices.size());
    Matrix<T, Dynamic, 1> truncate(indices.size());
    for (Index i = 0; i < indices.size(); ++i) {
        truncate[i] = vec((Index) indices[i] - 1);
    }
    return truncate;
}

#endif  // SIMULIB_MATRIX_TOOLS_H
