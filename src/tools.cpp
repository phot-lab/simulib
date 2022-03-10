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

#include <simulib>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Eigen;
using namespace std;

void LineInfo(const string &filename, const int &line, const string &func_name) {
    cerr << "File Name: " << filename << endl;  // File name
    cerr << "Code Line: " << line << endl;      // Code line
    cerr << "Function: " << func_name << endl;  // Function name
}

void error(const string &filename, const int &line, const string &func_name, const string &s) {
    LineInfo(filename, line, func_name);
    throw runtime_error(s);
}

void warning(const string &filename, const int &line, const string &func_name, const string &s) {
    LineInfo(filename, line, func_name);
    cerr << "Warning: " << s << endl
         << endl;
}

bool IsInt(const double &n) {
    return abs(round(n) - n) < 0.000000000000001;
}

VectorXd Diff(const VectorXd &v) {
    auto block_size = v.size() - 1;
    return v.block(1, 0, block_size, 1) - v.block(0, 0, block_size, 1);
}

VectorXd GenVector(const double &start, const double &end) {
    if (start > end) {
        VectorXd vec(0);
        return vec;
    }
    auto size = (unsigned long) ((end - start) + 1);
    VectorXd res(size);
    double cur = start;
    for (unsigned i = 0; i < size; ++i) {
        res(i) = cur++;
    }
    return res;
}

VectorXd GenStepVector(const double &start, const double &step, const double &end) {
    auto size = (unsigned long) ((end - start) / step + 1);
    VectorXd res(size);
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

MatrixXcd FastExp(const MatrixXcd& m) {
    return (m * 1i).array().exp();
}
