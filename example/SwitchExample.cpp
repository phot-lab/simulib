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
 * Created: 2022/4/15
 * Supported by: National Key Research and Development Program of China
 */

/**
 * This example displays how to perform runtime switch between CPU and GPU.
 */

#include <SimuLib>
#include <string>

using namespace std;
using namespace SimuLib;

int main() {
    Par par{};

    // Global parameters
    int nSymb = 1024;  // number of symbols
    int nt    = 32;    // number of discrete points per symbol

    // Tx parameters
    int symbrate     = 10;      // symbol rate [Gbaud].
    par.rolloff      = 0.2;     // pulse roll-off
    par.emph         = "asin";  // digital-premphasis type
    string modFormat = "ook";   // modulation format
    double powerDBM  = 0;       // power [dBm]

    RowVectorXd lambda(1);
    RowVectorXd ptx(1);
    lambda << 1550;  // carrier wavelength [nm]
    ptx << 3;

    /**** Channel parameters ****/
    double dtot = 0;  // residual dispersion per span [ps/nm]
    Fiber fiber{};    // Transmission fiber

    // Init
    double nSamp = nSymb * nt;     // overall number of samples
    double fs    = symbrate * nt;  // sampling rate [GHz]
    initGstate(nSamp, fs);         // initialize global variables: Nsamp and fs.

    // Tx side
    RowVectorXd pLin(1);
    pLin << pow(10, powerDBM / 10);  // [mW]

    Option option{};
    option.pol       = 1;
    option.linewidth = ptx;
    option.n0        = 0.5;

    // 光源模块
    E e = GPU::laserSource(pLin, lambda, option);  // y-pol does not exist

    string array[2] = {"alpha", modFormat};
    VectorXi pat;
    MatrixXi patBinary;

    CPU::MatrixXd m;
    GPU::MatrixXd m2 = m;

    // 随机二进制生成器
    tie(pat, patBinary) = CPU::pattern(nSymb, "rand", array);

    MatrixXcd signal;
    double norm;

    // 数字调制器
    tie(signal, norm) = GPU::digitalModulator(patBinary, symbrate, par, modFormat, "costails");

    double gain = 0;

    // 电信号放大器
    tie(e, gain) = CPU::electricAmplifier(e, 10, 1, 10.0e-12);

    // MZ调制器
    e = GPU::mzmodulator(e, signal);

    Out out{};

    // 光纤传输模块
    tie(out, e) = GPU::fiberTransmit(e, fiber);

    //    cout << "光场：" << e.field << endl;
    //    cout << "光场矩阵的行：" << e.field.rows() << endl;
    //    cout << "光场矩阵的列：" << e.field.cols() << endl;
    //    cout << "光的波长：" << e.lambda(0, 0) << endl;
    //    cout << "light field:" << e.field << endl;

    cout << "light field row:" << e.field.rows() << endl;
    cout << "light field col:" << e.field.cols() << endl;
    cout << "light field wavelength:" << e.lambda(0, 0) << endl;

    fiber.modFormat         = modFormat;
    fiber.opticalFilterType = "gauss";

    // 电信号放大器
    tie(e, gain) = GPU::electricAmplifier(e, 20, 1, 10.0e-12);

    // 前端接收器
    MatrixXcd returnSignal = CPU::rxFrontend(e, lambda, symbrate, fiber);

    complex<double> eyeOpening;
    MatrixXcd iricMat;

    // 眼图分析器（随后使用eyeOpening和iricMat这两个值去绘制眼图）
    tie(eyeOpening, iricMat) = CPU::evaluateEye(patBinary, returnSignal, symbrate, modFormat, fiber);
    cout << "Eye open:" << eyeOpening << endl;
    //    cout << "iricMat:" << iricMat << endl;
    return 0;
}