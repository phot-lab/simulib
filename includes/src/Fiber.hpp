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
 * Created: 2022/5/14
 * Supported by: National Key Research and Development Program of China
 */

/**
 * structs related to fiber module
 */

#ifndef OPTICALAB_FIBER_TYPES_H
#define OPTICALAB_FIBER_TYPES_H

#include "LaserSource.hpp"
#include <complex>
#include <memory>
#include <vector>

using namespace std;

namespace SimuLib {

struct Linear {
    bool is_scalar;
    bool is_unique;
};

struct NonScalarLinear : Linear {
    vector<MatrixXcd> matin;
    MatrixXd db;
};

struct ScalarLinear : Linear {
    double matin;
    double db;
};

struct Out {
    double time;             // elapsed time [s] within the function FIBER.
    double firstStepLength;  // first step length [m] used by the SSFM.
    unsigned long nCycle;    // number of SSFM iterations.
};

struct Fiber {
    double length            = 10000;   // fiber length [m]
    double lambda            = 1550;    // lambda: wavelength [nm] of fiber parameters
    double attenuation       = 1;       // alphadB: fiber attenuation [dB/km]
    double dispersion        = 17;      // fiber chromatic dispersion coefficient [ps/nm/km]
    double slope             = 0;       // fiber slope [ps/nm^2/km], i.e. derivative of X.disp
    double nonlinearIndex    = 0;       // n2: fiber nonlinear index [m^2/W]
    double effectiveArea     = 80;      // aeff: fiber effective area [um^2] of the fundamental mode
    double pmdParameter      = 0;       // Polarization-mode dispersion (PMD) parameter [ps/sqrt(km)] in strong-coupling mode.
    double beatLength        = 20;      // Beat length [m]
    double nWavePlates       = 100;     // Number of waveplates when birefringence is on
    double maxStepLength     = length;  // max. step [m]. Default: fiber.length.
    double chlambda          = 0;       // save for later, because E will be canceled

    double accuracyParameter = 20;      // Accuracy parameter (dphimax)
    double alphaLinear       = 0;       // Nonlinear step parameter
    double bandwidth         = 0;       // Bandwidth [GHz] for the step size set-up. Usually the bandwidth of the propagating wavelength-division multiplexing (WDM) signal.
                                        // Default: the simulation bandwidth, GSTATE.FSAMPLING (See INIGSTATE).
    double gam     = 0;                 // Nonlinear parameter
    bool isUnique  = true;              // true: unique-field propagation, false: separate-field propagation
    bool isKerr    = false;             // If Kerr effect is active
    bool isDual    = false;             // true: dual polarization
    bool isManakov = false;             // true: nonlinear Kerr effect is modeled by the Manakov equation [Mar97, Ant16, Mum14].
                                        // false: solve the coupled-NLSE (CNLSE). Default: false.
    bool isCle   = true;                // constant local error (CLE)
    bool isSym   = false;               // Symmetric step computation
    bool trace   = false;               // true: print SSFM information on the screen. Default: false.
    bool dphiFwm = true;                // true: set the first step according to the maximum four-wave mixing (FWM) phase criterion [Mus18].
                                        // false: set the first step according to the maximum nonlinear phase criterion [Sin03,Mus18].
                                        // Default: true.
    string coupling = "none";           // Polarization coupling mode. It can be 'none' for no coupling, or 'pol' for strong polarization coupling.
                                        // The default value depends on the PMD parameter and the Kerr effect.
    string stepUpdate = "cle";          // Step update rule. Sets the updating rule for all steps following the first one.
                                        // It can be 'nlp' (nonlinear phase criterion, [Sin03]) or 'cle' (constant local error criterion, [Zha05,Zha08]).
                                        // Default: 'cle'.
    string stepType;                    // step type. It can be 'asymm' (asymmetric step) or 'symm' (symmetric step) [Sha14]. Default: 'asymm'.
};

/**
 * Single-mode optical fiber in the nonlinear regime
 * FIBER(E,Fiber) solves the nonlinear Schroedinger equation (NLSE). E is the
 * propagating electric field, such that E.fieldx are the time-samples of
 * the x-polarization,  E.fieldy the samples of the y-polarization (if
 * existing), and E.lambda is the carrier wavelength.
 * @param e: a comb of constant waves of powers PTX [mW] at wavelengths LAM [nm]. Such waves are stored in the struct E of fields
 * @param fiber: contains fiber parameters. It is a structure of fields
 * @return
 */

}  // namespace SimuLib

#endif  // OPTICALAB_FIBER_TYPES_H
