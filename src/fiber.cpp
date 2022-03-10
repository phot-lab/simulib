#include <simulib>
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std;
using namespace Eigen;

tuple<Linear *, double> InitFiber(const E &e, Fiber &fiber);
tuple<double, double> FirstStep(VectorXcd field, Fiber fiber);
double NextStep(const VectorXcd &field, const Fiber &fiber, double dz_old);
tuple<RowVectorXd, RowVectorXd> CheckStep(double zprop, double dz, double len_corr);
VectorXcd LinearStep(Linear *linear, VectorXd betat, RowVectorXd dzb, RowVectorXd nindex, VectorXcd field);
VectorXcd NonlinearStep(VectorXcd field, Fiber fiber, double dz);
tuple<double, unsigned long, E> SSFM(E e, Linear *linear, const VectorXd &betat, Fiber fiber);

tuple<Out, E> FiberTransmit(E &e, Fiber fiber) {

    unsigned Nsymb    = 1024;           // number of symbols
    unsigned Nt       = 32;             // number of discrete points per symbol
    unsigned symbrate = 10;             // symbol rate [Gbaud].
    unsigned Nsamp    = Nsymb * Nt;     // overall number of samples
    unsigned fs       = symbrate * Nt;  // sampling rate [GHz]
    InitGstate(gstate, Nsamp, fs);      // initialize global variables: Nsamp and fs.

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();  // Start time

    double dgd;
    Linear *linear;
    tie(linear, dgd) = InitFiber(e, fiber);

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
        // Separate field 情况还没有实现
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

    double first_dz;
    unsigned long ncycle;
    tie(first_dz, ncycle, e) = SSFM(e, linear, betat, fiber);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();  // End time

    long long duration_ms = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    double time           = (double) duration_ms / 1000;

    Out out = {.time = time, .first_dz = first_dz, .ncycle = ncycle};
    return make_tuple(out, e);
}

tuple<double, unsigned long, E> SSFM(E e, Linear *linear, const VectorXd &betat, Fiber fiber) {
    if (fiber.trace) {
        cout << "Stepupd      step #   z [m]" << endl;
    }
    fiber.chlambda       = e.lambda;
    unsigned long ncycle = 1;                             // number of steps
    double len_corr      = fiber.length / fiber.nplates;  // waveplate length [m]
    double dz, phimax, dzs;

    tie(dz, phimax)   = FirstStep(e.field, fiber);
    double half_alpha = 0.5 * fiber.alpha_lin;  // [1/m]
    double first_dz   = dz;
    double zprop      = dz;  // running distance [m]

    RowVectorXd dzb, nindex;
    if (fiber.is_sym) {  // first half LIN step outside the cycle
        tie(dzb, nindex) = CheckStep(dz / 2, dz / 2, len_corr);
        e.field          = LinearStep(linear, betat, dzb, nindex, e.field);  // Linear step
    } else {
        dzs = 0;  // symmetric step contribution.
    }
    while (zprop < fiber.length) {                    // all steps except the last
        e.field = NonlinearStep(e.field, fiber, dz);  // Nonlinear step
        e.field = e.field * exp(-half_alpha * dz);    // Linear step 1/2: attenuation (scalar)
        double hlin;
        if (fiber.is_sym) {
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
        if (fiber.is_sym) {
            swap(dz, dzs);  // exchange dz and dzs
        } else {
            dz = NextStep(e.field, fiber, dz);
        }

        zprop += dz;
        ncycle++;
    }

    double last_step = fiber.length - zprop + dz;
    zprop            = zprop - dz + last_step;
    double hlin      = fiber.is_sym ? last_step / 2 : last_step;

    // Last Nonlinear step
    if (fiber.gam != 0)
        e.field = NonlinearStep(e.field, fiber, last_step);

    e.field = e.field * exp(-half_alpha * last_step);

    // Last Linear step: GVD + birefringence
    tie(dzb, nindex) = CheckStep(fiber.length, hlin, len_corr);
    e.field          = LinearStep(linear, betat, dzb, nindex, e.field);

    e.lambda = fiber.chlambda;
    return make_tuple(first_dz, ncycle, e);
}

tuple<double, double> FirstStep(VectorXcd field, Fiber fiber) {
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
            step   = NextStep(field, fiber, NAN);
            phimax = fiber.dphi_max;
        }
    }
    return make_tuple(step, phimax);
}

