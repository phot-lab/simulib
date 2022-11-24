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

/**
 * common types
 */

#ifndef OPTICALAB_GLOBALS_H
#define OPTICALAB_GLOBALS_H

#include "CommonTypes.hpp"

namespace SimuLib {

extern const double LIGHT_SPEED;      // Speed of light in vacuum [m/s]
extern const double PLANK_CONST;      // Planck's constant [J*s]
extern const double ELE_CHARGE;       // Electron's charge [C] (CODATA value, year 2006)
extern const double BOLTZMANN_CONST;  // Boltzmann's constant [J/oK] (CODATA value)
extern const double PI;

extern Gstate gstate;

}  // namespace SimuLib

#endif  // OPTICALAB_GLOBALS_H
