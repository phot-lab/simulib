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

/**
 * function declarations realted to matrix operations
 */

#ifndef SIMULIB_MATRIX_TOOLS_H
#define SIMULIB_MATRIX_TOOLS_H

#include <iostream>
#include <list>
#include <map>
#include <vector>

using namespace std;

namespace SimuLib {

namespace HARDWARE_TYPE {

template<typename T>
Matrix<T, Dynamic, 1> stlToEigen(vector<T> v) {
    return Map<Matrix<T, Dynamic, 1>, Unaligned>(v.data(), (long) v.size());
}

template<typename T>
vector<T> eigenToSTL(const Matrix<T, Dynamic, 1> &v) {
    vector<T> vec(v.data(), v.data() + v.size());
    return vec;
}

template<typename T>
MatrixXi getUnit(Matrix<T, Dynamic, Dynamic> m) {
    return MatrixXi::Ones(m.rows(), m.cols());
}

VectorXd diff(const VectorXd &v);

VectorXd removeZero(const VectorXd &a, const VectorXd &b);

RowVectorXd genStepVector(const double &start, const double &step, const double &end);

RowVectorXd genVector(const double &start, const double &end);

MatrixXcd fastExp(const MatrixXcd &m);

void setValueIndices(VectorXd &vec, const VectorXd &indices, double value);

void replaceVector(VectorXd &vec, VectorXd indices, VectorXd replace);

template<typename T>
Matrix<T, Dynamic, 1> matrixToVec(Matrix<T, Dynamic, Dynamic> m) {
    return m.reshaped(m.rows() * m.cols(), 1);
    //    return Map<Matrix<T, Dynamic, 1>>(m.data(), m.cols() * m.rows());
}

// template<typename Derived>
// MatrixBase<Derived> matrixToVec(MatrixBase<Derived> m){
//     return m.reshaped(m.rows(),m.cols());
// }

template<typename T>
Matrix<T, Dynamic, 1> truncateVec(Matrix<T, Dynamic, 1> &vec, VectorXd indices) {
    assert(vec.size() >= indices.size());
    Matrix<T, Dynamic, 1> truncate(indices.size());
    for (Index i = 0; i < indices.size(); ++i) {
        truncate[i] = vec((Index) indices[i] - 1);
    }
    return truncate;
}

template<typename T>
Matrix<T, Dynamic, Dynamic> truncateMatrix(Matrix<T, Dynamic, Dynamic> &m, VectorXi indices) {
    assert(m.rows() >= indices.size());
    Matrix<T, Dynamic, Dynamic> truncate(indices.size(), m.cols());
    for (Index i = 0; i < indices.size(); ++i) {
        truncate.row(i) = m.row((Index) indices[i]);
    }
    return truncate;
}

MatrixXcd matVecProduct(MatrixXcd m, const VectorXcd &v);

// https://blog.csdn.net/xinshuwei/article/details/84643810
template<typename T>
Matrix<T, Dynamic, Dynamic> circShift(Matrix<T, Dynamic, Dynamic> data, int A, int B = 0) {
    Index row = data.rows();
    Index col = data.cols();
    Matrix<T, Dynamic, Dynamic> out(row, col);
    Matrix<T, Dynamic, Dynamic> y(row, col);
    long a = A % row;
    long b = B % col;
    if (a > 0)  // Move a rows downwards
    {
        out.topRows(a)          = data.bottomRows(a);
        out.bottomRows(row - a) = data.topRows(row - a);
    } else if (a < 0)  // Move a rows upwards
    {
        out.topRows(row + a)   = data.bottomRows(row + a);
        out.bottomRows(abs(a)) = data.topRows(abs(a));
    } else if (a == 0) {
        out.array() = data.array();
    }

    if (b > 0)  //  Move b columns rightwards
    {
        y.leftCols(b)        = out.rightCols(b);
        y.rightCols(col - b) = out.leftCols(col - b);
    } else if (b < 0)  //  Move b columns leftwards
    {
        y.leftCols(col + b) = out.rightCols(col + b);
        y.rightCols(abs(b)) = out.leftCols(abs(b));
    } else if (b == 0) {
        y = out;
    }
    return y;
}

template<typename T>
Matrix<T, Dynamic, 1> sumRow(Matrix<T, Dynamic, Dynamic> m) {
    return m.rowwise().sum();
}

template<typename T>
Matrix<T, 1, Dynamic> sumCol(Matrix<T, Dynamic, Dynamic> m) {
    return m.colwise().sum();
}

VectorXd modulusEigen(const VectorXd &v, int n);

VectorXd nModulusEigen(VectorXd v, int n);

template<typename T>
Matrix<T, Dynamic, Dynamic> selectRows(Matrix<T, Dynamic, Dynamic> m, VectorXi v) {
    Matrix<T, Dynamic, Dynamic> res(m.rows(), m.cols());
    Index count = 0;
    for (Index i = 0; i < v.rows(); ++i) {
        if (v(i) == 1) {
            res.row(count) = m.row(i);
            count++;
        }
    }
    res.conservativeResize(count, res.cols());
    return res;
}

template<typename T>
Matrix<T, Dynamic, 1> excludeFirst(Matrix<T, Dynamic, 1> v) {
    return v.segment(1, v.size() - 1);
}

template<typename T>
Matrix<T, Dynamic, 1> excludeLast(Matrix<T, Dynamic, 1> v) {
    return v.segment(0, v.size() - 1);
}

/**
 * maxRow(m) will return a row vector with the biggest sum
 * @param m : Given matrix
 * @return A row vector with the biggest sum
 */

RowVectorXd maxRow(MatrixXd m);
RowVectorXcd maxRow(MatrixXcd m);
RowVectorXd minRow(MatrixXd m);
RowVectorXcd minRow(MatrixXcd m);

VectorXd maxCol(MatrixXd m);
VectorXcd maxCol(MatrixXcd m);
VectorXd minCol(MatrixXd m);
VectorXcd minCol(MatrixXcd m);

VectorXd sortEigen(VectorXd v, bool ascend = true);
VectorXcd sortEigen(VectorXcd v, bool ascend = true);

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib

#endif  // SIMULIB_MATRIX_TOOLS_H
