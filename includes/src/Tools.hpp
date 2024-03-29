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

/**
 * Tools function declarations
 */

#ifndef OPTICALAB_TOOLS_H
#define OPTICALAB_TOOLS_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace SimuLib {

#define ERROR(...) error(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARNING(...) warning(__FILE__, __LINE__, __func__, __VA_ARGS__)

namespace HARDWARE_TYPE {

void error(const string &filename, const int &line, const string &func_name, const string &s);

void warning(const string &filename, const int &line, const string &func_name, const string &s);

bool isInt(const double &n);

template<typename T>
void Print2DArray(T *array, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            cout << array[i][j] << " ";
        }
        cout << endl;
    }
}

template<typename T>
void PrintArray(T *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        cout << array[i] << " ";
    }
    cout << endl;
}

tuple<int, int> continuedFractionApproximation(double f);

string findAlpha(const string &s);

string findDigit(const string &s);

double strToDigit(const string &s);

string digitToStr(double digit);

double uniformRng();
double uniformRng2();
double normalRng();
MatrixXd normalRng(Index rows, Index cols);

RowVectorXi DecToBin(unsigned long dec, int n_bit);

unsigned long HashStr(const string &s);

std::string toUpper(std::string data);
std::string toLower(std::string data);

complex<double> fastExp(complex<double> data);

VectorXi readPattern(const std::string &filepath);

// The implementation of fftShift referred to this website
// https://kerpanic.wordpress.com/2016/04/08/more-efficient-ifftshift-fftshift-in-c/
// 于optilux的fftShift有5-10%的误差。 There are 5-10% deviation from Optilux fftShift.
template<typename T>
Matrix<T, Dynamic, 1> fftShift(const Matrix<T, Dynamic, 1> &in) {
    Index size = in.rows();
    Matrix<T, Dynamic, 1> out(size, 1);

    unsigned pivot     = (size % 2 == 0) ? (size / 2) : ((size - 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf  = pivot;

    memcpy(out.data(), in.data() + pivot, sizeof(double) * rightHalf);
    memcpy(out.data() + rightHalf, in.data(), sizeof(double) * leftHalf);
    return out;
}

template<typename T>
Matrix<T, Dynamic, 1> ifftShift(const Matrix<T, Dynamic, 1> &in) {
    Index size = in.rows();
    Matrix<T, Dynamic, 1> out(size, 1);

    unsigned pivot     = (size % 2 == 0) ? (size / 2) : ((size - 1) / 2);
    unsigned rightHalf = size - pivot;
    unsigned leftHalf  = pivot;

    memcpy(out.data(), in.data() + pivot, sizeof(double) * rightHalf);
    memcpy(out.data() + rightHalf, in.data(), sizeof(double) * leftHalf);
    return out;
}

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib

#endif  // OPTICALAB_TOOLS_H
