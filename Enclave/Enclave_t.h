#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _Serialized2DVector
#define _Serialized2DVector
typedef struct Serialized2DVector {
	size_t size;
	size_t rows;
	size_t cols;
	char* data;
} Serialized2DVector;
#endif

void ecall_activation_function(void);
void ecall_offline_phase(void);
void ecall_get_X_n(char* Xnoisy, size_t size);
void ecall_get_W_n(char* Wnoisy, size_t size, char w_type);
void ecall_online_phase(char* Yn, size_t size, char w_type);
void ecall_compute_y(void);

sgx_status_t SGX_CDECL ocall_print(const char* str);
sgx_status_t SGX_CDECL ocall_print_vector(double* vec, size_t row, size_t col);
sgx_status_t SGX_CDECL ocall_print_double(double value);
sgx_status_t SGX_CDECL ocall_print_int32(int32_t value);
sgx_status_t SGX_CDECL ocall_print_int64(int64_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
