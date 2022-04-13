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

#include "SimuLib"

using namespace std;

MatrixXd getLambda(const double lamc, const double spac, const int Nch);

///**
// * @brief It is a ideal laser source module with single channel in dual-polarization. It would compute x-polarizations
// *        and y-polarizations in simulation. It is an ideal light source.
// * @param ptx: power of constant wave. [mw]
// * @param lam: wavelength of constant wave. Here odd columns represent x-polarizations samples,
// *             while the even columns the y-polarizations samples. [nm]
// * @return E_multimode: a struct of wave, details in fiber_types.h.
// */
//
// E laserSource(RowVectorXd ptx, RowVectorXd lam) {
//    unsigned long Nsamp = gstate.NSAMP;  // Sampling frequency
//    int Npow            = ptx.size();    // Number of the power of transmit channel
//    int Nch             = lam.size();    // Number of wavelength of transmit channel
//    E light;                    // transmit light
//
//    if (Npow > 1 && Npow != Nch)
//        ERROR("Powers and wavelengths must have the same length");
//
//    int Npol = Option::single;  // Number of polarizations
//
//    // Set light power.
//    RowVectorXd power;
//    if (Npow == 1) {
//        ptx.conservativeResize(Nch);
//        ptx.setConstant(ptx(0));
//        power = ptx;
//    } else
//        power = ptx;
//
//    // Compute every light wavelength and its time samples of the electric field
//    light.lambda = lam;
//    light.field  = MatrixXcd(Nsamp, Nch * Npol);
//    light.field.setZero();
//    for (int i = 0; i < Nch; i++)
//        light.field.col(i * Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));
//
//    return light;
//}
//
//
///**
// * @brief It is a laser source module with noise. It would compute in single-polarizations
// *        and dual-polarizations mode in simulation.
// * @param ptx: power of constant wave. [mw]
// * @param lam: wavelength of constant wave. Here odd columns represent x-polarizations samples,
// *             while the even columns the y-polarizations samples. [nm]
// * @param option: compute option. Details in laserSource.h
// * @return E_multimode: a struct of wave, details in fiber_types.h
// */
//
// E laserSource(RowVectorXd ptx, RowVectorXd lam, Option options) {
//    E light;                    // transmit light
//    unsigned long Nsamp = gstate.NSAMP;  // Sampling frequency
//    int Npow            = ptx.size();    // Number of the power of transmit channel
//    int Nch             = lam.size();    // Number of wavelength of transmit channel
//
//    if (Npow > 1 && Npow != Nch)
//        ERROR("Powers and wavelengths must have the same length");
//
//    double N0 = options.n0;
//    int Npol  = options.pol;  // Number of polarizations
//    RowVectorXd linewidth;
//    if (options.linewidth.size() == 1) {
//        linewidth = RowVectorXd(Nch);
//        linewidth.setConstant(options.linewidth(0));
//    } else if (options.linewidth.size() == Nch)
//        linewidth = options.linewidth;
//    else
//        ERROR("The linewidth must have the same length of the number of wavelengths.");
//
//    RowVectorXd power;
//    if (Npow == 1) {
//        ptx.conservativeResize(Nch);
//        ptx.setConstant(ptx(0));
//        power = ptx;
//    } else
//        power = ptx;
//
//    light.lambda = lam;
//    light.field  = MatrixXd::Zero(Nsamp, Nch * Npol);
//    for (int i = 0; i < Nch; i++)
//        light.field.col(i * Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));
//
//    // Add phase noise
//    // This part refer to 'freq_noise  = (ones(Nsamp,1) * sqrt(2*pi*linewidth./GSTATE.FSAMPLING)) .* randn( Nsamp, Nch)' in Optilux.
//    // New a vector named one(Nsamp,1) in equation above.
//    VectorXd oneVector(Nsamp);
//    oneVector.setOnes();
//    // New a matrix named randn( Nsamp, Nch), which follows standard normal distribution X(0,1).
//    static default_random_engine engine(time(0));
//    static normal_distribution<double> n(0, 1);
//    MatrixXd randMatrix = MatrixXd::Zero(Nsamp, Nch).unaryExpr([](double dummy) { return n(engine); });
//    MatrixXd freq_noise = linewidth * 2 * M_PI;
//    freq_noise          = freq_noise / gstate.SAMP_FREQ;
//    freq_noise          = freq_noise.cwiseSqrt();
//    freq_noise          = oneVector * freq_noise;
//    freq_noise          = freq_noise.cwiseProduct(randMatrix);
//    freq_noise(0, 0)    = 0;
//
//    MatrixXcd phase_noise = freq_noise;
//    for (int i = 0; i < phase_noise.rows() - 1; i++)
//        phase_noise(i + 1) += phase_noise(i);
//
//    VectorXd tim(Nsamp);
//    tim.setLinSpaced(0, Nsamp - 1);
//    phase_noise = phase_noise - ((tim / (Nsamp - 1)) * phase_noise.row(phase_noise.rows() - 1));
//    phase_noise = fastExp(phase_noise);
//    for (int i = 0; i < Nch - 1; i++)
//        light.field.col(i * Npol) = light.field.col(i * Npol).cwiseProduct(phase_noise.col(i * Npol));
//
//    // Add Gaussian complex white noise
//    double N0_lin        = pow(10, N0 / 10);
//    double sigma         = sqrt(N0_lin / 2 * gstate.SAMP_FREQ);
//    MatrixXd randMatrix1 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
//    MatrixXd randMatrix2 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
//    MatrixXcd randComplexMatrix(Nsamp, Nch * Npol);
//    randComplexMatrix.real() << randMatrix1;
//    randComplexMatrix.imag() << randMatrix2;
//    light.field = light.field + sigma * randComplexMatrix;
//
//    return light;
//}
/**
 * @brief It is a laser source module with multi-channel and noise. It would compute in single-polarizations
 *        and dual-polarizations mode in simulation.
 * @param ptx: power of constant wave. [mw]
 * @param lam: the central wavelength of the WDM comb. [nm]
 * @param spac: wavelength spacing [nm]
 * @param NLAMBDA: number of carriers.
 * @param option: compute option. Details in laserSource.h.
 * @return E: a struct of wave, details in fiber_types.h
 */
