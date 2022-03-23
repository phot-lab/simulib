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

#include "SimuLib"
#include <string>
#include <vector>

using namespace std;

FormatInfo modFormatInfo(const string &modFormat);
MatrixXi finalizePat2Samp(const MatrixXi &pat_bin, const FormatInfo &format_info);

MatrixXi Pat2Samp(const MatrixXi &pat_bin, const string &modFormat) {
    FormatInfo formatInfo = modFormatInfo(modFormat);
    double num_bit        = log2(formatInfo.digit);
    MatrixXi dataid;
    Index n_row = pat_bin.rows();
    Index n_col = pat_bin.cols();
    if (modFormat == "randn") {
        dataid = pat_bin;
        return dataid;
    }
    if (n_col == 1) {
        // 未完成
    }
    // From now on, pat is a binary matrix
    dataid = finalizePat2Samp(pat_bin, formatInfo);
    // Plot Constellation （未完成）
    return dataid;
}

FormatInfo modFormatInfo(const string &modFormat) {
    FormatInfo formatInfo{};
    formatInfo.digit = strToDigit(findDigit(modFormat));
    formatInfo.alpha = findAlpha(modFormat);
    if (modFormat == "bpsk" || modFormat == "dpsk") {
        formatInfo.digit     = 2;
        formatInfo.family    = "psk";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
    } else if (modFormat == "ook") {
        formatInfo.digit = 2;
        formatInfo.family = "ook";
        formatInfo.symb_mean = 1;
        formatInfo.symb_var  = 1;
    } else if (modFormat == "qpsk" || modFormat == "dqpsk" || (formatInfo.alpha == "qam" && formatInfo.digit == 4)) {
        formatInfo.digit     = 4;
        formatInfo.family    = "psk";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
    } else if (formatInfo.alpha == "psk") {
        formatInfo.family    = "psk";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
    } else if (formatInfo.alpha == "qamcirc" || formatInfo.alpha == "qamstar" || (formatInfo.alpha == "qamrect" && formatInfo.digit == 8)) {
        formatInfo.family    = "qam";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
    } else if (formatInfo.alpha == "qam") {
        formatInfo.family    = "qam";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
    } else if (formatInfo.alpha == "pam") {
        formatInfo.family    = "pam";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
    } else if (formatInfo.alpha == "randn") {
        formatInfo.family    = "randn";
        formatInfo.symb_mean = 0;
        formatInfo.symb_var  = 1;
        formatInfo.digit     = INFINITY;
    } else {
        ERROR("Unknown modulation format");
    }
    return formatInfo;
}

MatrixXi finalizePat2Samp(const MatrixXi &pat_bin, const FormatInfo &format_info) {

    // Note: dataid is forced to have average energy 1 with a uniformly distributed pattern.
    MatrixXi dataid;
    Index n_row = pat_bin.rows();
    Index n_col = pat_bin.cols();

    double m = pow(2, n_col);  // constellation size

    auto unit = getUnit(pat_bin);
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