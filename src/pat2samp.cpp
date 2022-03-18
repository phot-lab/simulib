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
 * Created: 2022/3/15
 * Supported by: National Key Research and Development Program of China
 */

#include "simulib"
#include <string>
#include <vector>

using namespace std;

FormatInfo ModFormatInfo(const string &mod_format);
MatrixXi FinalizePat2Samp(const MatrixXi &pat_bin, const FormatInfo &format_info);

MatrixXi Pat2Samp(const MatrixXi& pat_bin, const string& mod_format) {
    FormatInfo format_info = ModFormatInfo(mod_format);
    double num_bit         = log2(format_info.digit);
    MatrixXi dataid;
    Index n_row = pat_bin.rows();
    Index n_col = pat_bin.cols();
    if (mod_format == "randn") {
        dataid = pat_bin;
        return dataid;
    }
    if (n_col == 1) {
        // 未完成
    }
    // From now on, pat is a binary matrix
    dataid = FinalizePat2Samp(pat_bin, format_info);
    // Plot Constellation （未完成）
    return dataid;
}

FormatInfo ModFormatInfo(const string &mod_format) {
    FormatInfo format_info{};
    format_info.digit = StrToDigit(find_digit(mod_format));
    format_info.alpha = find_alpha(mod_format);
    if (mod_format == "bpsk" || mod_format == "dpsk") {
        format_info.digit     = 2;
        format_info.family    = "psk";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
    } else if (mod_format == "ook") {
        format_info.digit = 2;
        format_info.family == "ook";
        format_info.symb_mean = 1;
        format_info.symb_var  = 1;
    } else if (mod_format == "qpsk" || mod_format == "dqpsk" || (format_info.alpha == "qam" && format_info.digit == 4)) {
        format_info.digit     = 4;
        format_info.family    = "psk";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
    } else if (format_info.alpha == "psk") {
        format_info.family    = "psk";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
    } else if (format_info.alpha == "qamcirc" || format_info.alpha == "qamstar" || (format_info.alpha == "qamrect" && format_info.digit == 8)) {
        format_info.family    = "qam";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
    } else if (format_info.alpha == "qam") {
        format_info.family    = "qam";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
    } else if (format_info.alpha == "pam") {
        format_info.family    = "pam";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
    } else if (format_info.alpha == "randn") {
        format_info.family    = "randn";
        format_info.symb_mean = 0;
        format_info.symb_var  = 1;
        format_info.digit     = INFINITY;
    } else {
        ERROR("Unknown modulation format");
    }
    return format_info;
}

MatrixXi FinalizePat2Samp(const MatrixXi &pat_bin, const FormatInfo &format_info) {

    // Note: dataid is forced to have average energy 1 with a uniformly distributed pattern.
    MatrixXi dataid;
    Index n_row = pat_bin.rows();
    Index n_col = pat_bin.cols();

    double m = pow(2, n_col);  // constellation size

    auto unit = GetUnit(pat_bin);
    if (format_info.family == "ook")
        dataid = 2 * pat_bin;  // average energy: 1
    else if (format_info.alpha == "bpsk" || format_info.alpha == "dpsk" || format_info.alpha == "psbt" || (format_info.family == "psk" && m == 2)) {
        dataid = 2 * pat_bin - unit;  // average energy: 1
    } else if (format_info.alpha == "qpsk" || format_info.alpha == "dqpsk") {
        // 未完成
    } else {
        ERROR("Unknown modulation format.");
    }
    return dataid;
}