
#include <gmp.h>

#include "../struct.h"

struct int_value
{
	struct value super;
	
	mpz_t mpz;
};

