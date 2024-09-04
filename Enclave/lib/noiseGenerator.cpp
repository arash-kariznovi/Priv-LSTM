#include "noiseGenerator.hpp"

// #define PRINT

vector<vector<int>> multiplicative_noise_generate(int rows, int cols,
                                                  int32_t p) {
   random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1, p - 1);

  vector<vector<int>> noise(rows, vector<int>(cols, 0));

  for (size_t i = 0; i < cols; i++) {
    int temp = dis(gen);
    for (size_t j = 0; j < rows; j++) {
      noise[j][i] += temp;
    }
  }


    return noise;
}

vector<vector<int32_t>> r_noise_generate(int rows, int cols, int32_t p) {
     random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1, p - 1);

  vector<vector<int32_t>> noise(rows, vector<int32_t>(cols, 0));

  for (size_t i = 0; i < rows; i++) {
    int32_t temp = dis(gen);
    for (size_t j = 0; j < cols; j++) {
      noise[i][j] = temp;
    }
  }
    return noise;
}

vector<vector<int32_t>> s_noise_generate(int rows, int cols, int32_t p) {
   random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1, p - 1);

  vector<vector<int32_t>> noise(rows, vector<int32_t>(cols, 0));

  for (size_t i = 0; i < cols; i++) {
    int32_t temp = dis(gen);
    for (size_t j = 0; j < rows; j++) {
      noise[j][i] = temp;
    }
  }
    return noise;
}

vector<vector<int>> noise_inverse(vector<vector<int>> &eta, int32_t p) {
    vector<vector<int>> noise_inv(eta[0].size(), vector<int>(eta.size(), 0));

    for (size_t j = 0; j < eta[0].size(); j++) {
        int m = modular_inverse(eta[0][j], p);
        for (size_t i = 0; i < eta.size(); i++) {
            noise_inv[j][i] = m;
        }
    }

    return noise_inv;
}

int modular_inverse(int a, int p) {
    int t = 0, new_t = 1;
    int r = p, new_r = a;
    // ///????
    while (new_r != 0) {
        int quotient = r / new_r;
        t = t - quotient * new_t;
        swap(t, new_t);
        r = r - quotient * new_r;
        swap(r, new_r);
    }

    if (r > 1) {
        throw runtime_error("No modular inverse exists");
    }

    if (t < 0) {
        t = t + p;
    }

    return t;
}
