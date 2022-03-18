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

#include "simulib"

using namespace Eigen;
using namespace std;

VectorXd Diff(const VectorXd &v) {
    auto block_size = v.size() - 1;
    return v.block(1, 0, block_size, 1) - v.block(0, 0, block_size, 1);
}

RowVectorXd GenVector(const double &start, const double &end) {
    if (start > end) {
        RowVectorXd vec(0);
        return vec;
    }
    auto size = (unsigned long) ((end - start) + 1);
    RowVectorXd res(size);
    double cur = start;
    for (unsigned i = 0; i < size; ++i) {
        res(i) = cur++;
    }
    return res;
}

RowVectorXd GenStepVector(const double &start, const double &step, const double &end) {
    auto size = (unsigned long) ((end - start) / step + 1);
    RowVectorXd res(size);
    double cur = start;
    for (unsigned i = 0; i < size; ++i) {
        res(i) = cur;
        cur += step;
    }
    return res;
}

VectorXd RemoveZero(const VectorXd &a, const VectorXd &b) {
    assert(a.size() == b.size());
    vector<double> vec;
    for (Index i = 0; i < a.size(); ++i) {
        if (b[i] != 0) vec.emplace_back(a[i]);
    }
    return STLToEigen(vec);
}

MatrixXcd FastExp(const MatrixXcd &m) {
    return (m * 1i).array().exp();
}

void SetValueIndices(VectorXd &vec, const VectorXd &indices, double value) {
    for (const auto &item: indices) {
        vec((Index) (item - 1)) = value;
    }
}

void ReplaceVector(VectorXd &vec, VectorXd indices, VectorXd replace) {
    assert(indices.size() == replace.size());
    for (Index i = 0; i < indices.size(); ++i) {
        vec((Index) indices(i)) = replace(i);
    }
}