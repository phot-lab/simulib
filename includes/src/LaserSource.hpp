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
* Author: Shihao Xie
* Created: 2022/3/15
* Supported by: National Key Research and Development Program of China
*/

#ifndef SIMULIB_LASER_SOURCE_H
#define SIMULIB_LASER_SOURCE_H

#include <cfloat>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <random>

/**
 * @brief laser source simulation option
 * @param pol: polarization schema. 'single' only the x-polarization is created,otherwise even
 *             the y-polarization is created and set equal to zero in absence of noise.
 * @param linewidth: It can be a scalar or a vector of the same length of the wavelengths. A Wiener
 *                   phase noise with such a linewidth is added to the phase of  E.
 * @param n0: the one-sided spectral density [dB/GHz] of a Gaussian complex noise added to the laser field.
 */

struct Option {
    int pol               = dual;            // single or dual
    RowVectorXd linewidth = RowVectorXd(0);  // [GHz]
    double n0             = INT_MIN;         // [dB/GHz]
    enum { dual   = 2,
           single = 1 };
};

struct E {
    MatrixXd lambda;  // central wavelength [nm] of the electric field, i.e., wavelength that relates the lowpass equivalent signal to the corresponding bandpass signal.
    MatrixXcd field;  // time samples of the electric field, with polarizations (if existing) alternated on columns
};

// E laserSource(RowVectorXd ptx, RowVectorXd lam);
//
E laserSource(RowVectorXd ptx, RowVectorXd lam, Option options);
//
// E laserSource(RowVectorXd ptx, double lam, double spac, int NLAMBDA);

E laserSource(RowVectorXd ptx, RowVectorXd lam, double spac = 0.0, int NLAMBDA = 0, Option options = Option());

#endif  // SIMULIB_LASER_SOURCE_H
