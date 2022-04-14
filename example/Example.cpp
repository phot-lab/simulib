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
#include <string>

using namespace std;

int main() {
    Par par{};

    // Global parameters
    int nSymb = 32;  // number of symbols
    int nt    = 8;    // number of discrete points per symbol

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
    E e = laserSource(pLin, lambda, option);  // y-pol does not exist

    string array[2] = {"alpha", modFormat};
    VectorXi pat;
    MatrixXi patBinary;

    // 随机二进制生成器
    tie(pat, patBinary) = pattern(nSymb, "rand", array);
//    pat = VectorXi(nSymb);
//    patBinary = MatrixXi(nSymb,1);
//    for(int i = 0; i < nSymb; i++){
//        pat(i) = i%2;
//    }
//    std::random_device rand_dev;
//    std::mt19937 generator(rand_dev());
//    std::uniform_real_distribution<double> distr(0, 1);
//    for(int i = 0; i < nSymb; i++){
//        cout << "rand1:" << rand_dev() << endl;
//        cout << "rand2:" << generator() << endl;
//        cout << "rand3:" << distr(generator) << endl;
//    }

    patBinary.col(0) = pat;
    cout << "pat:---------------\n" << pat << endl;
    cout << "patBinary:---------------\n" << patBinary << endl;

//    cout << "rand:\n";
//    for(int i = 0; i < nSymb; i++){
//        cout << uniformRng() << endl;
//    }

    MatrixXcd signal;
    double norm;

    // 数字调制器
    tie(signal, norm) = digitalModulator(patBinary, symbrate, par, modFormat, "costails");

    double gain = 0;

    // 电信号放大器
    tie(e, gain) = electricAmplifier(e, 10, 1, 10.0e-12);

    // MZ调制器
    e = mzmodulator(e, signal);

    Out out{};

    // 光纤传输模块
    tie(out, e) = fiberTransmit(e, fiber);

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
    tie(e, gain) = electricAmplifier(e, 20, 1, 10.0e-12);

    // 前端接收器
    MatrixXcd returnSignal = rxFrontend(e, lambda, symbrate, fiber);

    complex<double> eyeOpening;
    MatrixXcd iricMat;

    // 眼图分析器（随后使用eyeOpening和iricMat这两个值去绘制眼图）
    tie(eyeOpening, iricMat) = evaluateEye(patBinary, returnSignal, symbrate, modFormat, fiber);
    cout << "Eye open:" << eyeOpening << endl;
//    cout << "iricMat:" << iricMat << endl;
    return 0;
}