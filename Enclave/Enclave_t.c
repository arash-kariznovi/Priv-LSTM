#include "Enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_ecall_activation_function(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_activation_function();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_offline_phase(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_offline_phase();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_X_n(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_X_n_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_get_X_n_t* ms = SGX_CAST(ms_ecall_get_X_n_t*, pms);
	ms_ecall_get_X_n_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_get_X_n_t), ms, sizeof(ms_ecall_get_X_n_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_Xnoisy = __in_ms.ms_Xnoisy;
	size_t _tmp_size = __in_ms.ms_size;
	size_t _len_Xnoisy = _tmp_size;
	char* _in_Xnoisy = NULL;

	CHECK_UNIQUE_POINTER(_tmp_Xnoisy, _len_Xnoisy);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_Xnoisy != NULL && _len_Xnoisy != 0) {
		if ( _len_Xnoisy % sizeof(*_tmp_Xnoisy) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_Xnoisy = (char*)malloc(_len_Xnoisy)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_Xnoisy, 0, _len_Xnoisy);
	}
	ecall_get_X_n(_in_Xnoisy, _tmp_size);
	if (_in_Xnoisy) {
		if (memcpy_verw_s(_tmp_Xnoisy, _len_Xnoisy, _in_Xnoisy, _len_Xnoisy)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_Xnoisy) free(_in_Xnoisy);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_W_n(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_W_n_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_get_W_n_t* ms = SGX_CAST(ms_ecall_get_W_n_t*, pms);
	ms_ecall_get_W_n_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_get_W_n_t), ms, sizeof(ms_ecall_get_W_n_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_Wnoisy = __in_ms.ms_Wnoisy;
	size_t _tmp_size = __in_ms.ms_size;
	size_t _len_Wnoisy = _tmp_size;
	char* _in_Wnoisy = NULL;

	CHECK_UNIQUE_POINTER(_tmp_Wnoisy, _len_Wnoisy);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_Wnoisy != NULL && _len_Wnoisy != 0) {
		if ( _len_Wnoisy % sizeof(*_tmp_Wnoisy) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_Wnoisy = (char*)malloc(_len_Wnoisy)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_Wnoisy, 0, _len_Wnoisy);
	}
	ecall_get_W_n(_in_Wnoisy, _tmp_size, __in_ms.ms_w_type);
	if (_in_Wnoisy) {
		if (memcpy_verw_s(_tmp_Wnoisy, _len_Wnoisy, _in_Wnoisy, _len_Wnoisy)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_Wnoisy) free(_in_Wnoisy);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_online_phase(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_online_phase_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_online_phase_t* ms = SGX_CAST(ms_ecall_online_phase_t*, pms);
	ms_ecall_online_phase_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_online_phase_t), ms, sizeof(ms_ecall_online_phase_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_Yn = __in_ms.ms_Yn;
	size_t _tmp_size = __in_ms.ms_size;
	size_t _len_Yn = _tmp_size;
	char* _in_Yn = NULL;

	CHECK_UNIQUE_POINTER(_tmp_Yn, _len_Yn);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_Yn != NULL && _len_Yn != 0) {
		if ( _len_Yn % sizeof(*_tmp_Yn) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_Yn = (char*)malloc(_len_Yn);
		if (_in_Yn == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_Yn, _len_Yn, _tmp_Yn, _len_Yn)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	ecall_online_phase(_in_Yn, _tmp_size, __in_ms.ms_w_type);

err:
	if (_in_Yn) free(_in_Yn);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_compute_y(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_compute_y();
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[6];
} g_ecall_table = {
	6,
	{
		{(void*)(uintptr_t)sgx_ecall_activation_function, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_offline_phase, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_get_X_n, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_get_W_n, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_online_phase, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_compute_y, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[5][6];
} g_dyn_entry_table = {
	5,
	{
		{0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_t));
	ocalloc_size -= sizeof(ms_ocall_print_t);

	if (str != NULL) {
		if (memcpy_verw_s(&ms->ms_str, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}

	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_vector(double* vec, size_t row, size_t col)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_vec = sizeof(double);

	ms_ocall_print_vector_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_vector_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(vec, _len_vec);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (vec != NULL) ? _len_vec : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_vector_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_vector_t));
	ocalloc_size -= sizeof(ms_ocall_print_vector_t);

	if (vec != NULL) {
		if (memcpy_verw_s(&ms->ms_vec, sizeof(double*), &__tmp, sizeof(double*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_vec % sizeof(*vec) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, vec, _len_vec)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_vec);
		ocalloc_size -= _len_vec;
	} else {
		ms->ms_vec = NULL;
	}

	if (memcpy_verw_s(&ms->ms_row, sizeof(ms->ms_row), &row, sizeof(row))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_col, sizeof(ms->ms_col), &col, sizeof(col))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_double(double value)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_print_double_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_double_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_double_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_double_t));
	ocalloc_size -= sizeof(ms_ocall_print_double_t);

	if (memcpy_verw_s(&ms->ms_value, sizeof(ms->ms_value), &value, sizeof(value))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_int32(int32_t value)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_print_int32_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_int32_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_int32_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_int32_t));
	ocalloc_size -= sizeof(ms_ocall_print_int32_t);

	if (memcpy_verw_s(&ms->ms_value, sizeof(ms->ms_value), &value, sizeof(value))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_int64(int64_t value)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_print_int64_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_int64_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_int64_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_int64_t));
	ocalloc_size -= sizeof(ms_ocall_print_int64_t);

	if (memcpy_verw_s(&ms->ms_value, sizeof(ms->ms_value), &value, sizeof(value))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

