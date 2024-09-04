
#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>
#include <vector>
#include <functional>
using namespace std;

// Serialize the 2D vector into a byte buffer
Serialized2DVector serialize2DVector(
    const std::vector<std::vector<int64_t>> &vec) {
    Serialized2DVector serialized;
    serialized.rows = vec.size();
    serialized.cols = vec.empty() ? 0 : vec[0].size();
    serialized.size = serialized.rows * serialized.cols * sizeof(int64_t);
    serialized.data = new char[serialized.size];

    size_t index = 0;
    for (const auto &row : vec) {
        std::memcpy(serialized.data + index, row.data(),
                    row.size() * sizeof(int64_t));
        index += row.size() * sizeof(int64_t);
    }

    return serialized;
}

// Deserialize the byte buffer into a 2D vector
std::vector<std::vector<int64_t>> deserialize2DVector(const char *data,
                                                      size_t rows,
                                                      size_t cols) {
    std::vector<std::vector<int64_t>> vec(rows, std::vector<int64_t>(cols));
    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        std::memcpy(vec[i].data(), data + index, cols * sizeof(int64_t));
        index += cols * sizeof(int64_t);
    }
    return vec;
}

std::vector<std::vector<int32_t>> deserialize2DVector32(const char *data,
                                                      size_t rows,
                                                      size_t cols) {
    std::vector<std::vector<int32_t>> vec(rows, std::vector<int32_t>(cols));
    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        std::memcpy(vec[i].data(), data + index, cols * sizeof(int32_t));
        index += cols * sizeof(int32_t);
    }
    return vec;
}

std::vector<std::vector<int32_t>> convertToInt32Vector(
    vector<vector<__int128_t>> &oldVector) {
    size_t rows = oldVector.size();
    if (rows == 0) {
        return {};
    }
    size_t cols = oldVector[0].size();

    std::vector<std::vector<int32_t>> newVector(rows,
                                                std::vector<int32_t>(cols));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            newVector[i][j] = static_cast<int32_t>(oldVector[i][j]);
        }
    }

    return newVector;
}

template <typename T>
void print(vector<vector<T>> &A, const char *intro) {
    cout << intro << endl;
    for (const auto &row : A) {
        for (const auto &value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
    cout << endl;
}




void generateRandomMatrix(int rows, int cols, vector<vector<double>> &matrix) {
    matrix.resize(rows);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-2.0, 2.0);

    for (int i = 0; i < rows; ++i) {
        matrix[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
            // ocall_print_double(matrix[i][j]);
        }
    }
}



// Function to multiply a portion of the matrix
template <typename T>
void multiplyPortion(const std::vector<std::vector<T>> &A,
                     const std::vector<std::vector<T>> &B,
                     std::vector<std::vector<int64_t>> &C, int startRow,
                     int endRow, int32_t pr) {
    int n = A[0].size();
    int p = B[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] = (C[i][j] + (A[i][k] * B[k][j]) % pr) % pr;
            }
            if (C[i][j] < 0) {
                C[i][j] += pr;
            }
        }
    }
}



std::vector<std::vector<int64_t>> matrixMultiply(
    const std::vector<std::vector<int64_t>> &A, const std::vector<std::vector<int64_t>> &B,
    int32_t pr, int numThreads) {
    int m = A.size();
    std::vector<std::vector<int64_t>> C(m,
                                        std::vector<int64_t>(B[0].size(), 0));

    std::vector<std::thread> threads;
    int chunkSize = m / numThreads;

    for (int t = 0; t < numThreads; ++t) {
        int startRow = t * chunkSize;
        int endRow = (t == numThreads - 1) ? m : startRow + chunkSize;
        threads.push_back(std::thread(multiplyPortion<int64_t>, std::ref(A),
                                      std::ref(B), std::ref(C), startRow,
                                      endRow, pr));
    }

    for (auto &th : threads) {
        th.join();
    }

    return C;
}




void matrixMultiplicationSingleThread(
    const std::vector<std::vector<int64_t>> &A, const std::vector<std::vector<int64_t>> &B,
    int32_t pr, vector<vector<int64_t>> &C) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();


    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
    
                C[i][j] = (C[i][j] + (A[i][k] * B[k][j]) % pr) % pr;
            }

            if (C[i][j] < 0) {
                C[i][j] += pr;
            }
        }
    }

}



vector<std::vector<double>> matrixMultiplicationDouble(
    const std::vector<std::vector<double>> &A,
    const std::vector<std::vector<double>> &B) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    std::vector<std::vector<double>> C(m, std::vector<double>(p, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += ((A[i][k] * B[k][j]));
            }
        }
    }
    return C;
}

vector<vector<int64_t>> rebuild_matrix(int64_t *data, size_t len) {
    vector<vector<int64_t>> received_vector;

    // Deserialize the data back into a vector of vectors
    size_t pos = 0;
    while (pos < len) {
        size_t row_size = static_cast<size_t>(data[pos]);
        std::vector<int64_t> row(data + pos + 1, data + pos + 1 + row_size);
        received_vector.push_back(row);
        pos += row_size + 1;
    }

    return received_vector;
}

vector<vector<int64_t>> castTo64(vector<vector<__int128_t>> vec) {
    vector<vector<int64_t>> res(vec.size(), vector<int64_t>(vec[0].size(), 0));
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[0].size(); j++) {
            res[i][j] = static_cast<int64_t>(vec[i][j]);
        }
    }
    return res;
}


