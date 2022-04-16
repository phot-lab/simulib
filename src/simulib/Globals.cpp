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
 * Created: 2022/3/6
 * Supported by: National Key Research and Development Program of China
 */

#include "Internal"

namespace SimuLib {

const double LIGHT_SPEED     = 299792458;        // Speed of light in vacuum [m/s]
const double PLANK_CONST     = 6.62606896e-34;   // Planck's constant [J*s]
const double ELE_CHARGE      = 1.602176487e-19;  // Electron's charge [C] (CODATA value, year 2006)
const double BOLTZMANN_CONST = 1.3806504E-23;    // Boltzmann's constant [J/oK] (CODATA value)
const double PI = 3.1415926535897931;    // pi

Gstate gstate;

}  // namespace SimuLib