// NEXTSTEP step size setup for the SSFM algorithm DZ=NEXTSTEP(U,X,DZ_OLD)
// evaluates the step size of the SSFM. U is the electric field,
// DZ_OLD is the step used in the previous SSFM cycle.

double NextStep(const VectorXcd &field, const Fiber &fiber, double dz_old) {
    double step;
    if (fiber.is_cle) {  // constant local error (CLE)
        double q = fiber.is_sym ? 3 : 2;
        step     = dz_old * std::exp(fiber.alpha_lin / q * dz_old);  // [m]

    } else {  // nonlinear phase criterion
        double pmax;
        if (fiber.is_dual) {  // max over time
            // 存在偏振的情况，还未实现
            // Pmax = max(abs(u(:,1:2:end)).^2 + abs(u(:,2:2:end)).^2);
        } else {
            pmax = field.cwiseAbs2().maxCoeff();
        }
        double invLnl = pmax * fiber.gam;         // max over channels
        double leff   = fiber.dphi_max / invLnl;  // effective length [m] of the step
        double dl     = fiber.alpha_lin * leff;   // ratio effective length/attenuation length
        if (dl >= 1) {
            step = fiber.dzmax;  // [m]
        } else {
            if (fiber.alpha_lin == 0) {
                step = leff;
            } else {
                step = -1 / fiber.alpha_lin * std::log(1 - dl);
            }
        }
    }
    double dz;
    if (step > fiber.dzmax)
        dz = fiber.dzmax;
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
    RowVectorXd nmid = GenVector(nini, nend);       // waveplate indexes

    RowVectorXd dz_split;

    if (nini == nend) {  // start/end of the step within a waveplate
        dz_split.resize(1);
        dz_split << dz;
    } else {  // multi-waveplate step

        // waveplate mid-coordinates
        RowVectorXd zmid      = len_corr * nmid.block(0, 0, 1, nmid.size() - 1);
        RowVectorXd diff_zmid = Diff(zmid.transpose());
        dz_split.resize(2 + diff_zmid.size());
        dz_split << zmid(0) - zini, diff_zmid, zend - zmid(zmid.size() - 1);
    }
    RowVectorXd dzb    = RemoveZero(dz_split, dz_split);  // remove zero-length steps
    RowVectorXd nindex = RemoveZero(nmid, dz_split);      // remove zero-length steps
    return make_tuple(dzb, nindex);
}

VectorXcd LinearStep(Linear *linear, VectorXd betat, RowVectorXd dzb, RowVectorXd nindex, VectorXcd field) {
    field = FFT(field);

    if (linear->is_scalar) {
        auto *scalar_linear = (ScalarLinear *) linear;
        VectorXd unit       = VectorXd ::Ones(betat.size());
        for (Index i = 0; i < dzb.size(); ++i) {  // the step is made of multi-waveplates
            field = field.cwiseProduct(FastExp((-(betat + unit)) * dzb[i]));
        }
    } else {
        // Linear非标量的情况还未实现
    }

    return IFFT(field);
}

VectorXcd NonlinearStep(VectorXcd field, Fiber fiber, double dz) {
    double leff;
    if (fiber.alpha_lin == 0)
        leff = dz;
    else
        leff = (1 - exp(-fiber.alpha_lin * dz)) / fiber.alpha_lin;  // effective length [m] of dz
    double gamleff = fiber.gam * leff;                              // [1/mW]
    if (fiber.is_unique) {                                          // UNIQUE FIELD
        VectorXd phi       = field.cwiseAbs2() * gamleff;           // nl phase [rad].
        VectorXcd expi_phi = FastExp(-phi);
        field              = field.cwiseProduct(expi_phi);  // expiphi .* u

        // dual-polarization未完成
    } else {  // separate-field
        // 未完成
    }
    return field;
}


tuple<Linear *, double> InitFiber(const E &e, Fiber &fiber) {
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
    double dgd;

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
    return make_tuple(linear, dgd);
}