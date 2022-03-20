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
 * Created: 2022/3/18
 * Supported by: National Key Research and Development Program of China
 */

/* C source code is found in dgemm_example.c */

#include "mkl.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <simulib>

using namespace std;

int min(int a, int b) {
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}

int main() {
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    const int length = 6000;

    printf("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
           " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
           " alpha and beta are double precision scalars\n\n");

    m = length, k = length, n = length;
    printf(" Initializing data for matrix multiplication C=A*B for matrix \n"
           " A(%ix%i) and matrix B(%ix%i)\n\n",
           m, k, k, n);
    alpha = 1.0;
    beta  = 0.0;

    printf(" Allocating memory for matrices aligned on 64-byte boundary for better \n"
           " performance \n\n");

    A = (double *) mkl_malloc(m * k * sizeof(double), 64);
    B = (double *) mkl_malloc(k * n * sizeof(double), 64);
    C = (double *) mkl_malloc(m * n * sizeof(double), 64);
    if (A == nullptr || B == nullptr || C == nullptr) {
        printf("\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
        mkl_free(A);
        mkl_free(B);
        mkl_free(C);
        return 1;
    }

    //    printf(" Intializing matrix data \n\n");
    for (i = 0; i < (m * k); i++) {
        A[i] = (double) (i + 1);
    }

    for (i = 0; i < (k * n); i++) {
        B[i] = (double) (-i - 1);
    }

    for (i = 0; i < (m * n); i++) {
        C[i] = 0.0;
    }

    MatrixXd m1 = MatrixXd ::Random(length, length);
    MatrixXd m2 = MatrixXd ::Random(length, length);
    MatrixXd m3(length, length);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    //    printf(" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, m1.data(), k, m2.data(), n, beta, m3.data(), n);
    //    printf("\n Computations completed.\n\n");
    //
    //    printf(" Top left corner of matrix A: \n");
    //    for (i = 0; i < min(m, 6); i++) {
    //        for (j = 0; j < min(k, 6); j++) {
    //            printf("%12.0f", A[j + i * k]);
    //        }
    //        printf("\n");
    //    }
    //
    //    printf("\n Top left corner of matrix B: \n");
    //    for (i = 0; i < min(k, 6); i++) {
    //        for (j = 0; j < min(n, 6); j++) {
    //            printf("%12.0f", B[j + i * n]);
    //        }
    //        printf("\n");
    //    }
    //
    //    printf("\n Top left corner of matrix C: \n");
    //    for (i = 0; i < min(m, 6); i++) {
    //        for (j = 0; j < min(n, 6); j++) {
    //            printf("%12.5G", C[j + i * n]);
    //        }
    //        printf("\n");
    //    }

    //    printf("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    long long duration_ms                = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    double time                          = (double) duration_ms / 1000;
    cout << "运行时间: " << time << "s" << endl;

    //    printf(" Example completed. \n\n");
    return 0;
}