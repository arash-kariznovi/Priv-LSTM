#include <cstring>

#include "Enclave_t.h"

#include "lib/algebra_field.h"
#include "lib/api.h"
#include "lib/enclave_utils.h"
#include "lib/freivalds.h"
#include "lib/noiseGenerator.hpp"
#include "lib/quantizer.hpp"

vector<vector<double>> X;
vector<vector<int32_t>> X_finite_field;
vector<vector<int64_t>> noisy_X;

vector<vector<int>> eta_prime;
vector<vector<int>> eta;
vector<vector<int32_t>> s;
vector<vector<int32_t>> r;
vector<vector<int64_t>> nsr;
//

vector<vector<double>> Wf;
vector<vector<double>> Wi;
vector<vector<double>> Wc;
vector<vector<double>> Wo;

vector<vector<int64_t>> noisy_Wf;
vector<vector<int64_t>> noisy_Wi;
vector<vector<int64_t>> noisy_Wc;
vector<vector<int64_t>> noisy_Wo;

vector<vector<double>> b;

vector<vector<int64_t>> sWf(W_ROW, vector<int64_t>(W_COL, 0));
vector<vector<int64_t>> sWi(W_ROW, vector<int64_t>(W_COL, 0));
vector<vector<int64_t>> sWc(W_ROW, vector<int64_t>(W_COL, 0));
vector<vector<int64_t>> sWo(W_ROW, vector<int64_t>(W_COL, 0));

vector<vector<double>> input_gate;
vector<vector<double>> output_gate;
vector<vector<double>> candidate;
vector<vector<double>> forget_gate;

vector<vector<double>> ht;
vector<vector<double>> ct;
vector<vector<double>> ct_1;

#define INTG
#define INTG_PRINT

void ecall_offline_phase(void) {
    ocall_print("init offline..\n");
    // Generate Multiplicative noise (eta*eta_prime % p = 1) (precomputed)
    eta = multiplicative_noise_generate(W_ROW, W_COL, p);
    eta_prime = noise_inverse(eta, p);
    // Generate Additive noise (precomputed)
    r = r_noise_generate(W_ROW, W_COL, p);
    s = s_noise_generate(X_ROW, X_COL, p);

    generateRandomMatrix(X_ROW, X_COL, X);

    generateRandomMatrix(W_ROW, X_COL, b);
    generateRandomMatrix(W_ROW, X_COL, ct_1);

    // random W generation:
    generateRandomMatrix(W_ROW, W_COL, Wf);
    generateRandomMatrix(W_ROW, W_COL, Wi);
    generateRandomMatrix(W_ROW, W_COL, Wc);
    generateRandomMatrix(W_ROW, W_COL, Wo);
    // ocall_print("W:\n");
    //     ocall_print_double(Wf[0][0]);
    //     ocall_print_double(Wf[0][1]);
    //     ocall_print_double(Wf[1][0]);
    //     ocall_print_double(Wf[1][1]);
    // Quantize W (precomputed)
    vector<vector<int32_t>> Wf_fixed = quantizeToFixedPoint(Wf, l);
    vector<vector<int32_t>> Wi_fixed = quantizeToFixedPoint(Wi, l);
    vector<vector<int32_t>> Wc_fixed = quantizeToFixedPoint(Wc, l);
    vector<vector<int32_t>> Wo_fixed = quantizeToFixedPoint(Wo, l);

    vector<vector<int32_t>> Wf_finite_field =
        transferToFiniteField(Wf_fixed, p);
    vector<vector<int32_t>> Wi_finite_field =
        transferToFiniteField(Wi_fixed, p);
    vector<vector<int32_t>> Wc_finite_field =
        transferToFiniteField(Wc_fixed, p);
    vector<vector<int32_t>> Wo_finite_field =
        transferToFiniteField(Wo_fixed, p);

    // Add noise to W (precomputed)
    noisy_Wf =
        elementMultiplication(matrixAddition32(Wf_finite_field, r, p), eta, p);
    noisy_Wi =
        elementMultiplication(matrixAddition32(Wi_finite_field, r, p), eta, p);
    noisy_Wc =
        elementMultiplication(matrixAddition32(Wc_finite_field, r, p), eta, p);
    noisy_Wo =
        elementMultiplication(matrixAddition32(Wo_finite_field, r, p), eta, p);

    // future needs
    vector<vector<int64_t>> Wf_sum = compute_W_sum(Wf_finite_field, p);
    vector<vector<int64_t>> Wi_sum = compute_W_sum(Wi_finite_field, p);
    vector<vector<int64_t>> Wc_sum = compute_W_sum(Wc_finite_field, p);
    vector<vector<int64_t>> Wo_sum = compute_W_sum(Wo_finite_field, p);

    sWf = elementMultiplication(Wf_sum, s, p);
    sWi = elementMultiplication(Wi_sum, s, p);
    sWc = elementMultiplication(Wc_sum, s, p);
    sWo = elementMultiplication(Wo_sum, s, p);

    //
    vector<vector<int64_t>> sr(W_ROW, vector<int64_t>(X_COL, 0));
    sr = elementMultiplication(r, s, p);
    nsr = constMultiplyreturn(sr, W_COL, p);

    ocall_print("end offline..\n");
    return;
}