E laserSource(RowVectorXd ptx, RowVectorXd lam, Option options) {
    E light;                             // transmit light
    unsigned long Nsamp = gstate.NSAMP;  // Sampling frequency
    int Npow            = ptx.size();    // Number of the power of transmit channel
    int Nch             = lam.size();    // Number of carriers

    if (Npow > 1 && Npow != Nch)
        ERROR("Powers and wavelengths must have the same length");

    double N0             = options.n0;
    int Npol              = options.pol;  // Number of polarizations
    RowVectorXd linewidth = RowVectorXd(0);
    if (options.linewidth.size() != 0 && options.linewidth(0) != 0) {
        if (options.linewidth.size() == 1) {
            linewidth = RowVectorXd(Nch);
            linewidth.setConstant(options.linewidth(0));
        } else if (options.linewidth.size() == Nch)
            linewidth = options.linewidth;
        else
            ERROR("The linewidth must have the same length of the number of wavelengths.");
    }

    RowVectorXd power;
    if (Npow == 1)
        power = RowVectorXd(Nch).setConstant(ptx(0));
    else
        power = ptx;

    // uniformly spaced carriers
    light.lambda = lam;

    light.field = MatrixXd::Zero(Nsamp, Nch * Npol);
    // by default, fully polarized on x (odd columns):
    for (int i = 0; i < Nch; i++)
        light.field.col(i * Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));

    // Standard normal distribution X(0,1) random number generation engine.
    static default_random_engine engine(time(0));
    static normal_distribution<double> n(0, 1);

    // Add phase noise
    // This part refer to 'freq_noise  = (ones(Nsamp,1) * sqrt(2*pi*linewidth./GSTATE.FSAMPLING)) .* randn( Nsamp, Nch)' in Optilux.
    // New a vector named one(Nsamp,1) in equation above.
    if (linewidth.size() != 0) {
        VectorXd oneVector(Nsamp);
        oneVector.setOnes();
        // New a matrix named randn( Nsamp, Nch), which follows standard normal distribution X(0,1).
        MatrixXd randMatrix = MatrixXd::Zero(Nsamp, Nch).unaryExpr([](double dummy) { return n(engine); });
        MatrixXd freq_noise = linewidth * 2 * M_PI;
        freq_noise          = freq_noise / gstate.SAMP_FREQ;
        freq_noise          = freq_noise.cwiseSqrt();
        freq_noise          = oneVector * freq_noise;
        freq_noise          = freq_noise.cwiseProduct(randMatrix);
        freq_noise(0, 0)    = 0;

        MatrixXcd phase_noise = freq_noise;
        for (int i = 0; i < phase_noise.rows() - 1; i++)
            phase_noise(i + 1) += phase_noise(i);

        VectorXd tim(Nsamp);
        tim.setLinSpaced(0, Nsamp - 1);
        phase_noise = phase_noise - (tim / (Nsamp - 1)) * phase_noise.row(phase_noise.rows() - 1);
        phase_noise = fastExp(phase_noise);
        for (int i = 0; i < Nch; i++)
            light.field.col(i * Npol) = light.field.col(i * Npol).cwiseProduct(phase_noise.col(i * Npol));
    }

    // Add Gaussian complex white noise
    if (N0 != INT_MIN) {
        double N0_lin        = pow(10, N0 / 10);
        double sigma         = sqrt(N0_lin / 2 * gstate.SAMP_FREQ);
        MatrixXd randMatrix1 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
        MatrixXd randMatrix2 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
        MatrixXcd randComplexMatrix(Nsamp, Nch * Npol);
//        randComplexMatrix.real() << randMatrix1;
//        randComplexMatrix.imag() << randMatrix2;
        light.field = light.field + sigma * randComplexMatrix;
    }

    return light;
}
//
///**
// * @brief It is a laser source module with noise. It would compute in single-polarizations
// *        and dual-polarizations mode in simulation.
// * @param ptx: power of constant wave. [mw]
// * @param lam: the central wavelength of the WDM comb. [nm]
// * @param spac: wavelength spacing [nm]
// * @param NLAMBDA: number of carriers.
// * @return E_multimode: a struct of wave, details in fiber_types.h
// */
//
// E laserSource(RowVectorXd ptx, double lam, double spac, int NLAMBDA) {
//    E light;                    // transmit light
//    unsigned long Nsamp = gstate.NSAMP;  // Sampling frequency
//    int Npow            = ptx.size();    // Number of the power of transmit channel
//    int Nch             = NLAMBDA;       // Number of carriers
//
//    if (Npow > 1 && Npow != Nch)
//        ERROR("Powers and wavelengths must have the same length");
//
//    int Npol = Option::dual;  // Number of polarizations
//
//    RowVectorXd power;
//    if (Npow == 1) {
//        ptx.conservativeResize(Nch);
//        ptx.setConstant(ptx(0));
//        power = ptx;
//    } else
//        power = ptx;
//
//    light.lambda = getLambda(lam, spac, Nch);
//    light.field  = MatrixXd::Zero(Nsamp, Nch * Npol);
//    for (int i = 0; i < Nch; i++)
//        light.field.col(i * Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));
//
//    return light;
//}
//
///**
// * @brief It is a laser source module with multi-channel and noise. It would compute in single-polarizations
// *        and dual-polarizations mode in simulation.
// * @param ptx: power of constant wave. [mw]
// * @param lam: the central wavelength of the WDM comb. [nm]
// * @param spac: wavelength spacing [nm]
// * @param NLAMBDA: number of carriers.
// * @param option: compute option. Details in laserSource.h.
// * @return E: a struct of wave, details in fiber_types.h
// */
// E laserSource(RowVectorXd ptx, RowVectorXd lam, double spac, int NLAMBDA, Option options) {
//    E light;                    // transmit light
//    unsigned long Nsamp = gstate.NSAMP;  // Sampling frequency
//    int Npow            = ptx.size();    // Number of the power of transmit channel
//    int Nch             = NLAMBDA;       // Number of carriers
//
//    if (Npow > 1 && Npow != Nch)
//        ERROR("Powers and wavelengths must have the same length");
//
//    double N0 = options.n0;
//    int Npol  = options.pol;  // Number of polarizations
//    RowVectorXd linewidth;
//    if (options.linewidth.size() == 1) {
//        linewidth = RowVectorXd(Nch);
//        linewidth.setConstant(options.linewidth(0));
//    } else if (options.linewidth.size() == Nch)
//        linewidth = options.linewidth;
//    else
//        ERROR("The linewidth must have the same length of the number of wavelengths.");
//
//    RowVectorXd power;
//    if (Npow == 1) {
//        ptx.conservativeResize(Nch);
//        ptx.setConstant(ptx(0));
//        power = ptx;
//    } else
//        power = ptx;
//
//    light.lambda = getLambda(lam(0), spac, Nch);
//    light.field  = MatrixXd::Zero(Nsamp, Nch * Npol);
//    for (int i = 0; i < Nch; i++)
//        light.field.col(i * Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));
//
//    // Add phase noise
//    // This part refer to 'freq_noise  = (ones(Nsamp,1) * sqrt(2*pi*linewidth./GSTATE.FSAMPLING)) .* randn( Nsamp, Nch)' in Optilux.
//    // New a vector named one(Nsamp,1) in equation above.
//    VectorXd oneVector(Nsamp);
//    oneVector.setOnes();
//    // New a matrix named randn( Nsamp, Nch), which follows standard normal distribution X(0,1).
//    static default_random_engine engine(time(0));
//    static normal_distribution<double> n(0, 1);
//    MatrixXd randMatrix = MatrixXd::Zero(Nsamp, Nch).unaryExpr([](double dummy) { return n(engine); });
//    MatrixXd freq_noise = linewidth * 2 * M_PI;
//    freq_noise          = freq_noise / gstate.SAMP_FREQ;
//    freq_noise          = freq_noise.cwiseSqrt();
//    freq_noise          = oneVector * freq_noise;
//    freq_noise          = freq_noise.cwiseProduct(randMatrix);
//    freq_noise(0, 0)    = 0;
//
//    MatrixXcd phase_noise = freq_noise;
//    for (int i = 0; i < phase_noise.rows() - 1; i++)
//        phase_noise(i + 1) += phase_noise(i);
//
//    VectorXd tim(Nsamp);
//    tim.setLinSpaced(0, Nsamp - 1);
//    phase_noise = phase_noise - (tim / (Nsamp - 1)) * phase_noise.row(phase_noise.rows()-1);
//    phase_noise = fastExp(phase_noise);
//    for (int i = 0; i < Nch; i++)
//        light.field.col(i * Npol) = light.field.col(i * Npol).cwiseProduct(phase_noise.col(i * Npol));
//
//    // Add Gaussian complex white noise
//    double N0_lin        = pow(10, N0 / 10);
//    double sigma         = sqrt(N0_lin / 2 * gstate.SAMP_FREQ);
//    MatrixXd randMatrix1 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
//    MatrixXd randMatrix2 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
//    MatrixXcd randComplexMatrix(Nsamp, Nch * Npol);
//    randComplexMatrix.real() << randMatrix1;
//    randComplexMatrix.imag() << randMatrix2;
//    light.field = light.field + sigma * randComplexMatrix;
//
//    return light;
//}

