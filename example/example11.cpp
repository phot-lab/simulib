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

int main() {
    Par par{};

    // Global parameters
    int nSymb = 64;  // number of symbols
    int nt    = 8;    // number of discrete points per symbol

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
    E e = laserSource(pLin, lambda);  // y-pol does not exist
    E ex;
    E ey;
//    cout << "before pbs e size:" << e.field.rows() <<"  "<<e.field.cols() << endl;
//    cout << "before pbs e.field:" << e.field << endl;
    tie(ex, ey) = pbs(e);
//    cout << "after pbs ex.field:" << ex.field << endl;
//    cout << "after pbs ey.field:" << ey.field << endl;

    string array[2] = {"alpha", modFormat};
    VectorXi patX;
    MatrixXi patBinaryX(64,1);
    VectorXi patY;
    MatrixXi patBinaryY(64,1);

    // 随机二进制生成器
    tie(patX, patBinaryX) = pattern(nSymb, "rand", array);
    tie(patY, patBinaryY) = pattern(nSymb, "rand", array);
//    patBinaryX.setOnes();
//    patBinaryY.setOnes();

    MatrixXcd signalX;
    double normX = 1;
    MatrixXcd signalY;
    double normY = 1;

//    cout << "before digitalModulator patBinaryX:" << patBinaryX << endl;
    // 数字调制器
    tie(signalX, normX) = digitalModulator(patBinaryX, symbrate, par, modFormat, "rootrc");
    tie(signalY, normY) = digitalModulator(patBinaryX, symbrate, par, modFormat, "rootrc");

    // IQ调制器
    IQOption iqOptionX{};
    IQOption iqOptionY{};
    iqOptionX.norm = normX;
    iqOptionY.norm = normY;
    iqOptionX.nch  = 1;
    iqOptionY.nch  = 1;
    iqOptionX.vpi = M_PI/2;
    iqOptionY.vpi = M_PI/2;
//    cout << "e.field:" << e.field << endl;
//    cout << "before iq ex.field:" << ex.field << endl;
//    cout << "before iq ey.field:" << ey.field << endl;

//    cout << "before iq signalX:\n" << signalX << endl;
//    cout << "before iq signalY:\n" << signalY << endl;

    ex             = IQModulator(ex, signalX, iqOptionX);
    ey             = IQModulator(ey, signalY, iqOptionY);
//    cout << "after iq ex.field:\n" << ex.field << endl;
//    cout << "after iq ey.field:\n" << ey.field << endl;

    e = pbc(ex, ey);

    Out out{};

    // 光纤传输模块
//    tie(out, e) = fiberTransmit(e, fiber);

//    cout << "after pbc e.field:" << e.field << endl;
//    cout << "light field row:" << e.field.rows() << endl;
//    cout << "light field col:" << e.field.cols() << endl;
//    cout << "light field wavelength:" << e.lambda(0, 0) << endl;

    fiber.modFormat         = modFormat;
    fiber.opticalFilterType = "gauss";

    // 前端接收器
    MatrixXcd returnSignal = rxFrontend(e, lambda, symbrate, fiber);
//    cout << "returnSignal:\n" << returnSignal << endl;
//    cout << "patBinary:" << patBinaryX << endl;

    complex<double> eyeOpening;
    MatrixXcd iricMat;

    // 眼图分析器（随后使用eyeOpening和iricMat这两个值去绘制眼图）
    MatrixXcd signalAngle = returnSignal.unaryExpr([](const complex<double> &a){
        return atan2(a.imag(),a.real());
    });
    cout << "returnSignal:" << returnSignal << endl;
//    cout << "signalAngle:" << signalAngle << endl;

//    MatrixXi patBinary(patBinaryX.rows() + patBinaryY.rows(),patBinaryX.cols());
//
//    patBinary << patBinaryX, patBinaryY;
//    cout << "patBinary:" << patBinary << endl;
    tie(eyeOpening, iricMat) = evaluateEye(patBinaryX, signalAngle , symbrate, modFormat, fiber);
    cout << "Eye open:" << eyeOpening << endl;
//    cout << "iricMat:" << iricMat << endl;
    return 0;
}
