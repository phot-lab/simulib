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
#include "Internal"

namespace SimuLib {

/**
 * @brief in-phase/quadrature optical modulator.
 * modulates the in-phase and
 *  quadrature components of the field E (see LASERSOURCE) with
 *  Mach-Zehnder modulators [1].
 *
 *          ____ MZ (in-phase) ____
 *        /                         \
 *  ------                           ------
 *        \ ____ MZ (quad) _ pi/2 _ /
 *
 * @param e: optical field, is a struct of fields lambda, field.
 * @param MODSIG: a complex vector containing in the real and in the imaginary
 *               parts the in-phase and the quadrature components, respectively. Such
 *               driving signals are created, e.g., by DIGITALMOD.
 * @param option: compute option. Details in IQModulator.h
 * @return E:
 */

namespace PARALLEL_TYPE {

E IQModulator(E e, VectorXcd modSig, IQOption option) {
    Mzoption mzoptioni;
    Mzoption mzoptionq;
    double iqratio    = option.iqratio;
    mzoptioni.biasu   = option.biasu[0];
    mzoptionq.biasu   = option.biasu[1];
    mzoptioni.biasl   = option.biasl[0];
    mzoptionq.biasl   = option.biasl[1];
    mzoptioni.vpi     = option.vpi;
    mzoptionq.vpi     = option.vpi;
    mzoptioni.exratio = option.exratio[0];
    mzoptionq.exratio = option.exratio[1];
    mzoptioni.norm    = option.norm;
    mzoptionq.norm    = option.norm;
    mzoptioni.mode    = Mzoption::push_pull;
    mzoptionq.mode    = Mzoption::push_pull;
    int npol          = 1;
    int nch           = 1;
    double biasc      = option.biasc;

    //  number of channels
    if (e.lambda.size() > 1) {
        mzoptioni.nch = option.nch;
        mzoptionq.nch = option.nch;
        nch           = option.nch;
    } else {
        mzoptioni.nch = 1;
        mzoptionq.nch = 1;
    }

    // number of polarization
    if (e.field.cols() == 2 * e.lambda.size())
        npol = 2;

    iqratio   = pow(10, iqratio / 20);
    double sr = iqratio / (1 + iqratio);

    // Now set polarizations in alternate way (if they exist)
    RowVectorXi ncols = RowVectorXi(npol * nch - npol * (nch - 1));
    ncols.setLinSpaced(npol * (nch - 1), npol * nch - 1);

    // The main function
    E ei = e;
    E eq = e;
    for (int i = 0; i < ncols.size(); i++) {
        int col           = ncols(i);
        ei.field.col(col) = e.field.col(col) * sr;
        eq.field.col(col) = e.field.col(col) * (1 - sr);
    }

    ei = mzmodulator(ei, modSig.real(), mzoptioni);
    eq = mzmodulator(eq, modSig.imag(), mzoptionq);

    // *2: undo the two 3dB coupler loss
    for (int i = 0; i < ncols.size(); i++) {
        int col          = ncols(i);
        e.field.col(col) = 2 * (ei.field.col(col) + eq.field.col(col) * fastExp(M_PI / 2 + biasc));
    }
    return e;
}

}  // namespace PARALLEL_TYPE

}  // namespace SimuLib