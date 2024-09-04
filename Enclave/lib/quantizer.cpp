#include "quantizer.hpp"

#include <random>

// quantize floating-point to fixed-point

vector<vector<int32_t>> quantizeToFixedPoint(const vector<vector<double>> &A,
                                             int l) {
    vector<vector<int32_t>> A_fixed(A.size(), vector<int32_t>(A[0].size(), 0));

    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[0].size(); j++) {
            A_fixed[i][j] = round(A[i][j] * (1 << l));
        }
    }

    return A_fixed;
}

// fixed-point to a finite field p
vector<vector<int32_t>> transferToFiniteField(
    const vector<vector<int32_t>> &A_fixed, int32_t p) {
    vector<vector<int32_t>> A_finite_field(A_fixed.size(),
                                           vector<int32_t>(A_fixed[0].size()));

    for (size_t i = 0; i < A_fixed.size(); ++i) {
        for (size_t j = 0; j < A_fixed[i].size(); ++j) {
            int32_t value = A_fixed[i][j];
            if (value < 0) {
                value += p;
            }
            A_finite_field[i][j] = value % p;
        }
    }

    return A_finite_field;
}

// fixed-point to a finite field p

vector<vector<int64_t>> transferFromFiniteField(
    const vector<vector<int64_t>> &A_finite_field, int32_t p) {
    vector<vector<int64_t>> A_fixed(A_finite_field.size(),
                                    vector<int64_t>(A_finite_field[0].size()));

    for (size_t i = 0; i < A_finite_field.size(); ++i) {
        for (size_t j = 0; j < A_finite_field[i].size(); ++j) {
            int64_t value = A_finite_field[i][j];
            if (value > p / 2) {
                value -= p;
            }
            A_fixed[i][j] = value;
        }
    }

    return A_fixed;
}

// dequantize fixed-point back to floating-point

vector<vector<double>> dequantizeToFloatingPoint(
    const vector<vector<int64_t>> &A_fixed, int l) {
    vector<vector<double>> A_float(A_fixed.size(),
                                   vector<double>(A_fixed[0].size()));

    for (size_t i = 0; i < A_fixed.size(); ++i) {
        for (size_t j = 0; j < A_fixed[i].size(); ++j) {
            A_float[i][j] = static_cast<double>(A_fixed[i][j]) / (1 << 2 * l);
        }
    }

    return A_float;
}

void generateRandomMatrix(int rows, int cols, vector<vector<double>> &matrix) {
    matrix.resize(rows);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    for (int i = 0; i < rows; ++i) {
        matrix[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
}


void show_vector(vector<vector<int64_t>> &matrix) {
    size_t row = matrix.size();
    size_t col = matrix[0].size();

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            ocall_print_int64(matrix[i][j]);
            ocall_print(" ");
        }
        ocall_print("\n");
    }
    return;
}