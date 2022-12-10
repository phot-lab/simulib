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
 * Created: 2022/4/1
 * Supported by: National Key Research and Development Program of China
 */

#include <SimuLib>

using namespace SimuLib;

int main() {
    VectorXcd v                            = VectorXcd ::Random(4324);
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    for (int i = 0; i < 10000; ++i) {
        v = fft(v);
        v = ifft(v);
    }
//    int a = 10;
//    for (int i = 0; i < 1000000000; ++i) {
//        a = 32 * 32 + 324 * 432;
//    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    long long duration_ms = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    double time           = (double) duration_ms / 1000;
    cout << "FFT运行时间: " << time << "s" << endl;
    //    std::cout << v << std::endl;

    //    cout << v << endl;
    //    v = fft(v);
    //    cout << v << endl;
    //    v = ifft(v);
    //    cout << v << endl;
}