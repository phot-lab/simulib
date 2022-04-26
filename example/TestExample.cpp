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
 * Created: 2022/4/26
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>
#include <fstream>
#include <string>

using namespace std;
using namespace SimuLib;

static VectorXi readField(const std::string &filepath) {

    // Open file in read mode
    ifstream infile;
    infile.open(filepath);

    VectorXi vec(1024);
    for (Index i = 0; i < vec.size(); ++i) {
        infile >> vec[i];
    }

    // Close file
    infile.close();
    return vec;
}

int main() {

    // Global parameters
    int nSymbol = 1024;  // number of symbols
    int nt      = 32;    // number of discrete points per symbol

    // Tx parameters
    Par par{};
    int symbolRate   = 10;      // symbol rate [Gbaud].
    par.rolloff      = 0.2;     // pulse roll-off
    par.emph         = "asin";  // digital-premphasis type
    string modFormat = "qpsk";  // modulation format
    double powerDBM  = 0;       // power [dBm]

    RowVectorXd lambda(1);
    lambda << 1550;  // carrier wavelength [nm]

    // Channel parameters
    Fiber fiber{};  // Transmission fiber

    // 初始化参数，这些参数可能会在之后的所有器件中用到
    double nSample = nSymbol * nt;     // overall number of samples
    double fs      = symbolRate * nt;  // sampling rate [GHz]
    initGstate(nSample, fs);           // initialize global variables: Nsamp and fs.

    // 光源模块的参数
    RowVectorXd pLin(1);
    pLin << pow(10, powerDBM / 10);  // [mW]
    LaserOption laserOption{};
    laserOption.pol = LaserOption::single;  // Single: only the x-polarization is created
    laserOption.n0  = 0.5;                  // One-sided spectral density

    // 光源模块
    E e = CPU::laserSource(pLin, lambda, laserOption);  // y-polarization does not exist

    // 随机二进制生成器的参数
    string array[2] = {"alpha", modFormat};
    VectorXi pattern;
    MatrixXi patternBinary;

    // 随机二进制生成器
    //        tie(pattern, patternBinary) = CPU::genPattern(nSymbol, "rand", array);

    pattern = readField("../files/pat.txt");

    MatrixXcd signal;
    double norm;

    // 数字调制器
    tie(signal, norm) = CPU::digitalModulator(pattern, symbolRate, par, modFormat, "rootrc");

    double gain = 0;

    // 电信号放大器
//    tie(signal, gain) = CPU::electricAmplifier(signal, 10, 1, 10.0e-12);

    // MZ调制器
    e = CPU::mzmodulator(e, signal);

    Out out{};

    // 光纤传输模块
    tie(out, e) = CPU::fiberTransmit(e, fiber);

    // 前端接收器的参数
    RxOption rxOption{};
    rxOption.modFormat = modFormat;
    rxOption.ofType    = "gauss";
    rxOption.obw       = INFINITY;

    // 前端接收器
    MatrixXcd returnSignal = CPU::rxFrontend(e, lambda, symbolRate, rxOption);

    // 电信号放大器（随后使用returnSignal去绘制眼图和星座图，注意是经过放大器的）
    tie(returnSignal, gain) = CPU::electricAmplifier(returnSignal, 20, 1, 10.0e-12);

    complex<double> eyeOpening;
    MatrixXcd iricMat;

    // 眼图分析器（随后使用eyeOpening和iricMat这两个值去计算误码率）
    tie(eyeOpening, iricMat) = CPU::evaluateEye(pattern, returnSignal, symbolRate, modFormat, fiber);
}