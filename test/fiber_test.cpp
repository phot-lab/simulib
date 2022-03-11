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

#include <fstream>
#include <iostream>
#include <simulib>

using namespace std;

VectorXcd ReadField() {

    // Open file in read mode
    ifstream infile;
    infile.open("../field.txt");

    VectorXcd vec(32768);
    for (Index i = 0; i < vec.size(); ++i) {
        infile >> vec[i];
    }

    // Close file
    infile.close();
    return vec;
}

int main() {

    E e;
    e.field = ReadField();
    Fiber fiber;
    Out out     = {};
    tie(out, e) = FiberTransmit(e, fiber);
    cout << e.field << endl;

    return 0;
}