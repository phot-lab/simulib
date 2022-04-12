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

#include "SimuLib"

using namespace std;

VectorXd diff(const VectorXd &v) {
    auto block_size = v.size() - 1;
    return v.block(1, 0, block_size, 1) - v.block(0, 0, block_size, 1);
}

RowVectorXd genVector(const double &start, const double &end) {
    if (start > end) {
        RowVectorXd vec(0);
        return vec;
    }
    auto size = (unsigned long) ((end - start) + 1);
    RowVectorXd res(size);
    double cur = start;
    for (unsigned i = 0; i < size; ++i) {
        res(i) = cur++;
    }
    return res;
}

RowVectorXd genStepVector(const double &start, const double &step, const double &end) {
    auto size = (unsigned long) ((end - start) / step + 1);
    RowVectorXd res(size);
    double cur = start;
    for (unsigned i = 0; i < size; ++i) {
        res(i) = cur;
        cur += step;
    }
    return res;
}

VectorXd removeZero(const VectorXd &a, const VectorXd &b) {
    assert(a.size() == b.size());
    vector<double> vec;
    for (Index i = 0; i < a.size(); ++i) {
        if (b[i] != 0) vec.emplace_back(a[i]);
    }
    return stlToEigen(vec);
}

MatrixXcd fastExp(const MatrixXcd &m) {
    return (m * 1i).array().exp();
}

void setValueIndices(VectorXd &vec, const VectorXd &indices, double value) {
    for (const auto &item: indices) {
        vec((Index) (item - 1)) = value;
    }
}

void replaceVector(VectorXd &vec, VectorXd indices, VectorXd replace) {
    assert(indices.size() == replace.size());
    for (Index i = 0; i < indices.size(); ++i) {
        vec((Index) indices(i)) = replace(i);
    }
}

VectorXd modulusEigen(const VectorXd &v, int n) {
    return v.unaryExpr([&](const int x) { return (double) (x - floor((double) x / n) * n); });
}

VectorXd nModulusEigen(VectorXd v, int n) {
    v = v.array() - 1 - n;
    v = modulusEigen(v, n);
    return v.array() + 1;
}

RowVectorXd maxRow(MatrixXd m) {
    VectorXd v = sumRow(m);
    double max = v.maxCoeff();
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == max)
            return m.row(i);
    }
    RowVectorXd res;
    return res;
}

VectorXd maxCol(MatrixXd m) {
    RowVectorXd v = sumCol(m);
    double max    = v.maxCoeff();
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == max)
            return m.col(i);
    }
    VectorXd res;
    return res;
}

RowVectorXcd maxRow(MatrixXcd m) {
    VectorXcd v         = sumRow(m);
    complex<double> max = v.redux([](const complex<double> &a, const complex<double> &b) {
        if (a.real() < b.real())
            return b;
        return a;
    });
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == max)
            return m.row(i);
    }
    RowVectorXcd res;
    return res;
}

VectorXcd maxCol(MatrixXcd m) {
    RowVectorXcd v      = sumCol(m);
    complex<double> max = v.redux([](const complex<double> &a, const complex<double> &b) {
        if (a.real() < b.real())
            return b;
        return a;
    });
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == max)
            return m.col(i);
    }
    VectorXcd res;
    return res;
}

RowVectorXd minRow(MatrixXd m) {
    VectorXd v = sumRow(m);
    double min = v.minCoeff();
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == min)
            return m.row(i);
    }
    RowVectorXd res;
    return res;
}

VectorXd minCol(MatrixXd m) {
    RowVectorXd v = sumCol(m);
    double min    = v.minCoeff();
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == min)
            return m.col(i);
    }
    VectorXd res;
    return res;
}

RowVectorXcd minRow(MatrixXcd m) {
    VectorXcd v         = sumRow(m);
    complex<double> min = v.redux([](const complex<double> &a, const complex<double> &b) {
        if (a.real() < b.real())
            return a;
        return b;
    });
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == min)
            return m.row(i);
    }
    RowVectorXcd res;
    return res;
}

VectorXcd minCol(MatrixXcd m) {
    RowVectorXcd v      = sumCol(m);
    complex<double> min = v.redux([](const complex<double> &a, const complex<double> &b) {
        if (a.real() < b.real())
            return a;
        return b;
    });
    for (Index i = 0; i < v.size(); ++i) {
        if (v(i) == min)
            return m.col(i);
    }
    VectorXcd res;
    return res;
}

VectorXd sortEigen(VectorXd v, bool ascend) {
    if (ascend)
        std::sort(v.begin(), v.end(), [](double lhs, double rhs) { return rhs > lhs; });
    else
        std::sort(v.begin(), v.end(), [](double lhs, double rhs) { return rhs < lhs; });
    return v;
}

VectorXcd sortEigen(VectorXcd v, bool ascend) {
    if (ascend)
        std::sort(v.begin(), v.end(), [](complex<double> lhs, complex<double> rhs) { return rhs.real() > lhs.real(); });
    else
        std::sort(v.begin(), v.end(), [](complex<double> lhs, complex<double> rhs) { return rhs.real() < lhs.real(); });
    return v;
}

MatrixXcd mvProduct(MatrixXcd m, const VectorXcd &v) {
    MatrixXcd res(m.rows(),m.cols());
    for (Index i = 0; i < m.cols(); ++i) {
        res.col(i) = m.col(i).cwiseProduct(v);
    }
    return res;
}