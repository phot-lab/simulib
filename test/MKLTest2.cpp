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
 * Created: 2022/5/9
 * Supported by: National Key Research and Development Program of China
 */

/* C source code is found in dgemm_example.c */

#include "mkl.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main() {
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    m = 1000, k = 1000, n = 1000;
    alpha = 1.0;
    beta  = 0.0;

    A = (double *) mkl_malloc(m * k * sizeof(double), 64);
    B = (double *) mkl_malloc(k * n * sizeof(double), 64);
    C = (double *) mkl_malloc(m * n * sizeof(double), 64);

    if (A == NULL || B == NULL || C == NULL) {
        printf("\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
        mkl_free(A);
        mkl_free(B);
        mkl_free(C);
        return 1;
    }

    for (i = 0; i < (m * k); i++) {
        A[i] = (double) (i + 1);
    }

    for (i = 0; i < (k * n); i++) {
        B[i] = (double) (-i - 1);
    }

    for (i = 0; i < (m * n); i++) {
        C[i] = 0.0;
    }

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, A, k, B, n, beta, C, n);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    long long duration_ms = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    double time           = (double) duration_ms / 1000;
    cout << "运行时间: " << time << "s" << endl;

    return 0;
}