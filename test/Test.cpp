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
 * Created: 2022/3/11
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>

using namespace SimuLib;

int foo() {
    return 3;
}

int main() {
    MatrixXi m(4, 1);
    m << 1, 2, 3, 4;
    std::cout << decToBin(m,3) << std::endl;
    double a;
    std::cout << a << std::endl;
    string name         = "Hello";
    const string &other = name;
    name                = "change";
    std::cout << other << std::endl;
}