/**
 * @brief It is a laser source module with multi-channel and noise. It would compute in single-polarizations
 *        and dual-polarizations mode in simulation.
 * @param ptx: power of constant wave. [mw]
 * @param lam: the central wavelength of the WDM comb. [nm]
 * @param spac: wavelength spacing [nm]
 * @param NLAMBDA: number of carriers.
 * @param option: compute option. Details in laserSource.h.
 * @return E: a struct of wave, details in fiber_types.h
 */
E laserSource(RowVectorXd ptx, RowVectorXd lam, double spac, int NLAMBDA, Option options) {
    E light;                             // transmit light
    unsigned long Nsamp = gstate.NSAMP;  // Sampling frequency
    int Npow            = ptx.size();    // Number of the power of transmit channel
    int Nch             = NLAMBDA;       // Number of carriers

    if (NLAMBDA > 0) {
        if (!(lam.size() == 1))
            ERROR("LAM is the central wavelength of the WDM comb. Size is one.");
    } else
        Nch = lam.size();  // Number of carriers

    if (Npow > 1 && Npow != Nch)
        ERROR("Powers and wavelengths must have the same length");

    double N0             = options.n0;
    int Npol              = options.pol;  // Number of polarizations
    RowVectorXd linewidth = RowVectorXd(0);
    if (options.linewidth.size() != 0 && options.linewidth(0) != 0) {
        if (options.linewidth.size() == 1) {
            linewidth = RowVectorXd(Nch);
            linewidth.setConstant(options.linewidth(0));
        } else if (options.linewidth.size() == Nch)
            linewidth = options.linewidth;
        else
            ERROR("The linewidth must have the same length of the number of wavelengths.");
    }

    RowVectorXd power;
    if (Npow == 1)
        power = RowVectorXd(Nch).setConstant(ptx(0));
    else
        power = ptx;

    // uniformly spaced carriers
    if (spac != 0.0)
        light.lambda = getLambda(lam(0), spac, Nch);
    else
        light.lambda = lam;

    light.field = MatrixXd::Zero(Nsamp, Nch * Npol);
    // by default, fully polarized on x (odd columns):
    for (int i = 0; i < Nch; i++)
        light.field.col(i * Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));

    // Standard normal distribution X(0,1) random number generation engine.
    static default_random_engine engine(time(0));
    static normal_distribution<double> n(0, 1);

    // Add phase noise
    // This part refer to 'freq_noise  = (ones(Nsamp,1) * sqrt(2*pi*linewidth./GSTATE.FSAMPLING)) .* randn( Nsamp, Nch)' in Optilux.
    // New a vector named one(Nsamp,1) in equation above.
    if (linewidth.size() != 0) {
        VectorXd oneVector(Nsamp);
        oneVector.setOnes();
        // New a matrix named randn( Nsamp, Nch), which follows standard normal distribution X(0,1).
        MatrixXd randMatrix = MatrixXd::Zero(Nsamp, Nch).unaryExpr([](double dummy) { return n(engine); });
        MatrixXd freq_noise = linewidth * 2 * M_PI;
        freq_noise          = freq_noise / gstate.SAMP_FREQ;
        freq_noise          = freq_noise.cwiseSqrt();
        freq_noise          = oneVector * freq_noise;
        freq_noise          = freq_noise.cwiseProduct(randMatrix);
        freq_noise(0, 0)    = 0;

        MatrixXcd phase_noise = freq_noise;
        for (int i = 0; i < phase_noise.rows() - 1; i++)
            phase_noise(i + 1) += phase_noise(i);

        VectorXd tim(Nsamp);
        tim.setLinSpaced(0, Nsamp - 1);
        phase_noise = phase_noise - (tim / (Nsamp - 1)) * phase_noise.row(phase_noise.rows() - 1);
        phase_noise = fastExp(phase_noise);
        for (int i = 0; i < Nch; i++)
            light.field.col(i * Npol) = light.field.col(i * Npol).cwiseProduct(phase_noise.col(i * Npol));
    }

    // Add Gaussian complex white noise
    if (N0 != INT_MIN) {
        double N0_lin        = pow(10, N0 / 10);
        double sigma         = sqrt(N0_lin / 2 * gstate.SAMP_FREQ);
        MatrixXd randMatrix1 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
        MatrixXd randMatrix2 = MatrixXd::Zero(Nsamp, Nch * Npol).unaryExpr([](double dummy) { return n(engine); });
        MatrixXcd randComplexMatrix(Nsamp, Nch * Npol);
        randComplexMatrix.real() << randMatrix1;
        randComplexMatrix.imag() << randMatrix2;
        light.field = light.field + sigma * randComplexMatrix;
    }

    return light;
}

MatrixXd getLambda(const double lamc, const double spac, const int Nch) {
    double freq = LIGHT_SPEED / lamc;  // [GHz]
    double DF   = pow(spac / lamc, 2) * LIGHT_SPEED;
    MatrixXd lambda(1, Nch);
    for (int i = 0; i < Nch; i++) {
        double freqt           = freq + DF * (i - (Nch + 1) / 2);
        lambda(1, Nch - i - 1) = LIGHT_SPEED / freqt;
    }
    return lambda;
}