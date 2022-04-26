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
 * Created: 2022/3/11
 * Supported by: National Key Research and Development Program of China
 */


#include "Internal"
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std;

namespace SimuLib {

namespace HARDWARE_TYPE {

tuple<Linear *, double> CheckFiber(const E &e, Fiber &fiber);
tuple<double, double> FirstStep(const MatrixXcd &field, Fiber fiber);
double NextStep(const MatrixXcd &field, const Fiber &fiber, double dz_old);
tuple<RowVectorXd, RowVectorXd> CheckStep(double zprop, double dz, double len_corr);
MatrixXcd LinearStep(Linear *linear, VectorXd betat, RowVectorXd dzb, RowVectorXd nindex, MatrixXcd field);
VectorXcd NonlinearStep(VectorXcd field, Fiber fiber, double dz);
tuple<double, unsigned long, E> SSFM(E e, Linear *linear, const VectorXd &betat, Fiber fiber);

/**
 * @brief Single-mode optical fiber in the nonlinear regime
 * @param e: electric field, is a struct of fields.
 *        E.lambda: central wavelength [nm] of the electric field, i.e.,
 *          wavelength that relates the lowpass equivalent signal to the
 *          corresponding bandpass signal.
 *        E.field: time samples of the electric field, with polarizations (if
 *          existing) alternated on columns
 * @param fiber: the transmit fiber.
 * @return out: fiber option
 * @return e: electric field
 */

tuple<Out, E> fiberTransmit(E &e, Fiber fiber) {

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();  // Start time

    double dgd;
    Linear *linear;
    tie(linear, dgd) = CheckFiber(e, fiber);

    /************************ SET-UP PARAMETERS ************************/

    fiber.alphaLinear = (log(10) * (1e-4)) * fiber.attenuation;  // [m^-1]

    /******* Linear Parameters *******/

    VectorXd omega = gstate.FN;  // angular frequency [rad/ns]
    omega *= 2 * M_PI;

    double b0 = 0;  // Phase reference of propagation constant
    double b1 = 0;  // Retarded time frame

    // beta2 [ns^2/m] @ fiber.lambda
    double b2 = -pow(fiber.lambda, 2) / 2 / M_PI / LIGHT_SPEED * fiber.dispersion *
                1e-6;
    // beta3 [ns^3/m] @ fiber.lambda
    double b3 = pow(fiber.lambda / 2 / M_PI / LIGHT_SPEED, 2) * (2 * fiber.lambda * fiber.dispersion + pow(fiber.lambda, 2) * fiber.slope) * (1e-6);
    MatrixXd betat;
    Index omega_size = omega.size();

    double domega_i  = 2 * M_PI * LIGHT_SPEED * (1. / e.lambda(0, 0) - 1. / fiber.lambda);  // [1/ns]
    unsigned int ipm = 0;
    if (fiber.isUnique) {                    // Unique field
        double beta1  = b1 + dgd / 2;        // [ns/m] @ E.lambda
        double beta2  = b2 + b3 * domega_i;  // beta2 [ns^2/m] @ E.lambda
        double beta3  = b3;                  // [ns^3/m] @ E.lambda
        VectorXd unit = VectorXd::Ones(omega_size);
        betat.resize(omega_size, 1);
        betat.col(ipm) = omega.cwiseProduct((omega.cwiseProduct(omega * beta3 / 6 + unit * beta2 / 2)) + unit * beta1);

        // betat: deterministic beta coefficient [1/m]
        if (fiber.isDual) {  // Add DGD on polarizations
            betat.conservativeResize(omega_size, 2);
            betat.col(ipm + 1) = betat.col(ipm) - dgd / 2 * omega;
        }
    } else {                                         // Separate field
        double freq = LIGHT_SPEED / e.lambda(0, 0);  // carrier frequencies [GHz]
        // Separate field 情况还没有实现
    }

    /******* Nonlinear Parameters *******/

    double gam;
    if (!fiber.isKerr) {
        gam = 0;
    } else {
        gam = (2 * M_PI * fiber.nonlinearIndex) / (e.lambda(0, 0) * fiber.effectiveArea * 1e18);  // Nonlinear coeff [1/mW/m]
        if (isinf(gam))
            ERROR("Cannot continue: not finite nonlinear Kerr coefficient.");
    }

    double coeff;
    if (fiber.isManakov) {  // Manakov
        if (fiber.isDual) {
            coeff = 8. / 9;
        } else {
            coeff = 1;  // Not a Manakov actually, but same model
        }
    } else {  // CNLSE
        coeff = 1;
    }

    fiber.gam = coeff * gam;  // [1/mW/m], including Manakov correction, if active

    if ((fiber.dispersion == 0 && fiber.slope == 0) || fiber.gam == 0) {  // only GVD or only Kerr
        fiber.accuracyParameter = INFINITY;
        fiber.maxStepLength     = fiber.length;
    }

    /********** SSFM Propagation **********/

    double first_dz;
    unsigned long ncycle;

    tie(first_dz, ncycle, e) = SSFM(e, linear, betat, fiber);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();  // End time

    long long duration_ms = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    double time           = (double) duration_ms / 1000;

    Out out = {.time = time, .firstStepLength = first_dz, .nCycle = ncycle};
    return make_tuple(out, e);
}

tuple<double, unsigned long, E> SSFM(E e, Linear *linear, const VectorXd &betat, Fiber fiber) {
    if (fiber.trace) {
        cout << "Stepupd      step #   z [m]" << endl;
    }
    fiber.chlambda       = e.lambda(0, 0);
    unsigned long ncycle = 1;                                 // number of steps
    double len_corr      = fiber.length / fiber.nWavePlates;  // waveplate length [m]
    double dz, phimax, dzs;

    tie(dz, phimax)   = FirstStep(e.field, fiber);
    double half_alpha = 0.5 * fiber.alphaLinear;  // [1/m]
    double first_dz   = dz;
    double zprop      = dz;  // running distance [m]

    RowVectorXd dzb, nindex;
    if (fiber.isSym) {  // first half LIN step outside the cycle
        tie(dzb, nindex) = CheckStep(dz / 2, dz / 2, len_corr);
        e.field          = LinearStep(linear, betat, dzb, nindex, e.field);  // Linear step
    } else {
        dzs = 0;  // symmetric step contribution.
    }
    while (zprop < fiber.length) {                    // all steps except the last
        e.field = NonlinearStep(e.field, fiber, dz);  // Nonlinear step
        e.field = e.field * exp(-half_alpha * dz);    // Linear step 1/2: attenuation (scalar)
        double hlin;
        if (fiber.isSym) {
            dzs = NextStep(e.field, fiber, dz);
            if (zprop + dzs > fiber.length)
                dzs = fiber.length - zprop;  // needed in case of last step
            hlin = (dz + dzs) / 2;
        } else {
            hlin = dz;
        }

        // Linear step 2/2: GVD + birefringence
        tie(dzb, nindex) = CheckStep(zprop + dzs / 2, hlin, len_corr);       // zprop+dzs/2: end of step
        e.field          = LinearStep(linear, betat, dzb, nindex, e.field);  // Linear step
        if (fiber.isSym) {
            swap(dz, dzs);  // exchange dz and dzs
        } else {
            dz = NextStep(e.field, fiber, dz);
        }

        zprop += dz;
        ncycle++;
    }

    double last_step = fiber.length - zprop + dz;
    zprop            = zprop - dz + last_step;
    double hlin      = fiber.isSym ? last_step / 2 : last_step;

    // Last Nonlinear step
    if (fiber.gam != 0)
        e.field = NonlinearStep(e.field, fiber, last_step);

    e.field = e.field * exp(-half_alpha * last_step);

    // Last Linear step: GVD + birefringence
    tie(dzb, nindex) = CheckStep(fiber.length, hlin, len_corr);

    e.field        = LinearStep(linear, betat, dzb, nindex, e.field);
    e.lambda(0, 0) = fiber.chlambda;
    return make_tuple(first_dz, ncycle, e);
}

tuple<double, double> FirstStep(const MatrixXcd &field, Fiber fiber) {
    double step;
    double phimax;
    if (fiber.length == fiber.maxStepLength) {
        step   = fiber.maxStepLength;
        phimax = INFINITY;
    } else {
        if (fiber.dphiFwm) {
            fiber.bandwidth = gstate.SAMP_FREQ;
            double spac     = fiber.bandwidth * pow(fiber.chlambda, 2) / LIGHT_SPEED;  // bandwidth in [nm]
            step            = (fiber.accuracyParameter / abs(fiber.dispersion) / (2 * M_PI * spac * fiber.bandwidth * 1e-3) * 1e3);
            if (step > fiber.maxStepLength)
                step = fiber.maxStepLength;
            if (fiber.stepUpdate == "nlp") {                               // nonlinear phase criterion
                double invLnl = field.cwiseAbs2().maxCoeff() * fiber.gam;  // Max of 1/Lnl [1/m]
                double leff;
                if (fiber.alphaLinear == 0)
                    leff = step;
                else
                    leff = (1 - std::exp(-fiber.alphaLinear * step)) / fiber.alphaLinear;
                phimax = invLnl * leff;  // recalculate max nonlinear phase [rad] per step
            } else {
                phimax = fiber.accuracyParameter;
            }
        } else {  // nonlinear phase criterion
            step   = NextStep(field, fiber, NAN);
            phimax = fiber.accuracyParameter;
        }
    }
    return make_tuple(step, phimax);
}

// NEXTSTEP step size setup for the SSFM algorithm DZ=NEXTSTEP(U,X,DZ_OLD)
// evaluates the step size of the SSFM. U is the electric field,
// DZ_OLD is the step used in the previous SSFM cycle.

double NextStep(const MatrixXcd &field, const Fiber &fiber, double dz_old) {
    double step;
    if (fiber.isCle) {  // constant local error (CLE)
        double q = fiber.isSym ? 3 : 2;
        step     = dz_old * std::exp(fiber.alphaLinear / q * dz_old);  // [m]

    } else {  // nonlinear phase criterion
        double pmax;
        if (fiber.isDual) {  // max over time
            // 存在偏振的情况，还未实现
            // Pmax = max(abs(u(:,1:2:end)).^2 + abs(u(:,2:2:end)).^2);
        } else {
            pmax = field.cwiseAbs2().maxCoeff();
        }
        double invLnl = pmax * fiber.gam;                  // max over channels
        double leff   = fiber.accuracyParameter / invLnl;  // effective length [m] of the step
        double dl     = fiber.alphaLinear * leff;          // ratio effective length/attenuation length
        if (dl >= 1) {
            step = fiber.maxStepLength;  // [m]
        } else {
            if (fiber.alphaLinear == 0) {
                step = leff;
            } else {
                step = -1 / fiber.alphaLinear * std::log(1 - dl);
            }
        }
    }
    double dz;
    if (step > fiber.maxStepLength)
        dz = fiber.maxStepLength;
    else
        dz = step;
    return dz;
}

tuple<RowVectorXd, RowVectorXd> CheckStep(double zprop, double dz, double len_corr) {
    double zini = zprop - dz;  // starting coordinate of the step [m]
    double zend = zprop;       // ending coordinate of the step [m]

    // first waveplate index is 1
    double nini      = floor(zini / len_corr) + 1;  // Waveplate of starting coordinate
    double nend      = ceil(zend / len_corr);       // waveplate of ending coordinate
    RowVectorXd nmid = genVector(nini, nend);       // waveplate indexes

    RowVectorXd dz_split;

    if (nini == nend) {  // start/end of the step within a waveplate
        dz_split.resize(1);
        dz_split << dz;
    } else {  // multi-waveplate step

        // waveplate mid-coordinates
        RowVectorXd zmid      = len_corr * nmid.block(0, 0, 1, nmid.size() - 1);
        RowVectorXd diff_zmid = diff(zmid.transpose());
        dz_split.resize(2 + diff_zmid.size());
        dz_split << zmid(0) - zini, diff_zmid, zend - zmid(zmid.size() - 1);
    }
    RowVectorXd dzb    = removeZero(dz_split, dz_split);  // remove zero-length steps
    RowVectorXd nindex = removeZero(nmid, dz_split);      // remove zero-length steps
    return make_tuple(dzb, nindex);
}

MatrixXcd LinearStep(Linear *linear, VectorXd betat, RowVectorXd dzb, RowVectorXd nindex, MatrixXcd field) {
    field = fftCol(field);

    if (linear->is_scalar) {
        auto *scalar_linear = (ScalarLinear *) linear;
        for (Index i = 0; i < dzb.size(); ++i) {  // the step is made of multi-waveplates
            VectorXcd temp = fastExp((-betat) * dzb[i]);
            for (Index j = 0; j < field.cols(); ++j) {
                field.col(i) = field.col(i).cwiseProduct(temp);
            }
        }
    } else {
        // Linear非标量的情况还未实现
    }
    return ifftCol(field);
}

VectorXcd NonlinearStep(VectorXcd field, Fiber fiber, double dz) {
    double leff;
    if (fiber.alphaLinear == 0)
        leff = dz;
    else
        leff = (1 - exp(-fiber.alphaLinear * dz)) / fiber.alphaLinear;  // effective length [m] of dz
    double gamleff = fiber.gam * leff;                                  // [1/mW]
    if (fiber.isUnique) {                                               // UNIQUE FIELD
        VectorXd phi       = field.cwiseAbs2() * gamleff;               // nl phase [rad].
        VectorXcd expi_phi = fastExp(-phi);
        field              = field.cwiseProduct(expi_phi);  // expiphi .* u

        // dual-polarization未完成
    } else {  // separate-field
        // 未完成
    }
    return field;
}

tuple<Linear *, double> CheckFiber(const E &e, Fiber &fiber) {
    const string DEF_STEP_UPDATE = "cle";  // Default step-updating rule
    const bool DEF_IS_SYMMETRIC  = false;  // Default step computation
    const double DEF_PHI_FWM_CLE = 20;     // Default X.dphimax [rad] for CLE stepupd rule
    const double DEF_PHI_FWM_NLP = 4;      // Default X.dphimax [rad] for NLP & X.dphi1fwm
    const double DEF_PHI_NLP     = 0.003;  // Default X.dphimax [rad] for NLP & ~X.dphi1fwm

    // Check if Kerr effect is active
    if (isinf(fiber.effectiveArea) || (fiber.nonlinearIndex == 0)) {
        fiber.isKerr = false;
    } else {
        fiber.isKerr = true;
    }

    // Set-up coupling
    if (fiber.coupling != "none" && fiber.coupling != "pol") {
        ERROR(R"(Coupling must be "none" or "pol".)");
    }

    if (!fiber.isDual) {  // Scalar propagation
        if (fiber.isManakov)
            WARNING("Cannot use Manakov equation in scalar propagation: forced to NLSE.");
        fiber.isManakov = false;  // Because of no birefringence in scalar case
        if (fiber.coupling != "none")
            WARNING("Coupling not possible in scalar propagation");
        if (fiber.pmdParameter != 0)
            WARNING("PMD does not exist in scalar propagation: set to 0.");
        fiber.pmdParameter = 0;
        fiber.beatLength   = 0;
        fiber.coupling     = "none";  // Coupling impossible with one pol
    } else {                          // Dual-polarization
        if (fiber.isManakov) {
            if (fiber.pmdParameter == 0) {
                fiber.coupling = "none";  // Coupling indeed makes a difference for the Kerr effect of CNLSE
            } else {
                if (fiber.coupling == "none") {
                    WARNING("No coupling but PMD ~= 0.");
                } else {
                    fiber.coupling = "pol";
                }
            }
        } else {  // CNLSE
            fiber.isManakov = false;
            if (!fiber.isKerr && fiber.pmdParameter == 0) {
                fiber.coupling = "none";
            } else {
                fiber.coupling = "pol";
            }
        }
    }

    Linear *linear;
    double dgd;

    if (fiber.coupling == "pol") {                                                                         // X-Y coupling
        double corr_len = fiber.length / fiber.nWavePlates;                                                // waveplate length [m] (aka correlation length)
        dgd             = fiber.pmdParameter / sqrt(corr_len) * sqrt(3 * M_PI / 8) / sqrt(1000) * (1e-3);  // Differential
        // Group delay (DGD) per unit length [ns/m] @ x.lambda within a
        // Waveplate. To get [Df00] remember that within a waveplate the delay is dgd * corr_len.

        // 这两行没有替换
        //        x1.coupling = x.coupling;
        //        x1.beatlength = x.beatlength;
        for (int i = 0; i < fiber.nWavePlates; ++i) {  // SVD, hence different with the old FIBER version
                                                       //            [U,S]=eigendec(x1);
                                                       //            [lin.matin(:,:,l),lin.db0(l,:)] = deal(U,S);
        }
        // lin.db extended later
    } else {
        dgd                              = 0;  // Turn off all polarization and birefringence effects
        fiber.nWavePlates                = 1;
        fiber.coupling                   = "none";
        linear                           = new ScalarLinear();
        linear->is_scalar                = true;
        ((ScalarLinear *) linear)->matin = 1;
        ((ScalarLinear *) linear)->db    = 0;
    }
    linear->is_unique = fiber.isUnique;

    // SSFM checks

    // Maximum step
    if (fiber.maxStepLength > fiber.length)
        fiber.maxStepLength = fiber.length;

    if (fiber.isKerr) {
        if (fiber.stepUpdate.empty()) {
            fiber.stepUpdate = "cle";
        }
        if (fiber.stepUpdate == "cle") {
            if (!fiber.dphiFwm) {
                ERROR("The combination fiber.dphiFwm=false and fiber.stepUpdate=\"cle\" is not possible");
            }
            fiber.isCle = true;
        } else {
            fiber.isCle = false;
        }
        // First step parameter value
        if (fiber.dphiFwm) {
            if (fiber.stepUpdate == "cle") {
                fiber.accuracyParameter = DEF_PHI_FWM_CLE;  // CLE
            } else {
                fiber.accuracyParameter = DEF_PHI_FWM_NLP;  // NLP + PhiFWM in 1st step
            }
            if (fiber.bandwidth == 0) {
                fiber.accuracyParameter = fiber.accuracyParameter * 2.25;  // because in [Mus18] they used the signal bandwidth, x1.5 smaller than the simulation bandwidth.
            }
        } else {
            fiber.accuracyParameter = DEF_PHI_NLP;  // NLP
        }
    }

    // Step type
    if (!fiber.stepType.empty()) {
        if (fiber.stepType == "asymm") {
            fiber.isSym = false;
        } else if (fiber.stepType == "symm") {
            fiber.isSym = true;
        } else {
            ERROR("Wrong step type.");
        }
    } else {
        fiber.isSym = DEF_IS_SYMMETRIC;
    }
    return make_tuple(linear, dgd);
}

}

}  // namespace SimuLib