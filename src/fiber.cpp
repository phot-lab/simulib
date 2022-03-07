#include "Dense"
#include "common_types.h"
#include "fiber_tools.h"
#include "fiber_types.h"
#include "globals.h"
#include "tools.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace Eigen;

void init_fiber(E e, Fiber &fiber, double &dgd);

tuple<double, double> first_step(VectorXcd field, Fiber fiber);

void fiber_transmit(E &e, Fiber fiber) {

    unsigned Nsymb    = 1024;  // number of symbols
    unsigned Nt       = 32;    // number of discrete points per symbol
    unsigned symbrate = 10;    // symbol rate [Gbaud].

    unsigned Nsamp = Nsymb * Nt;     // overall number of samples
    unsigned fs    = symbrate * Nt;  // sampling rate [GHz]
    InitGstate(gstate, Nsamp, fs);   // initialize global variables: Nsamp and fs.

    // timeini = tic; 开始计时

    size_t Nfft = e.field.size();
    double dgd;
    init_fiber(e, fiber, dgd);

    /************************ SET-UP PARAMETERS ************************/

    fiber.alpha_lin = (log(10) * (1e-4)) * fiber.attenuation;  // [m^-1]

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

    double domega_i  = 2 * M_PI * LIGHT_SPEED * (1. / e.lambda - 1. / fiber.lambda);  // [1/ns]
    unsigned int ipm = 0;
    if (fiber.is_unique) {                   // Unique field
        double beta1  = b1 + dgd / 2;        // [ns/m] @ E.lambda
        double beta2  = b2 + b3 * domega_i;  // beta2 [ns^2/m] @ E.lambda
        double beta3  = b3;                  // [ns^3/m] @ E.lambda
        VectorXd unit = VectorXd::Ones(omega_size);
        betat.resize(omega_size, 1);
        betat.col(ipm) = omega.cwiseProduct((omega.cwiseProduct(omega * beta3 / 6 + unit * beta2 / 2)) + unit * beta1);

        // betat: deterministic beta coefficient [1/m]
        if (fiber.is_dual) {  // Add DGD on polarizations
            betat.conservativeResize(omega_size, 2);
            betat.col(ipm + 1) = betat.col(ipm) - dgd / 2 * omega;
        }
    } else {                                   // Separate field
        double freq = LIGHT_SPEED / e.lambda;  // carrier frequencies [GHz]
        // 两个偏振方向的这个还没有实现
    }

    /******* Nonlinear Parameters *******/

    double gam;
    if (!fiber.is_kerr) {
        gam = 0;
    } else {
        gam = (2 * M_PI * fiber.nonlinear_index) / (e.lambda * fiber.effective_area * 1e18);  // Nonlinear coeff [1/mW/m]
        if (isinf(gam))
            ERROR("Cannot continue: not finite nonlinear Kerr coefficient.");
    }

    double coeff;
    if (fiber.is_manakov) {  // Manakov
        if (fiber.is_dual) {
            coeff = 8. / 9;
        } else {
            coeff = 1;  // Not a Manakov actually, but same model
        }
    } else {  // CNLSE
        coeff = 1;
    }

    fiber.gam = coeff * gam;  // [1/mW/m], including Manakov correction, if active

    if ((fiber.dispersion == 0 && fiber.slope == 0) || fiber.gam == 0) {  // only GVD or only Kerr
        fiber.dphi_max = INFINITY;
        fiber.dzmax    = fiber.length;
    }

    /********** SSFM Propagation **********/
}

// function [firstdz,ncycle,E]=ssfm(E,lin,betat,x)
void SSFM(E e, Linear *linear, VectorXd betat, Fiber fiber) {
    if (fiber.trace) {
        cout << "Stepupd      step #   z [m]" << endl;
    }
    fiber.chlambda       = e.lambda;
    unsigned long ncycle = 1;                             // number of steps
    double len_corr      = fiber.length / fiber.nplates;  // waveplate length [m]
    double step, phimax;
    tie(step, phimax) = first_step(e.field, fiber);
}

tuple<double, double> first_step(VectorXcd field, Fiber fiber) {
    double step;
    double phimax;
    if (fiber.length == fiber.dzmax) {
        step   = fiber.dzmax;
        phimax = INFINITY;
    } else {
        if (fiber.dphi_fwm) {
            fiber.bandwidth = gstate.SAMP_FREQ;
            double spac     = fiber.bandwidth * pow(fiber.chlambda, 2) / LIGHT_SPEED;  // bandwidth in [nm]
            step            = (fiber.dphi_max / abs(fiber.dispersion) / (2 * M_PI * spac * fiber.bandwidth * 1e-3) * 1e3);
            if (step > fiber.dzmax)
                step = fiber.dzmax;
            if (fiber.step_update == "nlp") {                              // nonlinear phase criterion
                double invLnl = field.cwiseAbs2().maxCoeff() * fiber.gam;  // Max of 1/Lnl [1/m]
                double leff;
                if (fiber.alpha_lin == 0)
                    leff = step;
                else
                    leff = (1 - std::exp(-fiber.alpha_lin * step)) / fiber.alpha_lin;
                phimax = invLnl * leff;  // recalculate max nonlinear phase [rad] per step
            } else {
                phimax = fiber.dphi_max;
            }
        } else {  // nonlinear phase criterion
            // 没有实现
            //            step = nextstep(u,x,NaN);
            phimax = fiber.dphi_max;
        }
    }
    return make_tuple(step, phimax);
}