void ecall_online_phase(char* Yn, size_t size, char w_type) {
    //  ocall_print("init online..\n");
    vector<vector<int64_t>> Y_noisy = deserialize2DVector(Yn, W_ROW, X_COL);

    int k = 2;
    int64_t s = (1<<20);
    bool integrity_result;

    switch (w_type) {
        case 'f':
            integrity_result = freivalds(noisy_Wf, noisy_X, Y_noisy, s, k);
#ifdef INTG_PRINT
            integrity_result == 1 ? ocall_print("integrity verified!\n")
                                  : ocall_print("value corrupted!\n");
#endif
            break;
        case 'i':
            integrity_result = freivalds(noisy_Wi, noisy_X, Y_noisy, s, k);
#ifdef INTG_PRINT
            integrity_result == 1 ? ocall_print("integrity verified!\n")
                                  : ocall_print("value corrupted!\n");
#endif
            break;
        case 'c':
            integrity_result = freivalds(noisy_Wc, noisy_X, Y_noisy, s, k);
#ifdef INTG_PRINT
            integrity_result == 1 ? ocall_print("integrity verified!\n")
                                  : ocall_print("value corrupted!\n");
#endif
            break;
        case 'o':
            integrity_result = freivalds(noisy_Wo, noisy_X, Y_noisy, s, k);
#ifdef INTG_PRINT
            integrity_result == 1 ? ocall_print("integrity verified!\n")
                                  : ocall_print("value corrupted!\n");
#endif
            break;
        default:
            integrity_result = freivalds(noisy_Wf, noisy_X, Y_noisy, s, k);
#ifdef INTG_PRINT
            integrity_result == 1 ? ocall_print("integrity verified!\n")
                                  : ocall_print("value corrupted!\n");
#endif
            break;
    }

    vector<int64_t> X_sum_temp;
    vector<vector<int64_t>> X_sum(W_ROW, vector<int64_t>(X_COL, 0));
    vector<vector<int64_t>> rX(X_ROW, vector<int64_t>(X_COL, 0));
    for (int j = 0; j < X_COL; j++) {
        int64_t sum = 0;
        for (int i = 0; i < X_ROW; i++) {
            sum += X_finite_field[i][j] % p;
        }
        X_sum_temp.push_back(sum % p);
    }
    for (int i = 0; i < W_ROW; i++) {
        for (int j = 0; j < X_COL; j++) {
            X_sum[i][j] = X_sum_temp[j];
        }
    }

    rX = elementMultiplication(X_sum, r, p);

    vector<vector<int64_t>> rX_sW;
    switch (w_type) {
        case 'f':
            rX_sW = matrixAddition(rX, sWf, p);
            break;
        case 'i':
            rX_sW = matrixAddition(rX, sWi, p);
            break;
        case 'c':
            rX_sW = matrixAddition(rX, sWc, p);
            break;
        case 'o':
            rX_sW = matrixAddition(rX, sWo, p);
            break;
        default:
            rX_sW = matrixAddition(rX, sWf, p);
            break;
    }

    vector<vector<int64_t>> rX_sW_2sr = matrixAddition(rX_sW, nsr, p);
    vector<vector<int64_t>> Y_extracted =
        matrixSubstraction(Y_noisy, rX_sW_2sr, p);
    // Dequantize back to floating-point
    vector<vector<int64_t>> y_fixed_point =
        transferFromFiniteField(Y_extracted, p);

    // activation function
    switch (w_type) {
        case 'f':
            forget_gate = dequantizeToFloatingPoint(y_fixed_point, l);
            forget_gate = addDoubleMatrix(forget_gate, b);
            sigmoidMatrix(forget_gate);
            break;
        case 'i':
            input_gate = dequantizeToFloatingPoint(y_fixed_point, l);
            input_gate = addDoubleMatrix(input_gate, b);
            sigmoidMatrix(input_gate);
            break;
        case 'c':
            candidate = dequantizeToFloatingPoint(y_fixed_point, l);
            candidate = addDoubleMatrix(candidate, b);
            tanhMatrix(candidate);
            break;
        case 'o':
            output_gate = dequantizeToFloatingPoint(y_fixed_point, l);
            output_gate = addDoubleMatrix(output_gate, b);
            sigmoidMatrix(output_gate);
            break;
        default:
            forget_gate = dequantizeToFloatingPoint(y_fixed_point, l);
            forget_gate = addDoubleMatrix(forget_gate, b);
            sigmoidMatrix(forget_gate);
            break;
    }

    // in other functions
    // Ct = candidate * input_gate + Ct-1 * forget_gate
    // Ot = output_gate * tanh(Ct)

    // ocall_print("Y: \n");
    // ocall_print_double(Y_dequantized[20][30]);
    // ocall_print_double(Y_dequantized[190][1]);
    // ocall_print_double(Y_dequantized[1][230]);
    // ocall_print_double(Y_dequantized[134][112]);
    return;
}

