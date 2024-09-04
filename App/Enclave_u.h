#ifndef ENCLAVE_U_H__
#define ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


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

#ifndef OCALL_PRINT_DEFINED__
#define OCALL_PRINT_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print, (const char* str));
#endif
#ifndef OCALL_PRINT_VECTOR_DEFINED__
#define OCALL_PRINT_VECTOR_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_vector, (double* vec, size_t row, size_t col));
#endif
#ifndef OCALL_PRINT_DOUBLE_DEFINED__
#define OCALL_PRINT_DOUBLE_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_double, (double value));
#endif
#ifndef OCALL_PRINT_INT32_DEFINED__
#define OCALL_PRINT_INT32_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_int32, (int32_t value));
#endif
#ifndef OCALL_PRINT_INT64_DEFINED__
#define OCALL_PRINT_INT64_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_int64, (int64_t value));
#endif

sgx_status_t ecall_activation_function(sgx_enclave_id_t eid);
sgx_status_t ecall_offline_phase(sgx_enclave_id_t eid);
sgx_status_t ecall_get_X_n(sgx_enclave_id_t eid, char* Xnoisy, size_t size);
sgx_status_t ecall_get_W_n(sgx_enclave_id_t eid, char* Wnoisy, size_t size, char w_type);
sgx_status_t ecall_online_phase(sgx_enclave_id_t eid, char* Yn, size_t size, char w_type);
sgx_status_t ecall_compute_y(sgx_enclave_id_t eid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
