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
 * Created: 2022/3/17
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>


/**
 * @brief modulates the optical field E with the electric signal MODSIG by a Mach-Zehnder interferometer.
 * @param light: optical field, is a struct of fields lambda, field.
 * @param MODSIG: the electrical driving signal
 * @return E_multimode: a struct of wave, details in laserSource.h
 */
E mzmodulator(E light, VectorXcd modSig) {
    double biasl   = -1;                     // bias of lower arm
    double biasu   = -1;                     // bias of upper arm
    double exratio = INT_MAX;                // extinction ratio
    int mode       = Mzoption::push_pull;    // 0: push-pull. 1: push-push
    double vpi     = M_PI / 2 * (mode + 1);  // voltage of phase shift pi
    double normf   = 1;                      // normalization factor
    int nch        = 1;                      // number of channels (wavelengths)

    // Set-up extinction ratio
    double invexr_lin = pow(10, -exratio / 10);
    double gamma      = (1 - sqrt(invexr_lin)) / (1 + sqrt(invexr_lin));

    VectorXd modSigReal = modSig.real();  // signal must be real
    VectorXd phi_u      = M_PI / 2 * (modSigReal + VectorXd(modSigReal.size()).setConstant(biasu * vpi)) / vpi;
    VectorXd phi_l      = M_PI / 2 * (modSigReal + VectorXd(modSigReal.size()).setConstant(biasl * vpi)) / vpi;

    int Npol = 2;
    if (light.field.cols() == light.lambda.cols())
        Npol = 1;

    // Now set polarizations in alternate way
    VectorXi ncols(Npol * nch - Npol * (nch - 1));
    ncols.setLinSpaced(Npol * (nch - 1) + 1, Npol * nch);

    // now modulation only on the existing polarizations
    for (int i = 0; i < Npol; i++) {
        int np = ncols(i);
        for (int j = 0; j < light.field.rows(); j++) {
            if (light.field(j, np - 1) != complex<double>(0, 0)) {
                light.field.col(np - 1) = normf * light.field.col(np - 1).cwiseProduct((fastExp(phi_u) + gamma * fastExp(phi_l)) / (1 + gamma));
                break;
            }
        }
    }
    return light;
}

/**
 * @brief modulates the optical field E with the electric signal MODSIG by a Mach-Zehnder interferometer.
 * @param light: optical field, is a struct of fields lambda, field.
 * @param MODSIG: the electrical driving signal
 * @param option: compute option. Details in mzmodulator.h
 * @return E_multimode: a struct of wave, details in laserSource.h
 */
E mzmodulator(E light, VectorXcd modSig, Mzoption options) {
    double biasl   = -1;                     // bias of lower arm
    double biasu   = -1;                     // bias of upper arm
    double exratio = INT_MAX;                // extinction ratio
    int mode       = Mzoption::push_pull;    // 0: push-pull. 1: push-push
    double vpi     = M_PI / 2 * (mode + 1);  // voltage of phase shift pi
    double normf   = 1;                      // normalization factor
    int nch        = 1;                      // number of channels (wavelengths)

    if (light.lambda.size() < options.nch)
        ERROR("Channel index does not exist");
    if (light.lambda.size() > 1)
        nch = options.nch;
    if (options.vpi != INT_MAX)
        vpi = options.vpi;
    if (options.bias != INT_MAX) {
        biasl = options.bias;
        biasu = options.bias;
    } else {
        if (options.biasl != INT_MAX)
            biasl = options.biasl;
        if (options.biasu != INT_MAX)
            biasu = options.biasu;
    }
    mode = options.mode;
    if (options.norm != INT_MAX)
        normf = options.norm;

    // Set-up extinction ratio
    double invexr_lin = pow(10, -exratio / 10);
    double gamma      = (1 - sqrt(invexr_lin)) / (1 + sqrt(invexr_lin));

    VectorXd modSigReal = modSig.real();  // signal must be real
    VectorXd phi_u;
    VectorXd phi_l;
    if (mode == Mzoption::push_push) {
        phi_u = M_PI * (modSigReal + VectorXd(modSigReal.size()).setConstant(biasu * vpi)) / vpi;
        phi_l = M_PI * (modSigReal + VectorXd(modSigReal.size()).setConstant(biasl * vpi)) / vpi;
    } else {
        phi_u = M_PI / 2 * (modSigReal + VectorXd(modSigReal.size()).setConstant(biasu * vpi)) / vpi;
        phi_l = M_PI / 2 * (modSigReal + VectorXd(modSigReal.size()).setConstant(biasl * vpi)) / vpi;
    }

    int Npol = 2;
    if (light.field.cols() == light.lambda.cols())
        Npol = 1;

    // Now set polarizations in alternate way
    VectorXi ncols(Npol * nch - Npol * (nch - 1));
    ncols.setLinSpaced(Npol * (nch - 1) + 1, Npol * nch);

    // now modulation only on the existing polarizations
    for (int i = 0; i < Npol; i++) {
        int np = ncols(i);
        for (int j = 0; j < light.field.rows(); j++) {
            if (light.field(j, np - 1) != complex<double>(0, 0)) {
                light.field.col(np - 1) = normf * light.field.col(np - 1).cwiseProduct((fastExp(phi_u) + gamma * fastExp(phi_l)) / (1 + gamma));
                break;
            }
        }
    }

    return light;
}
