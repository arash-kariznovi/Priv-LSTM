#include <random>
#include <vector>
#include "api.h"
#include "Enclave_t.h"

using namespace std;


vector<vector<int64_t>> compute_W_sum(vector<vector<int32_t>>& W_finite_field,
                                      int32_t p) {
    vector<int64_t> W_sum_temp;
    vector<vector<int64_t>> W_sum(W_ROW, vector<int64_t>(X_COL, 0));

    for (int i = 0; i < W_ROW; i++) {
        int64_t sum = 0;
        for (int j = 0; j < W_COL; j++) {
            sum += W_finite_field[i][j] % p;
        }
        W_sum_temp.push_back(sum % p);
    }
    for (int i = 0; i < W_ROW; i++) {
        for (int j = 0; j < X_COL; j++) {
            W_sum[i][j] = W_sum_temp[i];
        }
    }

    return W_sum;
}


// Serialize the 2D vector into a byte buffer
Serialized2DVector serialize2DVector(
    const std::vector<std::vector<int64_t>>& vec) {
    Serialized2DVector serialized;
    serialized.rows = vec.size();
    serialized.cols = vec.empty() ? 0 : vec[0].size();
    serialized.size = serialized.rows * serialized.cols * sizeof(int64_t);
    serialized.data = new char[serialized.size];

    size_t index = 0;
    for (const auto& row : vec) {
        std::memcpy(serialized.data + index, row.data(),
                    row.size() * sizeof(int64_t));
        index += row.size() * sizeof(int64_t);
    }

    return serialized;
}

// Deserialize the byte buffer into a 2D vector
std::vector<std::vector<int64_t>> deserialize2DVector(char* data, size_t rows,
                                                      size_t cols) {
    std::vector<std::vector<int64_t>> vec(rows, std::vector<int64_t>(cols));
    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        std::memcpy(vec[i].data(), data + index, cols * sizeof(int64_t));
        index += cols * sizeof(int64_t);
    }
    return vec;
}