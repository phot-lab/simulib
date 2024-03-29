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

static MatrixXcd angle(const MatrixXcd &signal) {
    MatrixXcd result = signal.unaryExpr([](const complex<double> &a) {
        return atan2(a.imag(), a.real());
    });
    return result;
}

int main() {
    Par par{};

    // Global parameters
    int nSymb = 1024;  // number of symbols
    int nt    = 32;    // number of discrete points per symbol

    // Tx parameters
    int symbrate     = 10;      // symbol rate [Gbaud].
    par.rolloff      = 0.01;    // pulse roll-off
    par.emph         = "asin";  // digital-premphasis type
    string modFormat = "qpsk";  // modulation format
    double powerDBM  = 0;       // power [dBm]

    /**** Channel parameters ****/
    double dtot = 0;  // residual dispersion per span [ps/nm]
    Fiber fiber{};    // Transmission fiber

    // Rx parameters ?
    //    string sync_type = "da";  // time-recovery method ?
    //    string eftype = "rootrc";     // optical filter type ?
    //    double ebw    = 0.5;          // electrical filter bandwidth normalized to symbrate ?
    //    double epar   = par.rolloff;  // electrical filter extra parameters ?

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

    LaserOption option{};
    // option.pol       = Option::dual;
    option.lineWidth = ptx;
    option.n0        = 0.5;

    // 光源模块
    E e = CPU::laserSource(pLin, lambda);  // y-pol does not exist
    E ex;
    E ey;

    tie(ex, ey) = CPU::pbs(e);  // Split in two orthogonal polarizations

    string array[2] = {"alpha", modFormat};
    VectorXi patX;
    MatrixXi patBinaryX;
    VectorXi patY;
    MatrixXi patBinaryY;

    // 随机二进制生成器
    //    tie(patX, patBinaryX) = CPU::pattern(nSymb, "rand", array);
    //    tie(patY, patBinaryY) = CPU::pattern(nSymb, "rand", array);

    // 暂时用固定数据代替
    patX = readPattern("../files/patx.txt");
    patY = readPattern("../files/paty.txt");

    MatrixXcd signalX;
    double normX = 1;
    MatrixXcd signalY;
    double normY = 1;

    // 数字调制器
    tie(signalX, normX) = CPU::digitalModulator(patX, symbrate, par, modFormat, "rootrc");
    tie(signalY, normY) = CPU::digitalModulator(patY, symbrate, par, modFormat, "rootrc");

    // IQ调制器
    IqOption iqOptionX{};
    IqOption iqOptionY{};
    iqOptionX.norm = normX;
    iqOptionY.norm = normY;
    iqOptionX.nch  = 1;
    iqOptionY.nch  = 1;
    iqOptionX.vpi  = M_PI / 2;
    iqOptionY.vpi  = M_PI / 2;

    ex = CPU::iqModulator(ex, signalX, iqOptionX);
    ey = CPU::iqModulator(ey, signalY, iqOptionY);

    e = CPU::pbc(ex, ey);

    Out out{};

    // 光纤传输模块
    //    tie(out, e) = fiberTransmit(e, fiber);

    RxOption rxOption{};
    rxOption.modFormat = modFormat;
    rxOption.ofType    = "gauss";
    rxOption.obw       = INFINITY;

    // 前端接收器（随后使用returnSignal去绘制眼图和星座图）
    MatrixXcd returnSignal = CPU::rxFrontend(e, lambda, symbrate, rxOption);

    double eyeOpening;
    MatrixXcd iricMat;

    MatrixXi pat(patX.rows(), patX.cols() + patY.cols());
    pat << patX, patY;

    // 眼图分析器（随后使用eyeOpening和iricMat这两个值去计算误码率）
    tie(eyeOpening, iricMat) = CPU::evaluateEye(pat, angle(returnSignal), symbrate, modFormat, fiber);

    std::cout << "Eye opening: " << eyeOpening << " [mw]" << std::endl;
    return 0;
}
