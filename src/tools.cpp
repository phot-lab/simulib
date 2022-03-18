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

#include <cmath>
#include <iostream>
#include <regex>
#include "simulib"
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <random>

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

// https://gist.github.com/mikeando/7073d62385a34a61a6f7
tuple<int, int> continued_fraction_approximation(double f) {
    if (f == 0)
        return make_tuple(0, 1);
    bool flag = false;
    if (f <= 0) {
        f = -f;
        flag = true;
    }
    double tol = 1.e-6 * f;
    int Aprev[2] = {1, 0};
    int Bprev[2] = {0, 1};

    double x = f;
    while (true) {
        int n = floor(x);
        x -= n;
        x = 1 / x;

        int denominator = Aprev[0] + n * Aprev[1];
        Aprev[0] = Aprev[1];
        Aprev[1] = denominator;

        int numerator = Bprev[0] + n * Bprev[1];
        Bprev[0] = Bprev[1];
        Bprev[1] = numerator;

        double approx = (double) numerator / (double) denominator;
        if (fabs(approx - f) < tol) {
            if (flag) {
                return {-numerator, denominator};
            } else {
                return {numerator, denominator};
            }
        }
    }
}

string find_alpha(const string &s) {
    std::smatch m;
    std::regex e("([a-zA-Z]?)");
    string result;
    for (char i: s) {
        string x(1, i);
        regex_search(x, m, e);
        if (m.str() != " ")
            result += m.str();
    }
    return result;
}

string find_digit(const string &s) {
    std::smatch m;
    std::regex e("([0-9]?)");
    string result;
    for (char i: s) {
        string x(1, i);
        regex_search(x, m, e);
        if (m.str() != " ")
            result += m.str();
    }
    return result;
}

double StrToDigit(const string &s) {
    double res;
    stringstream ss;
    ss << s;
    ss >> res;
    return res;
}

string DigitToStr(double digit) {
    string res;
    stringstream ss;
    ss << digit;
    ss >> res;
    return res;
}

double UniformRng() {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> distr(0, 1);
    return distr(generator);
}

RowVectorXi DecToBin(unsigned long dec, int n_bit) {
    auto binary = std::bitset<64>(dec).to_string();  // to binary
    size_t bin_index = binary.size() - 1;
    RowVectorXi res(n_bit);
    for (Index i = res.size() - 1; i >= 0; --i) {
        res[i] = binary[bin_index--] - '0';
    }
    return res;
}

unsigned long HashStr(const string &s) {
    hash<string> hash;
    return hash(s);
}


