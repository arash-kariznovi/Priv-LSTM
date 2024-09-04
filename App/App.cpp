#include "App.h"

#include <stdio.h>
#include <string.h>

#include <chrono>
#include <cstring>

#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include "Enclave_u.h"
#include "api.h"
#include "sgx_urts.h"
#include "utils.h"

// #define BASELINE
// #define MULTITHREAD
#define FOURWAY

using namespace std;
const int32_t p = (1 << 25) - 39;

#define ENCLAVE_FILENAME "enclave.signed.so"

sgx_enclave_id_t global_eid = 0;




void ocall_print(const char *str);
void ocall_print_double(double value);
void ocall_print_int64(int64_t value);
void ocall_print_int32(int32_t value);
void ocall_print_vector(double *vec, size_t row, size_t col);


// add different s...         ??

// W,X,b                   (randomized)

// benchmark:

//  single threaded in untrusted ?
//  multithread in untrusted ?
// GPU in untrusted ?

int SGX_CDECL main(int argc, char *argv[]) {
    (void)(argc);
    (void)(argv);

    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    // initialize enclave
    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, NULL, NULL,
                             &global_eid, NULL);
    if (ret != SGX_SUCCESS) {
        printf("Enclave init error\n");
        getchar();
        return -1;
    }

    // offline(Ws,r,s,eta,eta_prime,add noise to Ws)
    ret = ecall_offline_phase(global_eid);

    size_t W_size = W_ROW * W_COL * sizeof(int64_t);
    char *W_serialized = new char[W_size];
    // get Wf
    ret = ecall_get_W_n(global_eid, W_serialized, W_size, 'f');
    vector<vector<int64_t>> Wf =
        deserialize2DVector(W_serialized, W_ROW, W_COL);
    // get Wi
    ret = ecall_get_W_n(global_eid, W_serialized, W_size, 'i');
    vector<vector<int64_t>> Wi =
        deserialize2DVector(W_serialized, W_ROW, W_COL);
    // get Wc
    ret = ecall_get_W_n(global_eid, W_serialized, W_size, 'c');
    vector<vector<int64_t>> Wc =
        deserialize2DVector(W_serialized, W_ROW, W_COL);
    // get Wo
    ret = ecall_get_W_n(global_eid, W_serialized, W_size, 'o');
    vector<vector<int64_t>> Wo =
        deserialize2DVector(W_serialized, W_ROW, W_COL);
    delete[] W_serialized;

    ofstream File("inference_time.txt",ios::app);
    if(!File.is_open()){
        cerr << "file is not open." <<endl;
        return -1;
    }
    // ********************* online phase *********************** //
    // method 1

#ifdef BASELINE
    auto start = chrono::high_resolution_clock::now();
    ecall_compute_y(global_eid);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elasped = end - start;
    cout << "elasped time (baseline): " << elasped.count() << " ms" << endl;
    File << "baseline    "<< elasped.count() << endl;
    File.close();
#endif
    ///////////////////////////////////////////////////////////////////////////////
    // method 2

#ifdef MULTITHREAD
    auto start = chrono::high_resolution_clock::now();
    size_t X_size = X_ROW * X_COL * sizeof(int64_t);
    char *X_serialized = new char[X_size];
    ret = ecall_get_X_n(global_eid, X_serialized, X_size);
    vector<vector<int64_t>> Xn =
        deserialize2DVector(X_serialized, X_ROW, X_COL);

    delete[] X_serialized;

    vector<vector<int64_t>> yf1 = matrixMultiply(Wf, Xn, p, 2);
    vector<vector<int64_t>> yi1 = matrixMultiply(Wi, Xn, p, 2);
    vector<vector<int64_t>> yc1 = matrixMultiply(Wc, Xn, p, 2);
    vector<vector<int64_t>> yo1 = matrixMultiply(Wo, Xn, p, 2);

    Serialized2DVector y_serial = serialize2DVector(yf1);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'f');
    delete[] y_serial.data;

    y_serial = serialize2DVector(yi1);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'i');
    delete[] y_serial.data;

    y_serial = serialize2DVector(yc1);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'c');
    delete[] y_serial.data;

    y_serial = serialize2DVector(yo1);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'o');
    delete[] y_serial.data;
    ecall_activation_function(global_eid);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elasped2 = end - start;
    cout << "elasped time (multithread): " << elasped2.count() << " ms" << endl;
    File << "multithread    "<< elasped2.count() << endl;
    File.close();
