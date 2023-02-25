
#include <gmp.h>

struct value* new_int_value(
	int value);
	
struct value* new_int_value_from_str(
	const char* str);

struct value* new_int_value_from_mpz(
	mpz_t mpz);
