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
 * Created: 2022/4/12
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>

int main() {
    MatrixXd m(2, 2);
    m << 1, 2, 3, 4;
    std::cout << m << std::endl;
    std::cout << m + m << std::endl;
    std::cout << m - m << std::endl;
    std::cout << m * m << std::endl;

    MatrixXcd m2(2, 2);
    m2 << 1, 2, 3, 4;
    std::cout << m2 << std::endl;
    std::cout << m2 + m2 << std::endl;
    std::cout << m2 - m2 << std::endl;
    std::cout << m2 * m2 << std::endl;

}