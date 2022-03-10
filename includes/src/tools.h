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
 * Created: 2022/2/28
 * Supported by: National Key Research and Development Program of China
 */

#ifndef OPTICALAB_TOOLS_H
#define OPTICALAB_TOOLS_H

#include "Eigen/Dense"
#include <cstring>
#include <string>
#include <vector>

#define ERROR(...) error(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARNING(...) warning(__FILE__, __LINE__, __func__, __VA_ARGS__)

using namespace std;
using namespace Eigen;

void error(const string &filename, const int &line, const string &func_name, const string &s);
void warning(const string &filename, const int &line, const string &func_name, const string &s);

bool IsInt(const double &n);

template<typename T>
Matrix<T, Dynamic, 1> STLToEigen(vector<T> v) {
    return Map<Matrix<T, Dynamic, 1>, Unaligned>(v.data(), (long) v.size());
}

template<typename T>
vector<T> EigenToSTL(const Matrix<T, Dynamic, 1> &v) {
    vector<T> vec(v.data(), v.data() + v.size());
    return vec;
}

VectorXd Diff(const VectorXd &v);
VectorXd RemoveZero(const VectorXd &a, const VectorXd &b);
VectorXd GenStepVector(const double &start, const double &step, const double &end);
VectorXd GenVector(const double &start, const double &end);
MatrixXcd FastExp(const MatrixXcd& m);


#endif  // OPTICALAB_TOOLS_H
