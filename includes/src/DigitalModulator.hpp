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

/**
 * structs that are used in digital modulator implementation
 */

#ifndef DIGITALMODULATOR_HPP
#define DIGITALMODULATOR_HPP

#include "CommonTypes.hpp"
#include "Fiber.hpp"
#include "Pattern.hpp"

namespace SimuLib {

struct Par {
    string emph    = "asin";
    int nsps       = 0;
    string norm    = "iid";
    int par        = 0;
    double rolloff = 0;
    double duty    = 1;
    string modFormat;
};

struct FormatInfo {
    double digit     = 0;
    double symb_mean = 0;
    double symb_var  = 0;
    string alpha;
    string family;
};

namespace HARDWARE_TYPE {

FormatInfo modFormatInfo(const string &modFormat);

}

}  // namespace SimuLib

#endif  // DIGITALMODULATOR_HPP
