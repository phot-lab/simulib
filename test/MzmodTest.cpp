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
 * Created: 2022/3/19
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>

using namespace SimuLib;

int main() {
    RowVectorXd ptx(1);
    RowVectorXd lam(1);
    ptx.setConstant(1);
    lam.setConstant(1550);
    // Global parameters
    int nSymb    = 16;             // number of symbols
    int nt       = 32;             // number of discrete points per symbol
    int symbrate = 10;             // symbol rate [Gbaud].
    double nSamp = nSymb * nt;     // overall number of samples
    double fs    = symbrate * nt;  // sampling rate [GHz]
    initGstate(nSamp, fs);         // initialize global variables: Nsamp and fs.
    double spac = 37.5*pow(1550,2)*LIGHT_SPEED;

    LaserOption option;
    // If the pol, polarization, is 2, it is mean laser source would consider the polarization in x and y.
    // If the pol is 1, lasersource would only output the polarization in x.
    option.pol       = 1;
    // If you set linewidth, laser source would add phase noise.
    option.lineWidth = VectorXd(1).setConstant(3);
    // If you set n0, laser source would add Gaussian complex white noise.
    option.n0        = 0.5;
    // If you do not set n0 and  linewidth, laser source would be an ideal light without noise.
    // Ideal ligth is constant. There are not noise and the sample seen to be only zero in imaginary of X polarization
    // and zero in y polarization. It would show like below:
    //(1,0) (0,0)
    //(1,0) (0,0)
    E light          = CPU::laserSource(ptx, lam,option);
    std::cout << "lambda = " << light.lambda<< endl;
    std::cout << "field = " << light.field << endl;

    VectorXcd modsig((int) nSamp);
    //    modsig.setOnes();
    modsig.setRandom();
//    std::cout << "modsig size = " << modsig.size() << endl;
//    std::cout << "modsig = " << modsig << endl;
    light = CPU::mzModulator(light, modsig);
    std::cout << "lambda = " << light.lambda << endl;
    std::cout << "field = " << light.field << endl;

    return 0;
}