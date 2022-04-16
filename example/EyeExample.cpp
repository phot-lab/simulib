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
 * Created: 2022/4/8
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>
#include <string>

using namespace std;
using namespace SimuLib;

int main() {
    Par par{};
    cout << "sin(-512*M_PI)/(-512*M_PI):" << sin((double)-512*M_PI) / ((double)-512*M_PI) << endl;

    // Global parameters
    int nSymb = 1024;  // number of symbols
    int nt    = 32;    // number of discrete points per symbol

    // Tx parameters
    int symbrate     = 10;      // symbol rate [Gbaud].
    par.rolloff      = 0.01;     // pulse roll-off
    par.emph         = "asin";  // digital-premphasis type
    string modFormat = "qpsk";   // modulation format
    double powerDBM  = 0;       // power [dBm]

    /**** Channel parameters ****/
    double dtot = 0;  // residual dispersion per span [ps/nm]
    Fiber fiber{};    // Transmission fiber

    // Rx parameters ?
    fiber.modFormat         = "qpsk";       // modulation format
    string sync_type        = "da";         // time-recovery method ?
    fiber.opticalFilterType = "gauss";      // optical filter type
    fiber.obw               = INT_MAX;      // optical filter bandwidth normalized to symbrate
    fiber.length = 1000;
    fiber.dispersion = 17;
    string eftype           = "rootrc";     // optical filter type ?
    double ebw              = 0.5;          // electrical filter bandwidth normalized to symbrate ?
    double epar             = par.rolloff;  // electrical filter extra parameters ?

    RowVectorXd lambda(1);
    RowVectorXd ptx(1);
    lambda << 1550;  // carrier wavelength [nm]
    ptx << 3;

    // Init
    double nSamp = nSymb * nt;     // overall number of samples
    double fs    = symbrate * nt;  // sampling rate [GHz]
    initGstate(nSamp, fs);         // initialize global variables: Nsamp and fs.

    // Tx side
    RowVectorXd pLin(1);
    pLin << pow(10, powerDBM / 10);  // [mW]

    Option option{};
//    option.pol       = Option::dual;
//    option.linewidth = ptx;
//    option.n0        = INT_MIN;

    // 光源模块
    E e = CPU::laserSource(pLin, lambda);  // y-pol does not exist
    E ex;
    E ey;

    tie(ex, ey) = GPU::pbs(e);

    string array[2] = {"alpha", modFormat};
    VectorXi patX;
    MatrixXi patBinaryX;
    VectorXi patY;
    MatrixXi patBinaryY;

    // 随机二进制生成器
    tie(patX, patBinaryX) = CPU::pattern(nSymb, "rand", array);
    tie(patY, patBinaryY) = CPU::pattern(nSymb, "rand", array);
//    cout << "patX.size():" << patX.size() << endl;
    for(int i = 0; i < patX.size(); i++){
        patX(i) = i % 4;
        patY(i) = i % 4;
    }
//    cout << "patX :" << patX << endl;


    MatrixXcd signalX;
    double normX = 1;
    MatrixXcd signalY;
    double normY = 1;

    // 数字调制器
    tie(signalX, normX) = CPU::digitalModulator(patX, symbrate, par, modFormat, "rootrc");
//    signalY = signalX;
//    normY = normX;
    tie(signalY, normY) = CPU::digitalModulator(patY, symbrate, par, modFormat, "rootrc");
//    cout << "signalX:" << signalX << endl;
//    cout << "signalY:" << signalY << endl;
//    double gain = 0;
//    tie(e, gain) = electricAmplifier(e, 10, 1, 10.0e-12);
//    tie(e, gain) = electricAmplifier(e, 10, 1, 10.0e-12);

    // IQ调制器
    IQOption iqOptionX{};
    IQOption iqOptionY{};
    iqOptionX.norm = normX;
    iqOptionY.norm = normY;
    iqOptionX.nch  = 1;
    iqOptionY.nch  = 1;
    iqOptionX.vpi = M_PI/2;
    iqOptionY.vpi = M_PI/2;

    ex             = CPU::IQModulator(ex, signalX, iqOptionX);
    ey             = CPU::IQModulator(ey, signalY, iqOptionY);

    e = GPU::pbc(ex, ey);

    Out out{};

    // 光纤传输模块
//    tie(out, e) = fiberTransmit(e, fiber);

    fiber.modFormat         = modFormat;
    fiber.opticalFilterType = "gauss";

    // 前端接收器
    MatrixXcd returnSignal = CPU::rxFrontend(e, lambda, symbrate, fiber);

//    cout << "returnSignal:" << returnSignal << endl;

    complex<double> eyeOpening;
    MatrixXcd iricMat;

    // 眼图分析器（随后使用eyeOpening和iricMat这两个值去绘制眼图）
    MatrixXcd signalAngle = returnSignal.unaryExpr([](const complex<double> &a){
//        if(abs(a.imag()) < 0.00000000001 && abs(a.real()) < 0.00000000001){
//            return (double)INT_MAX;
//        }
        return atan2(a.imag(),a.real());
    });
//    for(int col = 0; col < signalAngle.cols(); col++){
//        for(int row = 0; row < signalAngle.rows(); ++row){
//            if( signalAngle(row,col) == (double)INT_MAX){
//                signalAngle(row,col) = (signalAngle(row+1,col) + signalAngle(row-1,col))/(double)2;
//            }
//        }
//    }


//    cout << "signalAngle:\n" << signalAngle << endl;

    MatrixXi pat(patX.rows() ,patX.cols() + patY.cols());

    pat << patX, patY;
    tie(eyeOpening, iricMat) = GPU::evaluateEye(pat, signalAngle , symbrate, modFormat, fiber);
    cout << "Eye open:" << eyeOpening << endl;
//    cout << "iricMat:\n" << iricMat << endl;
    return 0;
}
