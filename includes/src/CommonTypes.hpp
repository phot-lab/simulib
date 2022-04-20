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
 * Created: 2022/3/1
 * Supported by: National Key Research and Development Program of China
 */
#ifndef OPTICALAB_COMMON_TYPES_H
#define OPTICALAB_COMMON_TYPES_H

#include <limits>
#include <vector>
#include <cmath>

namespace SimuLib {

#ifndef INT_MAX
#define INT_MAX __INT_MAX__
#endif

#ifndef INT_MIN
#define INT_MIN INT64_MIN
#endif

using namespace std;

struct Gstate {
    unsigned long NSAMP;
    VectorXd FN;
    double SAMP_FREQ;
};

void initGstate(double Nsamp, double Fs);

}  // namespace SimuLib

#endif  // OPTICALAB_COMMON_TYPES_H