void ecall_activation_function(void) {
    ct = addDoubleMatrix(multiplyDouble(candidate, input_gate),
                         multiplyDouble(ct_1, forget_gate));
    tanhMatrix(ct);
    ht = multiplyDouble(ct, output_gate);
}

void ecall_compute_y() {
    forget_gate = dotDouble(Wf, X);
    forget_gate = addDoubleMatrix(forget_gate, b);
    sigmoidMatrix(forget_gate);

    input_gate = dotDouble(Wi, X);
    input_gate = addDoubleMatrix(input_gate, b);
    sigmoidMatrix(input_gate);

    candidate = dotDouble(Wc, X);
    candidate = addDoubleMatrix(candidate, b);
    tanhMatrix(candidate);

    output_gate = dotDouble(Wo, X);
    output_gate = addDoubleMatrix(output_gate, b);
    sigmoidMatrix(output_gate);

    ct = addDoubleMatrix(multiplyDouble(candidate, input_gate),
                         multiplyDouble(ct_1, forget_gate));
    tanhMatrix(ct);
    ht = multiplyDouble(ct, output_gate);

    //   ocall_print_double(Y[3][3]);
    return;
}

void ecall_get_W_n(char* Wnoisy, size_t size, char w_type) {
    Serialized2DVector serialized_WN;
    switch (w_type) {
        case 'f':
            serialized_WN = serialize2DVector(noisy_Wf);
            break;
        case 'i':
            serialized_WN = serialize2DVector(noisy_Wi);
            break;
        case 'c':
            serialized_WN = serialize2DVector(noisy_Wc);
            break;
        case 'o':
            serialized_WN = serialize2DVector(noisy_Wo);
            break;
        default:
            serialized_WN = serialize2DVector(noisy_Wf);
            break;
    }

    // ocall_print_int64(noisy_W[0][0]);
    serialized_WN.rows = W_ROW;
    serialized_WN.cols = W_COL;
    serialized_WN.size = size;

    memcpy(Wnoisy, serialized_WN.data, serialized_WN.size);

    delete[] serialized_WN.data;
    return;
}

void ecall_get_X_n(char* Xnoisy, size_t size) {
    // Quantize X (online)
    // ocall_print("X:\n");
    // ocall_print_double(X[0][0]);
    //  ocall_print_double(X[0][1]);
    //   ocall_print_double(X[1][0]);
    //    ocall_print_double(X[1][1]);
    vector<vector<int32_t>> X_fixed = quantizeToFixedPoint(X, l);
    X_finite_field = transferToFiniteField(X_fixed, p);

    // Add noise to X (online)
    noisy_X = elementMultiplication(matrixAddition32(X_finite_field, s, p),
                                    eta_prime, p);

    Serialized2DVector serialized_XN = serialize2DVector(noisy_X);

    serialized_XN.rows = X_ROW;
    serialized_XN.cols = X_COL;
    serialized_XN.size = size;

    memcpy(Xnoisy, serialized_XN.data, serialized_XN.size);

    delete[] serialized_XN.data;
    return;
}