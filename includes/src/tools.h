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
#include <iostream>

#define ERROR(...) error(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARNING(...) warning(__FILE__, __LINE__, __func__, __VA_ARGS__)

using namespace std;
using namespace Eigen;

void error(const string &filename, const int &line, const string &func_name, const string &s);

void warning(const string &filename, const int &line, const string &func_name, const string &s);

bool IsInt(const double &n);

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

tuple<int, int> continued_fraction_approximation(double f);

string find_alpha(const string &s);

string find_digit(const string &s);

double StrToDigit(const string &s);

string DigitToStr(double digit);

double UniformRng();

RowVectorXi DecToBin(unsigned long dec, int n_bit);

unsigned long HashStr(const string &s);

MatrixXd GetLambda(const double lamc, const double spac, const int Nch);


#endif  // OPTICALAB_TOOLS_H
