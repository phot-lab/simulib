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
 * Created: 2022/3/10
 * Supported by: National Key Research and Development Program of China
 */

#define EIGEN_USE_MKL_ALL  // Turn on this to use MKL for all Eigen operations
#include <chrono>
#include <iostream>
#include <SimuLib>

using namespace Eigen;
using namespace std;

int main() {

    const int length = 6000;

    MatrixXd m1 = MatrixXd ::Random(length, length);
    MatrixXd m2 = MatrixXd ::Random(length, length);
    MatrixXd m3(length, length);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    m3 = m1 * m2;

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    long long duration_ms = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    double time           = (double) duration_ms / 1000;
    cout << "测试通过，可以运行" << endl;
    cout << "使用的线程数：" << nbThreads() << endl;
    cout << "Eigen运行时间: " << time << "s" << endl;
    return 0;
}