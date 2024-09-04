#include "Enclave_u.h"
#include <errno.h>

typedef struct ms_ecall_get_X_n_t {
	char* ms_Xnoisy;
	size_t ms_size;
} ms_ecall_get_X_n_t;

typedef struct ms_ecall_get_W_n_t {
	char* ms_Wnoisy;
	size_t ms_size;
	char ms_w_type;
} ms_ecall_get_W_n_t;

typedef struct ms_ecall_online_phase_t {
	char* ms_Yn;
	size_t ms_size;
	char ms_w_type;
} ms_ecall_online_phase_t;

typedef struct ms_ocall_print_t {
	const char* ms_str;
} ms_ocall_print_t;

typedef struct ms_ocall_print_vector_t {
	double* ms_vec;
	size_t ms_row;
	size_t ms_col;
} ms_ocall_print_vector_t;

typedef struct ms_ocall_print_double_t {
	double ms_value;
} ms_ocall_print_double_t;

typedef struct ms_ocall_print_int32_t {
	int32_t ms_value;
} ms_ocall_print_int32_t;

typedef struct ms_ocall_print_int64_t {
	int64_t ms_value;
} ms_ocall_print_int64_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print(void* pms)
{
	ms_ocall_print_t* ms = SGX_CAST(ms_ocall_print_t*, pms);
	ocall_print(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_print_vector(void* pms)
{
	ms_ocall_print_vector_t* ms = SGX_CAST(ms_ocall_print_vector_t*, pms);
	ocall_print_vector(ms->ms_vec, ms->ms_row, ms->ms_col);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_print_double(void* pms)
{
	ms_ocall_print_double_t* ms = SGX_CAST(ms_ocall_print_double_t*, pms);
	ocall_print_double(ms->ms_value);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_print_int32(void* pms)
{
	ms_ocall_print_int32_t* ms = SGX_CAST(ms_ocall_print_int32_t*, pms);
	ocall_print_int32(ms->ms_value);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_print_int64(void* pms)
{
	ms_ocall_print_int64_t* ms = SGX_CAST(ms_ocall_print_int64_t*, pms);
	ocall_print_int64(ms->ms_value);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[5];
} ocall_table_Enclave = {
	5,
	{
		(void*)Enclave_ocall_print,
		(void*)Enclave_ocall_print_vector,
		(void*)Enclave_ocall_print_double,
		(void*)Enclave_ocall_print_int32,
		(void*)Enclave_ocall_print_int64,
	}
};
sgx_status_t ecall_activation_function(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_offline_phase(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_get_X_n(sgx_enclave_id_t eid, char* Xnoisy, size_t size)
{
	sgx_status_t status;
	ms_ecall_get_X_n_t ms;
	ms.ms_Xnoisy = Xnoisy;
	ms.ms_size = size;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_get_W_n(sgx_enclave_id_t eid, char* Wnoisy, size_t size, char w_type)
{
	sgx_status_t status;
	ms_ecall_get_W_n_t ms;
	ms.ms_Wnoisy = Wnoisy;
	ms.ms_size = size;
	ms.ms_w_type = w_type;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_online_phase(sgx_enclave_id_t eid, char* Yn, size_t size, char w_type)
{
	sgx_status_t status;
	ms_ecall_online_phase_t ms;
	ms.ms_Yn = Yn;
	ms.ms_size = size;
	ms.ms_w_type = w_type;
	status = sgx_ecall(eid, 4, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_compute_y(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 5, &ocall_table_Enclave, NULL);
	return status;
}

