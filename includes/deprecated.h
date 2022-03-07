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
* Created: 2022/3/6
* Supported by: National Key Research and Development Program of China
*/

/**
 * These functions have been deprecated since I found better implementation of them.
 */

#ifndef OPTICALAB_DEPRECATED_H
#define OPTICALAB_DEPRECATED_H

#include <vector>

using namespace std;

template<typename T>
vector<T> FFTShift(const vector<T> &vec) {
    size_t size = vec.size();
    vector<T> ret(size);
    unsigned int pivot = (size % 2 == 0) ? (size / 2) : ((size + 1) / 2);
    unsigned int rightHalf = size - pivot;
    unsigned int leftHalf = pivot;
    memcpy(&ret[0], &vec[0] + pivot, sizeof(T) * rightHalf);
    memcpy(&ret[0] + rightHalf, &vec[0], sizeof(T) * leftHalf);
    return ret;
}

template<typename T>
vector<T> IFFTShift(const vector<T> &vec) {
    size_t size = vec.size();
    vector<T> ret(size);
    unsigned int pivot = (size % 2 == 0) ? (size / 2) : ((size - 1) / 2);
    unsigned int rightHalf = size - pivot;
    unsigned int leftHalf = pivot;
    memcpy(&ret[0], &vec[0] + pivot, sizeof(T) * rightHalf);
    memcpy(&ret[0] + rightHalf, &vec[0], sizeof(T) * leftHalf);
    return ret;
}

vector<double> MultiplyVecCPU(const vector<double> &a, const vector<double> &b) {
    size_t size = a.size();
    vector<double> result(size);
    unsigned i;
#pragma omp parallel default(none) shared(a, b, size, result) private(i)
    {
#pragma omp for schedule(static)
        for (i = 0; i < size; i++)
            result[i] = a[i] * b[i];
    }
    return result;
}

vector<double> PlusValueCPU(const vector<double> &a, const double &value) {
    size_t size = a.size();
    vector<double> result(size);
    unsigned i;
#pragma omp parallel default(none) shared(a, size, result, value) private(i)
    {
#pragma omp for schedule(static)
        for (i = 0; i < size; i++)
            result[i] = a[i] + value;
    }
    return result;
}

vector<double> SubtractValueCPU(const vector<double> &a, const double &value) {
    size_t size = a.size();
    vector<double> result(size);
    unsigned i;
#pragma omp parallel default(none) shared(a, size, result, value) private(i)
    {
#pragma omp for schedule(static)
        for (i = 0; i < size; i++)
            result[i] = a[i] - value;
    }
    return result;
}

vector<double> MultiplyValueCPU(const vector<double> &a, const double &value) {
    size_t size = a.size();
    vector<double> result(size);
    unsigned i;
#pragma omp parallel default(none) shared(a, size, result, value) private(i)
    {
#pragma omp for schedule(static)
        for (i = 0; i < size; i++)
            result[i] = a[i] * value;
    }
    return result;
}

vector<double> DivideValueCPU(const vector<double> &a, const double &value) {
    size_t size = a.size();
    vector<double> result(size);
    unsigned i;
#pragma omp parallel default(none) shared(a, size, result, value) private(i)
    {
#pragma omp for schedule(static)
        for (i = 0; i < size; i++)
            result[i] = a[i] / value;
    }
    return result;
}

#endif //OPTICALAB_DEPRECATED_H
