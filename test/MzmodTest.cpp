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

#include <simulib>

int main(){
    RowVectorXd xx(3);
    RowVectorXd yy(3);
    xx.setConstant(3);
    yy.setConstant(3);
    gstate.NSAMP = 1;
    Option option;
    option.pol = 2;
    option.linewidth = xx;
    option.n0 = 0.5;
    E light = laserSource(xx, yy);
    std::cout<<"lambda = "<<light.lambda<<endl;
    std::cout<<"field = "<<light.field<<endl;

    VectorXcd modsig(1);
    modsig.setConstant(1);
    Mzoption mzoption;
    light = mzmodulator(light,modsig,mzoption);
    std::cout<<"lambda = "<<light.lambda<<endl;
    std::cout<<"field = "<<light.field<<endl;

    return 0;
}