#endif

///////////////////////////////////////////////////////////////////////////////
// method 3
#ifdef FOURWAY
    auto start = chrono::high_resolution_clock::now();
    size_t X_size1 = X_ROW * X_COL * sizeof(int64_t);
    char *X_serialized1 = new char[X_size1];
    ret = ecall_get_X_n(global_eid, X_serialized1, X_size1);
    vector<vector<int64_t>> Xn1 =
        deserialize2DVector(X_serialized1, X_ROW, X_COL);

// corrupt to test integrity check
    // Xn1[0][0] = 2;
    delete[] X_serialized1;
    start = chrono::high_resolution_clock::now();
    vector<vector<int64_t>> yf(W_ROW, vector<int64_t>(X_COL, 0));
    vector<vector<int64_t>> yi(W_ROW, vector<int64_t>(X_COL, 0));
    vector<vector<int64_t>> yc(W_ROW, vector<int64_t>(X_COL, 0));
    vector<vector<int64_t>> yo(W_ROW, vector<int64_t>(X_COL, 0));
    thread t5(matrixMultiplicationSingleThread, ref(Wf), ref(Xn1), p, ref(yf));
    thread t6(matrixMultiplicationSingleThread, ref(Wi), ref(Xn1), p, ref(yi));
    thread t7(matrixMultiplicationSingleThread, ref(Wc), ref(Xn1), p, ref(yc));
    thread t8(matrixMultiplicationSingleThread, ref(Wo), ref(Xn1), p, ref(yo));
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    Serialized2DVector y_serial = serialize2DVector(yf);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'f');
    delete[] y_serial.data;

    y_serial = serialize2DVector(yi);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'i');
    delete[] y_serial.data;

    y_serial = serialize2DVector(yc);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'c');
    delete[] y_serial.data;

    y_serial = serialize2DVector(yo);
    ecall_online_phase(global_eid, y_serial.data, y_serial.size, 'o');
    delete[] y_serial.data;
    ecall_activation_function(global_eid);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elasped3 = end - start;
    cout << "elasped time (4-way): " << elasped3.count() << " ms" << endl;
    File << "4-thread    "<< elasped3.count() << endl;
    File.close();
#endif
    ///////////////////////////////////////////////////////////////////////////////
    // method 0

    // start = chrono::high_resolution_clock::now();
    // vector<vector<int64_t>> test = matrixMultiply(Wf, Xn, p, 2);
    // matrixMultiply(Wi, Xn, p, 2);
    // matrixMultiply(Wc, Xn, p, 2);
    // matrixMultiply(Wo, Xn, p, 2);
    // end = chrono::high_resolution_clock::now();
    // chrono::duration<double, milli> elasped4 = end - start;
    // cout << "elasped time (untrusted): " << elasped4.count() << " ms" <<
    // endl;
    ///////////////////////////////////////////////////////////////////////////////

    // print(yf, "yf");
    // print(yf1, "yf1");
    // print(test, "test");
    // destroy the enclave

   
    sgx_destroy_enclave(global_eid);

    return 0;
}

/* ocall functions (untrusted) */

void ocall_print(const char *str) {
    printf("%s", str);
    return;
}

void ocall_print_double(double value) {
    printf("%f\n", value);
    // cout << value;
}

void ocall_print_int32(int32_t value) {
    // printf("%d",value);
    cout << value;
}

void ocall_print_int64(int64_t value) {
    // printf("%ld \n", value);
    cout << value << endl;
}

void ocall_print_vector(double *vec, size_t row, size_t col) {
    // cout << vec << endl;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            cout << vec[i * row + j] << " ";
        }
        cout << endl;
    }
}
