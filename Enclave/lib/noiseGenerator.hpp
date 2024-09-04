#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H
#include <sgx_tcrypto.h>
#include <sgx_trts.h>

#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "Enclave_t.h"
#include "api.h"

using namespace std;

int32_t modular_inverse(int32_t a, int32_t p);
vector<vector<int32_t>> multiplicative_noise_generate(int rows, int cols,
                                                      int32_t p);
vector<vector<int32_t>> r_noise_generate(int rows, int cols, int32_t p);
vector<vector<int32_t>> s_noise_generate(int rows, int cols, int32_t p);
vector<vector<int32_t>> noise_inverse(vector<vector<int32_t>> &eta, int32_t p);

#endif