#ifndef QUANTIZER_H
#define QUANTIZER_H
#include <sgx_tcrypto.h>
#include <sgx_trts.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "Enclave_t.h"
#include "api.h"

using namespace std;
const int po = 25;

void generateRandomMatrix(int rows, int cols, vector<vector<double>> &matrix);

vector<vector<int32_t>> quantizeToFixedPoint(const vector<vector<double>> &A,
                                             int l);
vector<vector<int32_t>> transferToFiniteField(
    const vector<vector<int32_t>> &A_fixed, int32_t p);

vector<vector<int64_t>> transferFromFiniteField(
    const vector<vector<int64_t>> &A_fixed, int32_t p);

vector<vector<double>> dequantizeToFloatingPoint(
    const vector<vector<int64_t>> &A_fixed, int l);


void show_vector(vector<vector<int64_t>> &matrix);

#endif