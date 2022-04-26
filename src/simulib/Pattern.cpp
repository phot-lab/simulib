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
 * Created: 2022/3/14
 * Supported by: National Key Research and Development Program of China
 */

#include "Internal"
#include <string>

using namespace std;

namespace SimuLib {

namespace HARDWARE_TYPE {

tuple<VectorXi, MatrixXi> genPattern(unsigned nSymbol, const string &patternType) {
    RowVectorXi pattern;
    MatrixXi patternBinary;
    if (patternType == "rand") {  // RANDOM UNIFORMLY-DISTRIBUTED PATTERN
        int qq = 2;
        pattern.resize(nSymbol);
        for (Index i = 0; i < pattern.size(); ++i) {
            pattern[i] = floor(uniformRng() * qq);
        }
        patternBinary.resize(pattern.size(), log2(qq));
        for (Index i = 0; i < patternBinary.rows(); ++i) {
            patternBinary.row(i) = DecToBin(pattern[i], log2(qq));
        }
    }
    return make_tuple(pattern.transpose(), patternBinary);
}

tuple<VectorXi, MatrixXi> genPattern(unsigned nSymbol, const string &patternType, string array[]) {
    RowVectorXi pattern;
    MatrixXi patternBinary;
    if (patternType == "rand") {  // RANDOM UNIFORMLY-DISTRIBUTED PATTERN
        int qq;
        if (array[0] == "digit") {
            qq = strToDigit(array[1]);
        } else if (array[0] == "alpha") {
            FormatInfo format_info = modFormatInfo(array[1]);
            qq                     = format_info.digit;
            if (format_info.alpha == "randn") {
                ERROR("Cannot use uniform-distributed symbols with a randn format.");
            }
        } else {
            qq = 2;
        }
        pattern.resize(nSymbol);
        for (Index i = 0; i < pattern.size(); ++i) {
            pattern[i] = floor(uniformRng2() * qq);
        }
        patternBinary.resize(pattern.size(), log2(qq));
        for (Index i = 0; i < patternBinary.rows(); ++i) {
            patternBinary.row(i) = DecToBin(pattern[i], log2(qq));
        }
    }
    return make_tuple(pattern.transpose(), patternBinary);
}

}  // namespace HARDWARE_TYPE

}  // namespace SimuLib