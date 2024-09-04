#include <climits>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

//  linear operations

template <typename T>
vector<vector<int64_t>> matrixMultiplication(const vector<vector<T>> &A,
                                             const vector<vector<T>> &B,
                                             int32_t pr) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    vector<vector<int64_t>> C(m, vector<int64_t>(p, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
                // C[i][j] += ((A[i][k] * B[k][j]) % pr);
                C[i][j] = (C[i][j] + (A[i][k] * B[k][j]) % pr) % pr;
            }

            if (C[i][j] < 0) {
                C[i][j] += pr;
            }
        }
    }
    return C;
}

template <typename T>
vector<vector<int64_t>> matrixAddition(const vector<vector<T>> &A,
                                       const vector<vector<T>> &B, int32_t p) {
    int rows = A.size();
    int cols = A[0].size();  // Assuming both matrices have the same dimensions

    vector<vector<int64_t>> C(rows, vector<int64_t>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            C[i][j] = ((int64_t)A[i][j] + B[i][j]) % p;
            // C[i][j] = ((__int128_t)(A[i][j]) + B[i][j]) % p;
            // if (C[i][j] < 0) {
            //   C[i][j] += p;
            // }
        }
    }

    return C;
}

vector<vector<int64_t>> matrixAddition32(const vector<vector<int32_t>> &A,
                                         const vector<vector<int32_t>> &B,
                                         int32_t p) {
    int rows = A.size();
    int cols = A[0].size();  // Assuming both matrices have the same dimensions

    // Initialize result matrix C with size rows x cols
    vector<vector<int64_t>> C(rows, vector<int64_t>(cols, 0));

    // Perform vector addition
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // C[i][j] = ((int64_t)(A[i][j]) + B[i][j]) % p;
            C[i][j] = ((int64_t)A[i][j] + B[i][j]) % p;
            if (C[i][j] < 0) {
                C[i][j] += p;
            }
        }
    }

    return C;
}

vector<vector<int64_t>> matrixSubstraction(const vector<vector<int64_t>> &A,
                                           const vector<vector<int64_t>> &B,
                                           int32_t p) {
    int rows = A.size();
    int cols = B[0].size();  // Assuming both matrices have the same dimensions

    // Initialize result matrix C with size rows x cols
    vector<vector<int64_t>> C(rows, vector<int64_t>(cols, 0));

    // Perform vector addition
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // C[i][j] = (A[i][j] - B[i][j]) % p;
            // if (C[i][j] < 0) {
            //   C[i][j] += p;
            // }

            C[i][j] = (A[i][j] - B[i][j]);
            if (C[i][j] < 0) {
                C[i][j] += p;
            }
        }
    }

    return C;
}

vector<vector<double>> Substraction(const vector<vector<double>> &A,
                                    const vector<vector<double>> &B) {
    int rows = A.size();
    int cols = B[0].size();  // Assuming both matrices have the same dimensions

    // Initialize result matrix C with size rows x cols
    vector<vector<double>> C(rows, vector<double>(cols, 0));

    // Perform vector addition
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            C[i][j] = abs(A[i][j] - B[i][j]);
        }
    }

    return C;
}
template <typename T>
vector<vector<int64_t>> elementMultiplication(const vector<vector<T>> &A,
                                              const vector<vector<int32_t>> &B,
                                              int32_t p) {
    vector<vector<int64_t>> result(A.size(), vector<int64_t>(A[0].size(), 0.0));

    // Perform element-wise multiplication
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < A[0].size(); ++j) {
            result[i][j] = ((int64_t)A[i][j] * B[i][j]) % p;
            // result[i][j] = ((int64_t)A[i][j] * B[i][j]) % p;
            // if (result[i][j] < 0) {
            //   result[i][j] += p;
            // }
        }
    }

    return result;
}

vector<vector<int64_t>> constMultiplyreturn(vector<vector<int64_t>> &A, int c,
                                            int32_t p) {
    vector<vector<int64_t>> result(A.size(), vector<int64_t>(A[0].size(), 0.0));

    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < A[0].size(); ++j) {
            result[i][j] = (((int64_t)A[i][j]) * c) % p;
            // result[i][j] = ((__int128_t)(A[i][j]) * c) % p;
            if (result[i][j] < 0) {
                result[i][j] += p;
            }
        }
    }

    return result;
}

//  activation functions

double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

void sigmoidMatrix(vector<vector<double>> &vec) {
    for (auto &row : vec) {
        for (auto &element : row) {
            element = sigmoid(element);
        }
    }
}

void tanhMatrix(vector<vector<double>> &vec) {
    for (auto &row : vec) {
        for (auto &element : row) {
            element = tanh(element);
        }
    }
}

// Double arithmatic

vector<vector<double>> dotDouble(const vector<vector<double>> &A,
                                 const vector<vector<double>> &B) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    vector<vector<double>> C(m, vector<double>(p, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += ((A[i][k] * B[k][j]));
            }
        }
    }
    return C;
}

vector<vector<double>> multiplyDouble(const vector<vector<double>> &vec1,
                                      const vector<vector<double>> &vec2) {
    vector<vector<double>> result(vec1.size(),
                                  vector<double>(vec1[0].size(), 0.0));
    for (size_t i = 0; i < vec1.size(); ++i) {
        for (size_t j = 0; j < vec1[0].size(); ++j) {
            result[i][j] = vec1[i][j] * vec2[i][j];
        }
    }

    return result;
}

vector<vector<double>> addDoubleMatrix(const vector<vector<double>> &A,
                                      const vector<vector<double>> &B) {
    int rows = A.size();
    int cols = A[0].size();

    vector<vector<double>> C(rows, vector<double>(cols, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}