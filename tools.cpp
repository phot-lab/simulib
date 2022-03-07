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

#include <string>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <vector>
#include "Dense"

using namespace Eigen;
using namespace std;

void LineInfo(const string &filename, const int &line, const string &func_name) {
    cerr << "File Name: " << filename << endl;   // File name
    cerr << "Code Line: " << line << endl;       // Code line
    cerr << "Function: " << func_name << endl;  // Function name
}

void error(const string &filename, const int &line, const string &func_name, const string &s) {
    LineInfo(filename, line, func_name);
    throw runtime_error(s);
}

void warning(const string &filename, const int &line, const string &func_name, const string &s) {
    LineInfo(filename, line, func_name);
    cerr << "Warning: " << s << endl << endl;
}

bool IsInt(const double &n) {
    return abs(round(n) - n) < 0.000000000000001;
}

// The implementation of fftshift referred to this website
// https://kerpanic.wordpress.com/2016/04/08/more-efficient-ifftshift-fftshift-in-c/
VectorXd FFTShift(const VectorXd &vec) {
    Index size = vec.size();
    VectorXd res(size);

    unsigned pivot = (size % 2 == 0) ? (size / 2) : ((size + 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf = pivot;

    memcpy(&res(0), &vec(0 + pivot), sizeof(double) * rightHalf);
    memcpy(&res(0 + rightHalf), &vec(0), sizeof(double) * leftHalf);
    return res;
}

VectorXd IFFTShift(const VectorXd &vec) {
    Index size = vec.size();
    VectorXd res(size);

    unsigned pivot = (size % 2 == 0) ? (size / 2) : ((size - 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf = pivot;

    memcpy(&res(0), &vec(0 + pivot), sizeof(double) * rightHalf);
    memcpy(&res(0 + rightHalf), &vec(0), sizeof(double) * leftHalf);
    return res;
}

VectorXd GenVector(const double &start, const double &step, const double &end) {
    auto size = (unsigned long) ((end - start) / step + 1);
    VectorXd res(size);
    double cur = start;
    for (unsigned i = 0; i < size; ++i) {
        res(i) = cur;
        cur += step;
    }
    return res;
}

