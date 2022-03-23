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
 * Created: 2022/3/19
 * Supported by: National Key Research and Development Program of China
 */
#ifndef DIGITALMODULATOR_HPP
#define DIGITALMODULATOR_HPP

#include "CommonTypes.hpp"
#include "Fiber.hpp"

struct FormatInfo {
    double digit     = 0;
    double symb_mean = 0;
    double symb_var  = 0;
    string alpha     = "";
    string family    = "";
};

FormatInfo modFormatInfo(const string &modFormat);

tuple<MatrixXcd, double> digitalModulator(MatrixXi pat_bin, double symbrate, Par par, string mod_format, string ptype);

#endif  // DIGITALMODULATOR_HPP
