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
* Created: 2022/4/10
* Supported by: National Key Research and Development Program of China
 */

#include "SimuLib"

MatrixXi decToBin(MatrixXi dec, int nBit){
    MatrixXd bin = dec.cast<double>();
    MatrixXd exponent(1,abs(1-nBit)+1);
    exponent.row(0) =  ArrayXd::LinSpaced(abs(1-nBit)+1,1-nBit ,0);
    exponent = pow(2,exponent.array());
    bin = bin * exponent;
    bin = floor(bin.array());

    return bin.unaryExpr([](double x){
        if ( x > 0)
            x = floor(x);
        else
            x = ceil(x);
        if( (int)x % 2 != 0)
            return 1;
        else
            return 0;
    });
}
