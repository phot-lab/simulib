//
// Created by 18301 on 2022/3/18.
//

#ifndef SIMULIB_LASER_SOURCE_H
#define SIMULIB_LASER_SOURCE_H

#include <float.h>
#include <iostream>
#include <random>
#include <ctime>
#include <stdarg.h>

/**
 * @brief laser source simulation option
 * @param pol: polarization schema. 'single' only the x-polarization is created,otherwise even
 *             the y-polarization is created and set equal to zero in absence of noise.
 * @param linewidth: It can be a scalar or a vector of the same length of the wavelengths. A Wiener
 *                   phase noise with such a linewidth is added to the phase of  E.
 * @param n0: the one-sided spectral density [dB/GHz] of a Gaussian complex noise added to the laser field.
 */
struct Option{
    int pol = dual; // single or dual
    RowVectorXd linewidth; // [GHz]
    double n0; // [dB/GHz]
    enum{dual=2 , single=1};
};

struct EMultimode{
    MatrixXd lambda;
    MatrixXcd field;
};

EMultimode laser_source (RowVectorXd ptx, RowVectorXd lam);

EMultimode laser_source(RowVectorXd ptx, RowVectorXd lam, Option options);

EMultimode laser_source(RowVectorXd ptx, double lam, double spac, int NLAMBDA);

EMultimode laser_source(RowVectorXd ptx, RowVectorXd lam, double spac, int NLAMBDA, Option options);

/**
 * @brief It is a laser source module with multi-channel and noise. It would compute in single-polarizations
 *        and dual-polarizations mode in simulation.
 * @param ptx: power of constant wave. [mw]
 * @param lam: the central wavelength of the WDM comb. [nm]
 * @param spac: wavelength spacing [nm]
 * @param NLAMBDA: number of carriers.
 * @param option: compute option. Details in laser_source.h.
 * @return E: a struct of wave, details in fiber_types.h
 */
template<typename ... Args>
EMultimode laser_source(RowVectorXd ptx, RowVectorXd lam,Args ... args){
    EMultimode light;                               // transmit light

    std::cout<< sizeof...(args)<<endl;

    double spac ;
    int NLAMBDA;
    Option options;
//    unsigned long Nsamp = gstate.NSAMP;                // Sampling frequency
//
//    int Npow = ptx.size();                  // Number of the power of transmit channel
//    int Nch = NLAMBDA;                      // Number of carriers
//
//    if ( Npow > 1 && Npow != Nch)
//        ERROR("Powers and wavelengths must have the same length");
//
//    double N0 = options.n0;
//    int Npol = options.pol;                // Number of polarizations
//    RowVectorXd linewidth;
//    if( options.linewidth.size() == 1){
//        linewidth = RowVectorXd(Nch);
//        linewidth.setConstant(options.linewidth(0));
//    }else if( options.linewidth.size() == Nch)
//        linewidth = options.linewidth;
//    else
//        ERROR("The linewidth must have the same length of the number of wavelengths.");
//
//    RowVectorXd power;
//    if (Npow == 1){
//        ptx.conservativeResize(Nch);
//        ptx.setConstant(ptx(0));
//        power = ptx;
//    }
//    else
//        power = ptx;
//
//    light.lambda = GetLambda(lam,spac,Nch);
//    light.field = MatrixXd::Zero(Nsamp,Nch*Npol);
//    for (int i = 0; i < Nch; i++)
//        light.field.col(i*Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));
//
//    // Add phase noise
//    // This part refer to 'freq_noise  = (ones(Nsamp,1) * sqrt(2*pi*linewidth./GSTATE.FSAMPLING)) .* randn( Nsamp, Nch)' in Optilux.
//    // New a vector named one(Nsamp,1) in equation above.
//    VectorXd oneVector(Nsamp);
//    oneVector.setOnes();
//    // New a matrix named randn( Nsamp, Nch), which follows standard normal distribution X(0,1).
//    static default_random_engine engine(time(0));
//    static normal_distribution<double> n(0,1);
//    MatrixXd randMatrix = MatrixXd::Zero(Nsamp,Nch).unaryExpr([](double dummy){return n(engine);});
//    MatrixXd freq_noise = linewidth * 2 * M_PI ;
//    freq_noise =  freq_noise / gstate.SAMP_FREQ;
//    freq_noise = freq_noise.cwiseSqrt();
//    freq_noise = oneVector * freq_noise;
//    freq_noise = freq_noise.cwiseProduct(randMatrix);
//    freq_noise(0,0) = 0;
//
//    MatrixXcd phase_noise = freq_noise;
//    for(int i = 0; i < phase_noise.rows()-1; i++)
//        phase_noise(i+1) += phase_noise(i);
//
//    VectorXd tim(Nsamp);
//    tim.setLinSpaced(0,Nsamp-1);
//    phase_noise = phase_noise - (tim / (Nsamp-1)) * phase_noise.row(phase_noise.rows());
//    phase_noise = FastExp(phase_noise);
//    for(int i = 0; i < Nch; i++)
//        light.field.col(i * Npol) = light.field.col(i * Npol).cwiseProduct( phase_noise.col(i * Npol) );
//
//    // Add Gaussian complex white noise
//    double N0_lin = pow(10,N0/10);
//    double sigma = sqrt(N0_lin / 2 * gstate.SAMP_FREQ);
//    MatrixXd randMatrix1 = MatrixXd::Zero(Nsamp,Nch*Npol).unaryExpr([](double dummy){return n(engine);});
//    MatrixXd randMatrix2 = MatrixXd::Zero(Nsamp,Nch*Npol).unaryExpr([](double dummy){return n(engine);});
//    MatrixXcd randComplexMatrix(Nsamp,Nch*Npol);
//    randComplexMatrix.real()<<randMatrix1;
//    randComplexMatrix.imag()<<randMatrix2;
//    light.field = light.field + sigma*randComplexMatrix;

    return light;
}

#endif  // SIMULIB_LASER_SOURCE_H