// NEXTSTEP step size setup for the SSFM algorithm DZ=NEXTSTEP(U,X,DZ_OLD)
// evaluates the step size of the SSFM. U is the electric field,
// DZ_OLD is the step used in the previous SSFM cycle.

void next_step(VectorXcd field, Fiber fiber, double dz_old) {

}


void init_fiber(E e, Fiber &fiber, double &dgd) {
    const string DEF_STEP_UPDATE = "cle";  // Default step-updating rule
    const bool DEF_IS_SYMMETRIC  = false;  // Default step computation
    const double DEF_PHI_FWM_CLE = 20;     // Default X.dphimax [rad] for CLE stepupd rule
    const double DEF_PHI_FWM_NLP = 4;      // Default X.dphimax [rad] for NLP & X.dphi1fwm
    const double DEF_PHI_NLP     = 0.003;  // Default X.dphimax [rad] for NLP & ~X.dphi1fwm

    // Check if Kerr effect is active
    if (isinf(fiber.effective_area) || (fiber.nonlinear_index == 0)) {
        fiber.is_kerr = false;
    } else {
        fiber.is_kerr = true;
    }

    // Set-up coupling
    if (fiber.coupling != "none" && fiber.coupling != "pol") {
        ERROR(R"(Coupling must be "none" or "pol".)");
    }

    if (!fiber.is_dual) {  // Scalar propagation
        if (fiber.is_manakov)
            WARNING("Cannot use Manakov equation in scalar propagation: forced to NLSE.");
        fiber.is_manakov = false;  // Because of no birefringence in scalar case
        if (fiber.coupling != "none")
            WARNING("Coupling not possible in scalar propagation");
        if (fiber.pmdpar != 0)
            WARNING("PMD does not exist in scalar propagation: set to 0.");
        fiber.pmdpar      = 0;
        fiber.beat_length = 0;
        fiber.coupling    = "none";  // Coupling impossible with one pol
    } else {                         // Dual-polarization
        if (fiber.is_manakov) {
            if (fiber.pmdpar == 0) {
                fiber.coupling = "none";  // Coupling indeed makes a difference for the Kerr effect of CNLSE
            } else {
                if (fiber.coupling == "none") {
                    WARNING("No coupling but PMD ~= 0.");
                } else {
                    fiber.coupling = "pol";
                }
            }
        } else {  // CNLSE
            fiber.is_manakov = false;
            if (!fiber.is_kerr && fiber.pmdpar == 0) {
                fiber.coupling = "none";
            } else {
                fiber.coupling = "pol";
            }
        }
    }

    Linear *linear;

    if (fiber.coupling == "pol") {                                                                   // X-Y coupling
        double corr_len = fiber.length / fiber.nplates;                                              // waveplate length [m] (aka correlation length)
        dgd             = fiber.pmdpar / sqrt(corr_len) * sqrt(3 * M_PI / 8) / sqrt(1000) * (1e-3);  // Differential
        // Group delay (DGD) per unit length [ns/m] @ x.lambda within a
        // Waveplate. To get [Df00] remember that within a waveplate the delay is dgd * corr_len.

        // 这两行没有替换
        //        x1.coupling = x.coupling;
        //        x1.beatlength = x.beatlength;
        for (int i = 0; i < fiber.nplates; ++i) {  // SVD, hence different with the old FIBER version
                                                   //            [U,S]=eigendec(x1);
                                                   //            [lin.matin(:,:,l),lin.db0(l,:)] = deal(U,S);
        }
        // lin.db extended later
    } else {
        dgd                              = 0;  // Turn off all polarization and birefringence effects
        fiber.nplates                    = 1;
        fiber.coupling                   = "none";
        linear                           = new ScalarLinear();
        linear->is_scalar                = true;
        ((ScalarLinear *) linear)->matin = 1;
        ((ScalarLinear *) linear)->db    = 0;
    }
    linear->is_unique = fiber.is_unique;

    // SSFM checks

    // Maximum step
    if (fiber.dzmax > fiber.length)
        fiber.dzmax = fiber.length;

    if (fiber.is_kerr) {
        if (fiber.step_update.empty()) {
            fiber.step_update = "cle";
        }
        if (fiber.step_update == "cle") {
            if (!fiber.dphi_fwm) {
                ERROR("The combination fiber.dphi_fwm=false and fiber.step_update=\"cle\" is not possible");
            }
            fiber.is_cle = true;
        } else {
            fiber.is_cle = false;
        }
        // First step parameter value
        if (fiber.dphi_fwm) {
            if (fiber.step_update == "cle") {
                fiber.dphi_max = DEF_PHI_FWM_CLE;  // CLE
            } else {
                fiber.dphi_max = DEF_PHI_FWM_NLP;  // NLP + PhiFWM in 1st step
            }
            // 这里需要改改
            /**
            if ~isfield(x,'bandwidth')
                x.dphimax = x.dphimax*(1.5)^2; % because in [Mus18] they used
                % the signal bandwidth, x1.5 smaller than the simulation
                % bandwidth.
            end
             */
        } else {
            fiber.dphi_max = DEF_PHI_NLP;  // NLP
        }
    }

    // Step type
    if (!fiber.step_type.empty()) {
        if (fiber.step_type == "asymm") {
            fiber.is_sym = false;
        } else if (fiber.step_type == "symm") {
            fiber.is_sym = true;
        } else {
            ERROR("Wrong step type.");
        }
    } else {
        fiber.is_sym = DEF_IS_SYMMETRIC;
    }
}