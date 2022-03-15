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
#include <simulib>
#include <float.h>
#include <iostream>
using namespace Eigen;
using namespace std;


/*
 * @brief laser source simulation option
 * @param pol: polarization schema. 'single' only the x-polarization is created,otherwise even
 *             the y-polarization is created and set equal to zero in absence of noise.
 * @param linewidth: It can be a scalar or a vector of the same length of the wavelengths. A Wiener
 *                   phase noise with such a linewidth is added to the phase of  E.
 * @param n0: the one-sided spectral density [dB/GHz] of a Gaussian complex noise added to the laser field.
 */
struct Option{
    int pol; // single or dual
    double linewidth = 3; // [GHz]
    double n0; // [dB/GHz]
    enum{dual=2 , single=1};
};

struct Ee{
    MatrixXd lambda;
    MatrixXd field;
};

/*
 * @brief It is a laser source module in dual-polarization. It would compute x-polarizations
 *        and y-polarizations in simulation. It is an ideal light source.
 * @param ptx: power of constant wave
 * @param lam: wavelength of constant wave. Here odd columns represent x-polarizations samples,
 *             while the even columns the y-polarizations samples
 * @return E: a struct of wave, details in fiber_types.h
 */
Ee lasersource (VectorXd ptx, VectorXd lam)
{
    unsigned long Nsamp = 2;                // Sampling frequency
    int Npow = ptx.size();                  // Number of the power of transmit channel
    int Nch = lam.size();                   // Number of wavelength of transmit channel
    Ee light;                               // transmit light

    if (Npow > 1 && Npow != Nch)
        std::cout<<"Powers and wavelengths must have the same length";

    int Npol = Option::dual;                // Number of polarizations

    RowVectorXd power;
    if (Npow == 1){
        ptx.conservativeResize(Nch);
        ptx.setConstant(ptx(0));
        power = ptx.transpose(); // Convert a column vector to a row vector
    }
    else
        power = ptx.transpose(); // Convert a column vector to a row vector

    light.lambda = lam.transpose();          // Convert a column vector to a row vector
    light.field = MatrixXd::Zero(Nsamp,Nch*Npol);
    for (int i = 0; i < Nch; i++)
        light.field.col(i*Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));

    return light;
}


/*
 * @brief It is a laser source module in dual-polarization. It would compute x-polarizations
 *        and y-polarizations in simulation.
 * @param ptx: power of constant wave.
 * @param lam: wavelength of constant wave. Here odd columns represent x-polarizations samples,
 *             while the even columns the y-polarizations samples.
 * @param option:
 * @return E: a struct of wave, details in fiber_types.h
 */
Ee lasersource(VectorXd ptx, VectorXd lam, Option options){
    Ee light;                               // transmit light
    unsigned long Nsamp = 2;                // Sampling frequency
    int Npow = ptx.size();                  // Number of the power of transmit channel
    int Nch = lam.size();                   // Number of wavelength of transmit channel

    if ( Npow > 1 && Npow != Nch)
        std::cout<<"Powers and wavelengths must have the same length";

    double N0 = options.n0;
    int Npol = options.pol;                // Number of polarizations
    RowVectorXd linewidth(Nch);
    linewidth.setConstant(options.linewidth);

    RowVectorXd power;
    if (Npow == 1){
        ptx.conservativeResize(Nch);
        ptx.setConstant(ptx(0));
        power = ptx.transpose(); // Convert a column vector to a row vector
    }
    else
        power = ptx.transpose(); // Convert a column vector to a row vector

    light.lambda = lam.transpose();          // Convert a column vector to a row vector
    light.field = MatrixXd::Zero(Nsamp,Nch*Npol);
    for (int i = 0; i < Nch; i++)
        light.field.col(i*Npol) = VectorXd(Nsamp).setConstant(sqrt(power(i)));

    VectorXd oneVector(Nsamp);
    oneVector.setOnes();
    linewidth = linewidth * 2 * M_PI ;


    return light;
}


int main(){
    MatrixXd x(5,5);
    x.setConstant(4);
    std::cout<<x<<endl;
    MatrixXd y = MatrixXd::Constant(5,1,9);
    std::cout<<y<<endl;
    x.col(0) = y;
    x.col(1) = VectorXd(5).setConstant(1);
    std::cout<<x<<endl;
    std::cout<<x.cwiseSqrt()<<endl;
}