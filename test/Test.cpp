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

using namespace std;
using namespace Eigen;

int main() {
    //    MatrixXcd m(2, 2);
    //    m << 1 + 4i, 45 + 3i, 2 + 4i, 9 + 10i;
    //    cout << maxRow(m) << endl;
    //    cout << minRow(m) << endl;
    MatrixXd m(2, 2);
    m << 2, 1, 3, 4;
    cout << m << endl;
    cout << maxCol(m) << endl;
}
