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

#include "simulib"
#include <string>

using namespace std;

tuple<VectorXi, MatrixXi> pattern(unsigned nsymb, const string &ptype, string format, ...) {
    RowVectorXi pat;
    MatrixXi pat_bin;
    va_list args;
    va_start(args, format);
    size_t count = format.size();
    if (ptype == "rand") {  // RANDOM UNIFORMLY-DISTRIBUTED PATTERN
        bool set_state = false;
        int qq;
        if (count == 0)
            qq = 2;  // alphabet
        else if (count == 1) {
            if (format[0] == 'd') {
                double arg1 = va_arg(args, double);
                // setrandseed未完成
                qq        = 2;
                set_state = true;
            } else if (format[0] == 'a') {
                auto array = va_arg(args, string *);
                if (array[0] == "digit") {
                    qq = StrToDigit(array[1]);
                } else if (array[0] == "alpha") {
                    FormatInfo format_info = ModFormatInfo(array[1]);
                    qq                     = format_info.digit;
                    if (format_info.alpha == "randn") {
                        ERROR("Cannot use uniform-distributed symbols with a randn format.");
                    }
                } else {
                    qq = 2;
                }
            } else {
                ERROR("Second input argument must be an integer or an array.");
            }
        }
        pat.resize(nsymb);
        for (Index i = 0; i < pat.size(); ++i) {
            pat[i] = floor(UniformRng() * qq);
        }
        if (set_state) {
            // 未完成
        }
        pat_bin.resize(pat.size(), log2(qq));
        for (Index i = 0; i < pat_bin.rows(); ++i) {
            pat_bin.row(i) = DecToBin(pat[i], log2(qq));
        }
    }
    va_end(args);
    return make_tuple(pat.transpose(), pat_bin);
}
