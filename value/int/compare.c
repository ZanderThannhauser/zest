
#include <gmp.h>

#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_int_values(
	const struct value* a,
	const struct value* b)
{
	const struct int_value *A = (void*) a, *B = (void*) b;
	
	return mpz_cmp(A->mpz, B->mpz);
}

