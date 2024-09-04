#include "freivalds.h"

#include "Enclave_t.h"

bool freivalds(vector<vector<int64_t>> &A, vector<vector<int64_t>> &B,
               vector<vector<int64_t>> &C, int64_t s, int k) {
    for (int i = 0; i < k; i++) {
        // printf("round %d \n", i+1);
        if (isProduct(A, B, C, s) == false) {
            return false;
        }
    }
    return true;
}

bool isProduct(vector<vector<int64_t>> &A, vector<vector<int64_t>> &B,
               vector<vector<int64_t>> &C, int64_t s) {
    int rows = C.size();
    int cols = C[0].size();

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-s, s);

    vector<int64_t> r(rows, 0);
    for (int i = 0; i < rows; i++) {
        r[i] = distrib(gen);
    }

    // x = r*C
    vector<int64_t> x(rows, 0);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            x[j] = (x[j] + (r[i] * C[i][j]) % prime) % prime;
        }
        if (x[j] < 0) {
            x[j] += prime;
        }
    }

    // y = r*A
    vector<int64_t> y(rows, 0);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            y[j] = (y[j] + (r[i] * A[i][j]) % prime) % prime;
        }
        if (y[j] < 0) {
            y[j] += prime;
        }
    }

    // z = y*r = r*A*B
    vector<int64_t> z(rows, 0);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            z[j] = (z[j] + (y[i] * B[i][j]) % prime) % prime;
        }
        if (z[j] < 0) {
            z[j] += prime;
        }
    }

    // r*C - r*A*B != 0 or ==0
    for (int j = 0; j < cols; j++) {
        // ocall_print_int64(x[j]);
        // ocall_print_int64(z[j]);
        if (x[j] - z[j] != 0) return false;
    }

    return true;
}